#include <Arduino.h>
#include "modules/low_power/STM32LowPower.h"
#include "ethernet.h"

#define LED_BUILTIN PB11
#define PWR4G_PIN PB0
#define VDD_PIN PA11
#define PWRKEY_PIN PA15

#define SHUTDOWN_TIME (10 * 1000)

#define OPEN_4GPWR() digitalWrite(PWR4G_PIN, HIGH)
#define CLOSE_4GPWR() digitalWrite(PWR4G_PIN, LOW)

#define OPEN_VDD() digitalWrite(VDD_PIN, LOW)
#define CLOSE_VDD() digitalWrite(VDD_PIN, HIGH)

//LTE module's pwrkey pin low > 500ms so start
#define PWRKEY_HIGH() digitalWrite(PWRKEY_PIN, LOW)
#define PWRKEY_LOW() digitalWrite(PWRKEY_PIN, HIGH)

#define OPEN_LED() digitalWrite(LED_BUILTIN, LOW)
#define CLOSE_LED() digitalWrite(LED_BUILTIN, HIGH)
#define TOGGLE_LED() digitalToggle(LED_BUILTIN)

#define LTE_TX PA2
#define LTE_RX PA3
HardwareSerial LTE_Serial(LTE_RX, LTE_TX);

Ethernet eth = Ethernet();

// const char apn[10] = "CMNET";
const char apn[10] = "UNINET";
const char URL[100] = "116.5.192.226";
char http_cmd[100] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\n\r\n\r";
int port = 8081;

int ret = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Begin...");
  // wio.Power_On();
  MODULE_PORT.begin(115200);
  pinMode(PWR4G_PIN, OUTPUT), OPEN_4GPWR();
  pinMode(VDD_PIN, OUTPUT), CLOSE_VDD();

  pinMode(PWRKEY_PIN, OUTPUT);
  PWRKEY_LOW(), delay(800), PWRKEY_HIGH();
  // eth.Power_On();
  while (false == eth.Check_If_Power_On())
  {
    Serial.println("Waitting for module to alvie...");
    delay(1000);
  }

  while (!eth.init())
  {
    delay(1000);
    Serial.println("Accessing network...");
  }
  Serial.println("Initialize done...");

  eth.join(apn);
  Serial.print("\n\rIP: ");
  Serial.print(eth.ip_string);

  if (eth.connect(URL, port, TCP))
  {
    eth.write(http_cmd);
    while (MODULE_PORT.available())
    {
      serialDebug.write(MODULE_PORT.read());
    }
    eth.close(1);
  }
  else
  {
    Serial.println("Connect Error!");
  }
}

void loop()
{
  /* Debug */
  AT_bypass();
}