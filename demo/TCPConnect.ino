#include <Arduino.h>
#include "STM32LowPower.h"
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

const char apn[10] = "CMNET";
// const char apn[10] = "UNINET";
const char URL[100] = "116.5.195.83";
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
  // delay(2000);  //确保4G模块稳定启动

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

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}