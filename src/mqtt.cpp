// #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "UART_Interface.h"
#include "aliyun_mqtt.h"
#include "modules/quectel/quectel_client.h"

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
HardwareSerial LTE_Serial(LTE_RX, LTE_TX);

unsigned long lastMs = 0;

QuectelClient espClient;
PubSubClient mqttClient(espClient);

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
    char param[32];
    char jsonBuf[128];

    sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}

void setup()
{

    /* initialize serial for debugging */
    Serial.begin(115200);
    LTE_Serial.begin(115200);

    Serial.println("Start");

    mqttClient.setCallback(callback);

    // pinMode(PWR4G_PIN, OUTPUT), OPEN_4GPWR();
    // pinMode(VDD_PIN, OUTPUT), CLOSE_VDD();

    // pinMode(PWRKEY_PIN, OUTPUT);
    // PWRKEY_LOW(), delay(800), PWRKEY_HIGH();
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

    unsigned int WAIT_MS = 2000;
    delay(WAIT_MS); // ms
    Serial.println(millis() / WAIT_MS);
}
