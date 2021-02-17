// #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
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

#define OPEN_LED() digitalWrite(LED_BUILTIN, LOW)
#define CLOSE_LED() digitalWrite(LED_BUILTIN, HIGH)
#define TOGGLE_LED() digitalToggle(LED_BUILTIN)

#define LED_BUILTIN PB11

#define LTE_TX PA2
#define LTE_RX PA3
/*  ---------------------------------------------------------------------------------------------------------------*/
HardwareSerial LTE_Serial(LTE_RX, LTE_TX);

unsigned long lastMs = 0;

QuectelClient espClient;
PubSubClient mqttClient(espClient);

STM32Flash flash;
Max31865 rtd1, rtd2;

float internal_temperature = 0;
float external_templature = 0;

/*  ---------------------------------------------------------------------------------------------------------------*/

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);

    if (strstr(topic, ALINK_TOPIC_PROP_SET))
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
        while (!connectAliyunMQTT(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
        }
        Serial.println("MQTT connect succeed!");
        // client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
        mqttClient.subscribe(ALINK_TOPIC_PROP_SET);
        Serial.println("subscribe done");
    }
}

void mqttIntervalPost()
{
    char param[128];
    char jsonBuf[128];

    sprintf(param, "{\"internal_temperature\":%d.%d,\"external_templature\":%d.%d}",
            (int)internal_temperature, (int)(internal_temperature * 100) % 100,
            (int)external_templature, (int)(external_templature * 100) % 100);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}

void setup()
{
    // pinMode(PWR4G_PIN, OUTPUT), OPEN_4GPWR();
    // pinMode(VDD_PIN, OUTPUT), CLOSE_VDD();

    // pinMode(PWRKEY_PIN, OUTPUT);
    // PWRKEY_LOW(), delay(800), PWRKEY_HIGH();

    /* initialize serial for debugging */
    Serial.begin(115200);
    flash.begin();

    rtd1.begin(PA4, PA5, PA6, PA7);
    rtd2.begin(PB12, PB13, PB14, PB15);

    LTE_Serial.begin(115200);

    Serial.println("begin...");

    mqttClient.setCallback(callback);

    char *str = "hello123456789123451";
    char *read_buf[10] = {0};

    flash.writeBuffer((uint8_t *)str, strlen(str) + 1);
    flash.readBuffer((uint8_t *)read_buf, strlen(str) + 1);
    Serial.println((char *)read_buf);
    Serial.flush();
    while (1)
        ;
}

// the loop function runs over and over again forever
void loop()
{
    // AT_bypass();

    if (millis() - lastMs >= 5000)
    {
        lastMs = millis();
        mqttCheckConnect();

        /* Post */
        mqttIntervalPost();
    }

    mqttClient.loop();

    internal_temperature = rtd1.getTemperature();
    external_templature = rtd2.getTemperature();
    Serial.printf("t1=%d,t2=%d\r\n", (int)internal_temperature, (int)external_templature);

    delay(500); // ms
}
