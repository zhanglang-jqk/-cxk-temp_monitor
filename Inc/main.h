/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS1_Pin GPIO_PIN_4
#define CS1_GPIO_Port GPIOA
#define SCLK1_Pin GPIO_PIN_5
#define SCLK1_GPIO_Port GPIOA
#define SDO1_Pin GPIO_PIN_6
#define SDO1_GPIO_Port GPIOA
#define SDI1_Pin GPIO_PIN_7
#define SDI1_GPIO_Port GPIOA
#define CS2_Pin GPIO_PIN_12
#define CS2_GPIO_Port GPIOB
#define SCLK2_Pin GPIO_PIN_13
#define SCLK2_GPIO_Port GPIOB
#define SDO2_Pin GPIO_PIN_14
#define SDO2_GPIO_Port GPIOB
#define SDI2_Pin GPIO_PIN_15
#define SDI2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
