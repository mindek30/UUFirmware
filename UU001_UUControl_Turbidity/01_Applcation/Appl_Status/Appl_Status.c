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
#include "main.h"
#include "Appl_USBService.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/
#define APPL_STATUSS_BUFFER 64

#define SEN_HDC2021_ADDR 0x80

#define MEM_M24512_ADDR 0xA0
#define MEM_M24512_PAGE_ADDR 0xB0

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
APPL_STATUS_t APPL_StatusS;
uint8_t APPL_StatusS_MODSEND[APPL_STATUSS_BUFFER];

void APPL_Status_Init(void);
void APPL_Status_LED_Toogle(void);

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
EKM_MODBUS_t APPL_StatusS_MODData;
void APPL_Status_LTEGetMem(void);
void APPL_Status_ModbusGetMem(void);
/*******************************************************************************
* Function: APPL_Status_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Status_Init(void)
{

  APPL_Status_LTEGetMem();
  APPL_Status_ModbusGetMem();
  APPL_StatusS.MODBUS_SEND = &APPL_StatusS_MODData;
}

/*******************************************************************************
* Function: APPL_Status_LTEGetMem
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Status_LTEGetMem(void)
{
  APPL_StatusS.IOT_ID = 1;
  APPL_StatusS.MQTT_IP0 = 203;
  APPL_StatusS.MQTT_IP1 = 151;
  APPL_StatusS.MQTT_IP2 = 79;
  APPL_StatusS.MQTT_IP3 = 149;
  APPL_StatusS.MQTT_PORT = 1883;
  strcpy(APPL_StatusS.MQTT_USER, "1111");
  strcpy(APPL_StatusS.MQTT_PASS, "1111");
  APPL_StatusS.Temperature = 0;
  APPL_StatusS.Humidity = 0;
}

/*******************************************************************************
* Function: APPL_Status_ModbusGetMem
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Status_ModbusGetMem(void)
{
  Bit16ToBit8_t dat,datsize;
  dat.REG = 2999;
  datsize.REG = 34; 
  EKM_MODBUS_Setup(&APPL_StatusS_MODData, APPL_StatusS_MODSEND, APPL_STATUSS_BUFFER);
  EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData, 0x01);
  EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData, 0x03);
  EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData, dat.HREG);
  EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData, dat.LREG);
  EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData, datsize.HREG);
  EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData, datsize.LREG);
  EKM_MODBUS_CRC_Encode(&APPL_StatusS_MODData);
}

/*******************************************************************************
* Function: APPL_Status_LED_Toogle
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_Status_LED_Toogle(void)
{
  HAL_GPIO_TogglePin(LED_Task_GPIO_Port, LED_Task_Pin);
}

/*******************************************************************************
* Function: APPL_Status_Battery
*
* Parameters:      -
* Returned value:  -
*
* Description: using for set Battery level
*
* Calling:
******************************************************************************/
void APPL_Status_Battery(APPL_STATUS_t *pdata)
{
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 5);
  pdata->Batt = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
}

/*******************************************************************************
* Function: APPL_Status_TempAndHumi
*
* Parameters:      -
* Returned value:  -
*
* Description: Read Temperature form HDC2021
*
* Calling:
******************************************************************************/
void APPL_Status_TempAndHumi(APPL_STATUS_t *pdata)
{
  uint8_t temp[4];
  // Record
  temp[0] = 0x0E;
  temp[1] = 0x70;
  HAL_I2C_Master_Transmit(&hi2c1, SEN_HDC2021_ADDR, temp, 2, 10);

  temp[0] = 0x00;
  temp[1] = 0x00;
  //HAL_I2C_Master_Transmit(&hi2c1, SEN_HDC2021_ADDR, temp, 0, 10);
  HAL_I2C_Master_Receive(&hi2c1, SEN_HDC2021_ADDR, temp, 4, 10);
  //APPL_USB_printf("Temp = %04x,%04x\r\n", temp[0], temp[1]);
  //APPL_USB_printf("Humi = %04x,%04x\r\n", temp[2], temp[3]);

  pdata->HumidH = temp[0];
  pdata->HumidL = temp[1];
  pdata->TemperH = temp[2];
  pdata->TemperL = temp[3];
}

//END OF FILE