// #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "modules/host_int/host_server.h"
#include "modules/low_power/STM32LowPower.h"
#include "modules/flash/stm32_flash.h"
#include "modules/max31865/max31865.h"
#include "UART_Interface.h"
#include "modules/quectel/quectel_client.h"
#include "aliyun_mqtt.h"
/*  ---------------------------------------------------------------------------------------------------------------*/

#define PRODUCT_KEY "a1vhKQ19mpk"
#define DEVICE_NAME "CkCuGNUkYm6sWVsbGQl5"
#define DEVICE_SECRET "ac8e301a20d0ce5f39c06166a3862435"

#define ALINK_BODY_FORMAT "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_METHOD_PROP_POST "thing.event.property.post"

#define VDD_PIN PA11
#define LED_BUILTIN PB11

#define OPEN_LED() digitalWrite(LED_BUILTIN, LOW)
#define CLOSE_LED() digitalWrite(LED_BUILTIN, HIGH)
#define TOGGLE_LED() digitalToggle(LED_BUILTIN)

#define OPEN_VDD() digitalWrite(VDD_PIN, LOW)
#define CLOSE_VDD() digitalWrite(VDD_PIN, HIGH)

#define LTE_TX PA2
#define LTE_RX PA3

#define STANDBY_TIME (5 * 1000)
/*  ---------------------------------------------------------------------------------------------------------------*/
HardwareSerial LTE_Serial(LTE_RX, LTE_TX);

unsigned long lastMs = 0;

QuectelClient espClient;
PubSubClient mqttClient(espClient);
STM32LowPower lowPower;
STM32Flash flash;
Max31865 rtd1, rtd2;
// DynamicJsonBuffer jsonBuffer;
// StaticJsonBuffer<1024> jsonBuffer;
HostServer hostServer; //与主机通讯接口

float internal_temperature = 0;
float external_templature = 0;

// String configJSON = R"({
// "product_key":"a1vhKQ19mpk",
// "device_name":"CkCuGNUkYm6sWVsbGQl5",
// "device_secret":"ac8e301a20d0ce5f39c06166a3862435",
// "collect_interval":500,
// "total_report_count":10,
// "ip_address":"101.133.196.109",
// "port":1883,
// "standby_time":5
// })";

String configJSON = R"({
"product_key":"a1ncEmjzVzj",
"device_name":"pC8b52IBYtcN5E4a4hjV",
"device_secret":"de71106836fb59df395ad76eb9014ec9",
"collect_interval":500,
"total_report_count":10,
"ip_address":"101.133.196.109",
"port":1883,
"standby_time":5
})";

#define TEMP_LIST_LEN 100
struct SystemStatus
{
public:
    uint32_t collectCNT = 0;
    uint16_t index = 0;
    float internalTemperatures[TEMP_LIST_LEN] = {0};
    float externalTemperatures[TEMP_LIST_LEN] = {0};
    std::string tostring()
    {
        static std::string out = "";
        out += "/* SystemStatus ---------------------------------------------------------------------------------------------------------------*/\r\n";
        out += "collectCNT:" + std::to_string(collectCNT) + "\r\n";
        out += "index:" + std::to_string(index) + "\r\n";
        out += "[";
        for (int i = 0; i < TEMP_LIST_LEN; i++)
        {
            out += std::to_string((int)(internalTemperatures[i])) + ",";
        }
        out += "]\r\n";
        out += "[";
        for (int i = 0; i < TEMP_LIST_LEN; i++)
        {
            out += std::to_string((int)(externalTemperatures[i])) + ",";
        }
        out += "]\r\n";
        out += "/* SystemStatus ---------------------------------------------------------------------------------------------------------------*/\r\n";
        return out;
    }
};
SystemStatus sysStatus; //system running status
// bool isRefactory = false;
bool isRefactory = false;

/*  ---------------------------------------------------------------------------------------------------------------*/

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);

    if (strstr(topic, hostServer.bean.alink_topic_prop_set.c_str()))
    {
        StaticJsonBuffer<100> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);
        if (!root.success())
        {
            Serial.println("parseObject() failed");
            return;
        }
    }
}

void mqttCheckConnect()
{
    if (!mqttClient.connected())
    {
        while (!connectAliyunMQTT(mqttClient,
                                  hostServer.bean.product_key.c_str(),
                                  hostServer.bean.device_name.c_str(),
                                  hostServer.bean.device_secret.c_str()))
        {
        }
        Serial.println("MQTT connect succeed!");
        mqttClient.subscribe(hostServer.bean.alink_topic_prop_set.c_str());
        Serial.println("subscribe done");
    }
}

void mqttIntervalPost()
{
    static char param[128];
    static char jsonBuf[1024];

    sprintf(param, "{\"internal_temperature\":%d.%d,\"external_templature\":%d.%d}",
            (int)internal_temperature, (int)(internal_temperature * 100) % 100,
            (int)external_templature, (int)(external_templature * 100) % 100);

    sprintf(jsonBuf, hostServer.bean.alink_body_format.c_str(), hostServer.bean.alink_method_prop_post.c_str(), param);
    Serial.println(String("publish:") + jsonBuf);
    bool isok = mqttClient.publish(hostServer.bean.alink_topic_prop_post.c_str(), jsonBuf);
    if (isok)
    {
        Serial.println("publish fail,reset...");
        Serial.flush();
        delay(200);
        HAL_NVIC_SystemReset();
    }
}

void setup()
{
    // pinMode(PWR4G_PIN, OUTPUT), OPEN_4GPWR();
    // pinMode(VDD_PIN, OUTPUT), CLOSE_VDD();

    // pinMode(PWRKEY_PIN, OUTPUT);
    // PWRKEY_LOW(), delay(800), PWRKEY_HIGH();

    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(VDD_PIN, OUTPUT), CLOSE_VDD();
    OPEN_VDD();

    /* initialize serial for debugging */
    Serial.begin(115200);
    Serial.println("begin...");

    lowPower.begin();
    flash.begin();

    hostServer.parseBean(configJSON.c_str()); //set config by 'configJSON'
    Serial.println("system config:");
    String out = hostServer.bean.tostring();
    Serial.println(out);

    rtd1.begin(PA4, PA5, PA6, PA7);
    rtd2.begin(PB12, PB13, PB14, PB15);

    LTE_Serial.begin(115200);

    mqttClient.setCallback(callback);

    flash.readBuffer((uint8_t *)&sysStatus, sizeof(SystemStatus));
    Serial.println("read 'sysStatus'");
    std::string flashStr = sysStatus.tostring();
    Serial.println(flashStr.c_str());
    int i;
    for (i = 0; i < sizeof(SystemStatus); i++)
    {
        uint8_t *pbuf = (uint8_t *)&sysStatus;
        if (pbuf[i] != 0xff)
            break;
    }
    if (isRefactory)
    {
        memset((uint8_t *)&sysStatus, 0, sizeof(SystemStatus));
    }
    if (i == sizeof(SystemStatus)) //if 'sysStatus' memory both are 0xff,reset it to 0
        memset((uint8_t *)&sysStatus, 0, sizeof(SystemStatus));
}

// the loop function runs over and over again forever
void loop()
{
    // AT_bypass();

    internal_temperature = rtd1.getTemperature();
    external_templature = rtd2.getTemperature();
    // Serial.printf("t1=%d,t2=%d\r\n", (int)internal_temperature, (int)external_templature);

    if (millis() - lastMs >= 5000)
    {
        // lastMs = millis();
        mqttCheckConnect();

        /* Post */
        mqttIntervalPost();
        lastMs = millis();
    }
    mqttClient.loop();

    static uint32_t runLED_c = 0;
    if (millis() - runLED_c > 100)
    {
        TOGGLE_LED();
        runLED_c = millis();
    }
}
