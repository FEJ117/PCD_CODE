/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

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
#define ModeSwitch_Pin GPIO_PIN_13
#define ModeSwitch_GPIO_Port GPIOC
#define Summer_Pin GPIO_PIN_7
#define Summer_GPIO_Port GPIOA
#define Button4_Pin GPIO_PIN_12
#define Button4_GPIO_Port GPIOB
#define Button3_Pin GPIO_PIN_13
#define Button3_GPIO_Port GPIOB
#define Button2_Pin GPIO_PIN_14
#define Button2_GPIO_Port GPIOB
#define Button1_Pin GPIO_PIN_15
#define Button1_GPIO_Port GPIOB
#define PS2CLK_Pin GPIO_PIN_11
#define PS2CLK_GPIO_Port GPIOA
#define PS2CLK_EXTI_IRQn EXTI4_15_IRQn
#define PS2DAT_Pin GPIO_PIN_12
#define PS2DAT_GPIO_Port GPIOA
#define LED2B_Pin GPIO_PIN_4
#define LED2B_GPIO_Port GPIOB
#define LED2G_Pin GPIO_PIN_5
#define LED2G_GPIO_Port GPIOB
#define LED2R_Pin GPIO_PIN_6
#define LED2R_GPIO_Port GPIOB
#define LED1B_Pin GPIO_PIN_7
#define LED1B_GPIO_Port GPIOB
#define LED1G_Pin GPIO_PIN_8
#define LED1G_GPIO_Port GPIOB
#define LED1R_Pin GPIO_PIN_9
#define LED1R_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
