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
#include "main.h"
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
  HAL_Delay(1000);

  APPL_USB_Init();
  APPL_USB_printf("USB Init\r\n");

  APPL_IOCON_Init();
  APPL_USB_printf("I/O Init\r\n");

  APPL_Module_Init();
  APPL_USB_printf("Module Init\r\n");

  APPL_Status_Init();
  APPL_USB_printf("Status Init\r\n");

  APPL_LTE_Init();
  APPL_USB_printf("LTE Init\r\n");

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
    break;
  }
  case 1:
  {
    APPL_IOCON_Set(&APPL_IOCON_s);
    break;
  }
  case 3:
  {
    APPL_IOCON_Get(&APPL_IOCON_s);
    break;
  }
  case 4:
  {
    APPL_Status_Battery(&APPL_StatusS);
    break;
  }
  case 5:
  {
    APPL_IOCON_ADC_Get(&APPL_IOCON_s);
    break;
  }
  case 7:
  {
    APPL_USB_LineProcess();
    break;
  }
  case 8:
  {
    APPL_Module_LineProcess();
    break;
  }
  case 9:
  {
    APPL_IOCON_LineProcess();
    break;
  }
  }
  APPL_LTE_LineProcess();
}

/*******************************************************************************
* Function: APPL_Task_100ms
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
    APPL_Task_1s();
    //APPL_IOCON_s.OUT0 = ~APPL_IOCON_s.OUT0;
    APPL_Task_Time_100ms = 0;
    break;
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
  case 2:
  {
    APPL_IOCON_MODBUS_print(APPL_StatusS.MODBUS_SEND);
    break;
  }
  case 3:
  {
    APPL_Module_MODBUS_print(APPL_StatusS.MODBUS_SEND);
    break;
  }
  case 9:
  {
    APPL_LTE_MQTT_PUBLISH("ST", "%04x:%04X:%04X", APPL_StatusS.Batt, APPL_StatusS.Temperature, APPL_StatusS.Humidity);
    break;
  }
  case 12:
  {
    APPL_LTE_MQTT_PUBLISH("IO", "%02x:%04X:%04X:%04X:%04X", APPL_IOCON_s.DIO, APPL_IOCON_s.ANIN0, APPL_IOCON_s.ANIN1, APPL_IOCON_s.ANIN2, APPL_IOCON_s.ANIN3);
    break;
  }
  case 15:
  {
    APPL_Task_Time_1s = 0;
    break;
  }
  }
}

//END OF FILE