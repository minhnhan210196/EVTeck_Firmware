/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

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
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VAL_Pin GPIO_PIN_2
#define VAL_GPIO_Port GPIOE
#define MOTOR_Pin GPIO_PIN_3
#define MOTOR_GPIO_Port GPIOE
#define MUX_A1_Pin GPIO_PIN_13
#define MUX_A1_GPIO_Port GPIOF
#define MUX_B1_Pin GPIO_PIN_14
#define MUX_B1_GPIO_Port GPIOF
#define MUX_B2_Pin GPIO_PIN_15
#define MUX_B2_GPIO_Port GPIOF
#define MUX_B2G0_Pin GPIO_PIN_0
#define MUX_B2G0_GPIO_Port GPIOG
#define MUX_A3_Pin GPIO_PIN_1
#define MUX_A3_GPIO_Port GPIOG
#define MUX_B3_Pin GPIO_PIN_7
#define MUX_B3_GPIO_Port GPIOE
#define MUX_A4_Pin GPIO_PIN_8
#define MUX_A4_GPIO_Port GPIOE
#define MUX_B4_Pin GPIO_PIN_9
#define MUX_B4_GPIO_Port GPIOE
#define ADC_CNV1_Pin GPIO_PIN_8
#define ADC_CNV1_GPIO_Port GPIOD
#define ADC_BUSY1_Pin GPIO_PIN_9
#define ADC_BUSY1_GPIO_Port GPIOD
#define ADC_RS1_Pin GPIO_PIN_10
#define ADC_RS1_GPIO_Port GPIOD
#define ADC_CS1_Pin GPIO_PIN_11
#define ADC_CS1_GPIO_Port GPIOD
#define ADC_CNV2_Pin GPIO_PIN_2
#define ADC_CNV2_GPIO_Port GPIOG
#define ADC_BUSY2_Pin GPIO_PIN_3
#define ADC_BUSY2_GPIO_Port GPIOG
#define ADC_RS2_Pin GPIO_PIN_4
#define ADC_RS2_GPIO_Port GPIOG
#define ADC_CS2_Pin GPIO_PIN_5
#define ADC_CS2_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
