/*
 * Appl_Module.c
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 *
 */

/*******************************************************************************
*
* INCLUDE
*
******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "EKM_Buffer.h"
#include "EKM_Modbus.h"
#include "EKM_CMD.h"
#include "Appl_USBService.h"
#include "Appl_Module.h"
#include "Appl_Status.h"
#include "Appl_LTE.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/
#define APPL_MODULE_BUFFER_SIZE 255

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
EKM_MODBUS_t APPL_Module_MODBUS_RX;
uint8_t APPL_Module_RX_Buf_d[APPL_MODULE_BUFFER_SIZE];
uint8_t APPL_Module_EN_INTERVAL = 0;
uint8_t APPL_Module_MODBUS_Send_Semaphome = 0;

void APPL_Module_Init(void);
void APPL_Module_printf(const char *format, ...);
void APPL_Module_MODBUS_print(EKM_MODBUS_t *tmp);
void APPL_Module_LineProcess(void);
void APPL_Module_USB_MODBUS_print(EKM_MODBUS_t *tmp);

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/

/*******************************************************************************
* Function: APPL_Module_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Module_Init(void)
{
  HAL_GPIO_WritePin(USART2_EN_GPIO_Port, USART2_EN_Pin, GPIO_PIN_RESET);
  EKM_MODBUS_Setup(&APPL_Module_MODBUS_RX, APPL_Module_RX_Buf_d, APPL_MODULE_BUFFER_SIZE);
  EKM_MODBUS_DataClear(&APPL_Module_MODBUS_RX);
}

/*******************************************************************************
* Function: APPL_Module_printf
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Module_printf(const char *format, ...)
{
  // there was an error reading.
  uint8_t SerialBuffer[100];
  memset(SerialBuffer, 0, 100);
  va_list args;
  va_start(args, format);
  vsprintf((char *)SerialBuffer, format, args);
  va_end(args);
  HAL_UART_Transmit(&huart2, SerialBuffer, strlen((char *)SerialBuffer), 10);
}

/*******************************************************************************
* Function: APPL_Module_MODBUS_print
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Module_MODBUS_print(EKM_MODBUS_t *tmp)
{
  int i;
  uint8_t Ftemp[255];
  for (i = 0; i < tmp->len; i++)
  {
    Ftemp[i] = tmp->MODdata[i];
    APPL_USB_printf("%02x ", Ftemp[i]);
  }
  Ftemp[i++] = tmp->HCRC;
  Ftemp[i] = tmp->LCRC;
  APPL_Module_MODBUS_Send_Semaphome = 1;
  HAL_GPIO_WritePin(USART2_EN_GPIO_Port, USART2_EN_Pin, GPIO_PIN_SET);
  for (i = 0; i < tmp->len + 2; i++)
  {
    HAL_UART_Transmit(&huart2, &Ftemp[i], 1, 10);
    HAL_Delay(1);
  }
  HAL_GPIO_WritePin(USART2_EN_GPIO_Port, USART2_EN_Pin, GPIO_PIN_RESET);
  APPL_Module_MODBUS_Send_Semaphome = 0;
}

/*******************************************************************************
* Function: APPL_Module_USB_MODBUS_print
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Module_USB_MODBUS_print(EKM_MODBUS_t *tmp)
{
  APPL_USB_printf("ModBus %d : ", tmp->len);
  for (int i = 0; i < tmp->len; i++)
  {
    APPL_USB_printf("%02x ", tmp->MODdata[i]);
  }
  APPL_USB_printf("%04x\r\n", tmp->crc);
}

/*******************************************************************************
* Function: APPL_Module_LineProcess
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Module_LineProcess(void)
{
  if (APPL_Module_EN_INTERVAL == 1)
  {
    //APPL_Module_USB_MODBUS_print(&APPL_Module_MODBUS_RX);
    if (EKM_MODBUS_CRC_Decode(&APPL_Module_MODBUS_RX) == 1)
    {
      APPL_Module_USB_MODBUS_print(&APPL_Module_MODBUS_RX);
      APPL_LTE_MODBUS_MQTT_PUBLISH("WTM", &APPL_Module_MODBUS_RX);
      EKM_MODBUS_DataClear(&APPL_Module_MODBUS_RX);
    }
    if (APPL_Module_MODBUS_RX.len >= APPL_Module_MODBUS_RX.Maxsize)
    {
      EKM_MODBUS_DataClear(&APPL_Module_MODBUS_RX);
      APPL_USB_printf("Modbus test\r\n");
    }
    APPL_Module_EN_INTERVAL = 0;
  }
}

//END OF FILE