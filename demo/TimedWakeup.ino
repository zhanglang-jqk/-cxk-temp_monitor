/*
  TimedWakeup

  This sketch demonstrates the usage of Internal Interrupts to wakeup a chip
  in deep sleep mode.

  In this sketch, the internal RTC will wake up the processor every second.

  This example code is in the public domain.
*/

#include "STM32LowPower.h"

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

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT), OPEN_LED();
  Serial.begin(9600);
  LTE_Serial.begin(9600);
  Serial.println("start...");

  pinMode(PWR4G_PIN, OUTPUT), OPEN_4GPWR();
  pinMode(VDD_PIN, OUTPUT), CLOSE_VDD();

  pinMode(PWRKEY_PIN, OUTPUT);
  PWRKEY_LOW(), delay(600), PWRKEY_HIGH();
  // Configure low power
  LowPower.begin();
}

void loop()
{
  // digitalWrite(LED_BUILTIN, HIGH);
  // LowPower.shutdown(SHUTDOWN_TIME);
  // digitalWrite(LED_BUILTIN, LOW);
  // LowPower.shutdown(SHUTDOWN_TIME);
  // Serial.println("hello");

  // LTE_Serial.println("hello");
  // Serial.println("serial1 hello");
  // delay(100);

  if (Serial.available())
  {
    uint8_t b = Serial.read();
    // LTE_Serial.printf("%c", b);
    LTE_Serial.write(b);
    LTE_Serial.flush();
  }
  TOGGLE_LED();
}

/********************************************************************************
*  
* @biref£º set 
* @biref£º set crystal Oscillator to internal
* @param£º
********************************************************************************/
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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