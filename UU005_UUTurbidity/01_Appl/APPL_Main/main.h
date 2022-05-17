/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "EKM_CMD.h"
#include "EKM_Buffer.h"
#include "EKM_Modbus.h"
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "APPL_ads1115.h"
#include "APPL_DAC80502.h"
#include "APPL_TPL0401X.h"
#include "Appl_CMD.h"
#include "Appl_Module.h"
#include "Appl_Task.h"

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
extern uint8_t Systick_count;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART2_DE_Pin GPIO_PIN_1
#define USART2_DE_GPIO_Port GPIOA
#define AN_EN_Pin GPIO_PIN_4
#define AN_EN_GPIO_Port GPIOA
#define LAMP_EN_Pin GPIO_PIN_5
#define LAMP_EN_GPIO_Port GPIOA
#define LED_STATUS_Pin GPIO_PIN_6
#define LED_STATUS_GPIO_Port GPIOA
#define TurnON_PUMP_Pin GPIO_PIN_7
#define TurnON_PUMP_GPIO_Port GPIOA
#define MOD0_Pin GPIO_PIN_0
#define MOD0_GPIO_Port GPIOB
#define MOD1_Pin GPIO_PIN_1
#define MOD1_GPIO_Port GPIOB
#define I2C1_ALE_Pin GPIO_PIN_12
#define I2C1_ALE_GPIO_Port GPIOA
#define I2C1_ALEB5_Pin GPIO_PIN_5
#define I2C1_ALEB5_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
