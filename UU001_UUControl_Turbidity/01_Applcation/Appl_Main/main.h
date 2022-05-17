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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32l4xx_hal.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "EKM_Buffer.h"
#include "EKM_CMD.h"
#include "EKM_Modbus.h"
#include "Appl_LTE.h"
#include "Appl_IOCon.h"
#include "Appl_Module.h"
#include "Appl_IOCon.h"
#include "Appl_Task.h"
#include "Appl_Status.h"
#include "Appl_Turbidity.h"


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
extern uint8_t Systick_count;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I2C1_INT_Pin GPIO_PIN_0
#define I2C1_INT_GPIO_Port GPIOF
#define WD_WDI_Pin GPIO_PIN_1
#define WD_WDI_GPIO_Port GPIOF
#define WD_MR_Pin GPIO_PIN_2
#define WD_MR_GPIO_Port GPIOF
#define I2C1_WP_Pin GPIO_PIN_3
#define I2C1_WP_GPIO_Port GPIOC
#define USART2_RS485EN_Pin GPIO_PIN_1
#define USART2_RS485EN_GPIO_Port GPIOA
#define SPI1_CSADC_Pin GPIO_PIN_4
#define SPI1_CSADC_GPIO_Port GPIOA
#define SPI1_CSDAC_Pin GPIO_PIN_4
#define SPI1_CSDAC_GPIO_Port GPIOC
#define SPI1_RESET_Pin GPIO_PIN_0
#define SPI1_RESET_GPIO_Port GPIOB
#define LED_Task_Pin GPIO_PIN_2
#define LED_Task_GPIO_Port GPIOB
#define USART3_RS485EN_Pin GPIO_PIN_12
#define USART3_RS485EN_GPIO_Port GPIOD
#define DoorOut1_Pin GPIO_PIN_6
#define DoorOut1_GPIO_Port GPIOC
#define DoorOut0_Pin GPIO_PIN_7
#define DoorOut0_GPIO_Port GPIOC
#define DoorIn0_Pin GPIO_PIN_8
#define DoorIn0_GPIO_Port GPIOC
#define DoorIn1_Pin GPIO_PIN_9
#define DoorIn1_GPIO_Port GPIOC
#define UART1_Reset_Pin GPIO_PIN_0
#define UART1_Reset_GPIO_Port GPIOD
#define UART1_EN_Pin GPIO_PIN_1
#define UART1_EN_GPIO_Port GPIOD
#define GPIO_DI0_Pin GPIO_PIN_3
#define GPIO_DI0_GPIO_Port GPIOD
#define GPIO_DI1_Pin GPIO_PIN_4
#define GPIO_DI1_GPIO_Port GPIOD
#define GPIO_DI2_Pin GPIO_PIN_5
#define GPIO_DI2_GPIO_Port GPIOD
#define GPIO_DI3_Pin GPIO_PIN_6
#define GPIO_DI3_GPIO_Port GPIOD
#define GPIO_DO0_Pin GPIO_PIN_8
#define GPIO_DO0_GPIO_Port GPIOB
#define GPIO_DO1_Pin GPIO_PIN_9
#define GPIO_DO1_GPIO_Port GPIOB
#define GPIO_DO2_Pin GPIO_PIN_0
#define GPIO_DO2_GPIO_Port GPIOE
#define GPIO_DO3_Pin GPIO_PIN_1
#define GPIO_DO3_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
