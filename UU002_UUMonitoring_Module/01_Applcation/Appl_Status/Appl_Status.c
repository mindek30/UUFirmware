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
#include "EKM_Buffer.h"
#include "EKM_Modbus.h"
#include "Appl_Status.h"
#include "Appl_USBService.h"
#include "main.h"
#include "gpio.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/
#define APPL_STATUSS_BUFFER 32

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
void APPL_Status_LTEGetMem(void){
    APPL_StatusS.IOT_ID = 1;
    APPL_StatusS.MQTT_IP0 = 203;
    APPL_StatusS.MQTT_IP1 = 151;
    APPL_StatusS.MQTT_IP2 = 79;
    APPL_StatusS.MQTT_IP3 = 149;
    APPL_StatusS.MQTT_PORT = 1883;
    strcpy(APPL_StatusS.MQTT_USER, "1111");
    strcpy(APPL_StatusS.MQTT_PASS, "1111");
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
void APPL_Status_ModbusGetMem(void){
    EKM_MODBUS_Setup(&APPL_StatusS_MODData, APPL_StatusS_MODSEND, APPL_STATUSS_BUFFER);
    EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData,0x58);
    EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData,0x03);
    EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData,0x00);
    EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData,0x00);
    EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData,0x00);
    EKM_MODBUS_Receiver_Set(&APPL_StatusS_MODData,0x0C);
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
    HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
}

//END OF FILE