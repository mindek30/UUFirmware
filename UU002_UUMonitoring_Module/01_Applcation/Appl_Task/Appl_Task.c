/*
 * Appl_Task.c
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
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "EKM_Buffer.h"
#include "EKM_Modbus.h"
#include "Appl_LTE.h"
#include "Appl_Module.h"
#include "Appl_Status.h"
#include "Appl_Task.h"
#include "Appl_USBService.h"

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
void APPL_Task_Init(void);
void APPL_Task_10ms(void);
void APPL_Task_100ms(void);
void APPL_Task_1s(void);
/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
uint8_t APPL_Task_Time_10ms = 0;
uint8_t APPL_Task_Time_100ms = 0;
uint8_t APPL_Task_Time_1s = 0;

/*******************************************************************************
* Function: APPL_Task_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Task_Init(void)
{
  APPL_USB_Init();
  APPL_USB_printf("USB On\r\n");
  APPL_Status_Init();
  APPL_USB_printf("Status On\r\n");
  APPL_Module_Init();
  APPL_USB_printf("LTE Start\r\n");
  APPL_LTE_Init();
  APPL_USB_printf("LTE On\r\n");
  APPL_USB_printf("All System Green\r\n");
}

/*******************************************************************************
* Function: APPL_Task_10ms
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Task_10ms(void)
{
  switch (APPL_Task_Time_10ms++)
  {
  case 10:
  {
    APPL_Task_100ms();
    APPL_Status_LED_Toogle();
    APPL_Task_Time_10ms = 0;
  }
  case 6:
  {
    APPL_USB_LineProcess();
  }
  }
  APPL_Module_LineProcess();
  APPL_LTE_LineProcess();
}

/*******************************************************************************
* Function: APPL_Task_10ms
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Task_100ms(void)
{
  APPL_Task_Time_100ms++;
  switch (APPL_Task_Time_100ms)
  {
  case 10:
  {
    APPL_Task_Time_100ms = 0;
    APPL_Task_1s();
  }
  }
}

/*******************************************************************************
* Function: APPL_Task_1s
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Task_1s(void)
{
  APPL_Task_Time_1s++;
  switch (APPL_Task_Time_1s)
  {
  case 7:
  {
    //APPL_Module_USB_MODBUS_print(APPL_StatusS.MODBUS_SEND);
    APPL_Module_MODBUS_print(APPL_StatusS.MODBUS_SEND);

  }
  case 10:
  {
    //APPL_LTE_MQTT_PUBLISH("WTM", "%d,%d", 177696, 17194);
    APPL_Task_Time_1s = 0;
  }
  }
}
//END OF FILE