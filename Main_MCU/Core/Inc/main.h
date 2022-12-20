/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define DIN_4_Pin GPIO_PIN_2
#define DIN_4_GPIO_Port GPIOE
#define DIN_5_Pin GPIO_PIN_3
#define DIN_5_GPIO_Port GPIOE
#define DIN_6_Pin GPIO_PIN_4
#define DIN_6_GPIO_Port GPIOE
#define DIN_7_Pin GPIO_PIN_5
#define DIN_7_GPIO_Port GPIOE
#define DIN_8_Pin GPIO_PIN_6
#define DIN_8_GPIO_Port GPIOE
#define DIN_9_Pin GPIO_PIN_13
#define DIN_9_GPIO_Port GPIOC
#define DIN_10_Pin GPIO_PIN_14
#define DIN_10_GPIO_Port GPIOC
#define DIN_11_Pin GPIO_PIN_15
#define DIN_11_GPIO_Port GPIOC
#define ERESET_Pin GPIO_PIN_0
#define ERESET_GPIO_Port GPIOA
#define LCD0_Pin GPIO_PIN_0
#define LCD0_GPIO_Port GPIOB
#define LCD1_Pin GPIO_PIN_1
#define LCD1_GPIO_Port GPIOB
#define LCD2_Pin GPIO_PIN_2
#define LCD2_GPIO_Port GPIOB
#define DOUT_0_Pin GPIO_PIN_7
#define DOUT_0_GPIO_Port GPIOE
#define DOUT_1_Pin GPIO_PIN_8
#define DOUT_1_GPIO_Port GPIOE
#define DOUT_2_Pin GPIO_PIN_9
#define DOUT_2_GPIO_Port GPIOE
#define DOUT_3_Pin GPIO_PIN_10
#define DOUT_3_GPIO_Port GPIOE
#define DOUT_4_Pin GPIO_PIN_11
#define DOUT_4_GPIO_Port GPIOE
#define DOUT_5_Pin GPIO_PIN_12
#define DOUT_5_GPIO_Port GPIOE
#define DOUT_6_Pin GPIO_PIN_13
#define DOUT_6_GPIO_Port GPIOE
#define DOUT_7_Pin GPIO_PIN_14
#define DOUT_7_GPIO_Port GPIOE
#define DOUT_8_Pin GPIO_PIN_15
#define DOUT_8_GPIO_Port GPIOE
#define DOUT_9_Pin GPIO_PIN_10
#define DOUT_9_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_14
#define LED_GPIO_Port GPIOB
#define LCDLED_Pin GPIO_PIN_15
#define LCDLED_GPIO_Port GPIOB
#define DOUT_10_Pin GPIO_PIN_8
#define DOUT_10_GPIO_Port GPIOD
#define DOUT_11_Pin GPIO_PIN_9
#define DOUT_11_GPIO_Port GPIOD
#define RS485_TX_Pin GPIO_PIN_8
#define RS485_TX_GPIO_Port GPIOC
#define LCD3_Pin GPIO_PIN_3
#define LCD3_GPIO_Port GPIOB
#define LCDR_Pin GPIO_PIN_4
#define LCDR_GPIO_Port GPIOB
#define LCDE_Pin GPIO_PIN_5
#define LCDE_GPIO_Port GPIOB
#define LCDA_Pin GPIO_PIN_6
#define LCDA_GPIO_Port GPIOB
#define DIN_0_Pin GPIO_PIN_8
#define DIN_0_GPIO_Port GPIOB
#define DIN_1_Pin GPIO_PIN_9
#define DIN_1_GPIO_Port GPIOB
#define DIN_2_Pin GPIO_PIN_0
#define DIN_2_GPIO_Port GPIOE
#define DIN_3_Pin GPIO_PIN_1
#define DIN_3_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
