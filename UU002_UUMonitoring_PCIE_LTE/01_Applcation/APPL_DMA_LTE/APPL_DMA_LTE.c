/*
 * APPL_DMA_LTE.c
 *
 * Created: 22/4/2022 1:46:27
 *  Author: Eakawit_kor
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "usart.h"
#include "EKM_CMD.h"
#include "EKM_Buffer.h"
#include "APPL_DMA_LTE.h"
#include "Appl_Data.h"
#include "Appl_USBService.h"

/*******************************************************************************
 *
 * DEFINE
 *
 ******************************************************************************/

#define APPL_DMA_LTE_BUFFER_MAX 128

//! Structure for an entry in the command list table.

//*****************************************************************************

// Prototypes for the APIs.

//*****************************************************************************

/*******************************************************************************
 *
 * PRIVATE VARIABLE
 *
 ******************************************************************************/

uint16_t APPL_DMA_LTE_SIM7000G_ErrorCount = 0;
uint16_t APPL_DMA_LTE_TimemerCount = 0;

int APPL_DMA_LTE_MQTT_ONLINE = 0;

uint8_t APPL_DMA_LTE_Init_Stage = 0;
APPL_DMA_LTE_t APPL_DMA_LTE_Step_Init[15];

uint8_t APPL_DMA_LTE_Buffer_d[APPL_DMA_LTE_BUFFER_MAX];
EKM_Buffer_t APPL_DMA_LTE_Buffer;

void APPL_DMA_LTE_Init(void);
void APPL_DMA_LTE_SIM7000G_Init(void);
void APPL_DMA_LTE_printf(const char *format, ...);
void APPL_DMA_LTE_MQTT_PUBLISH_Raw(const char *MQTT_Topic, uint8_t *data, uint8_t len);
void APPL_DMA_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...);
void APPL_DMA_LTE_LineProcess(void);
void APPL_DMA_LTE_SIM7000G_Error_Handel(void);

EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_OK;
EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_PSUTTZ;
EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_Error;
EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_APP;

int APPL_DMA_LTE_OK(int argc, char *argv[]);
int APPL_DMA_LTE_PSUTTZ(int argc, char *argv[]);
int APPL_DMA_LTE_Error(int argc, char *argv[]);
int APPL_DMA_LTE_APP(int argc, char *argv[]);

/*******************************************************************************
 * Function: APPL_DMA_LTE_Init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DMA_LTE_Init(void)
{
	APPL_DMA_LTE_MQTT_ONLINE = 0;
	APPL_DMA_LTE_SIM7000G_ErrorCount = 0;
	EKM_Buffer_Setup(&APPL_DMA_LTE_Buffer, APPL_DMA_LTE_BUFFER_MAX, APPL_DMA_LTE_Buffer_d);
	EKM_Buffer_Reset(&APPL_DMA_LTE_Buffer);

	APPL_DMA_LTE_CMD_OK.EKM_CMD_pcCmd = "OK";
	APPL_DMA_LTE_CMD_OK.EKM_CMD_pcHelp = "ok function";
	APPL_DMA_LTE_CMD_OK.EKM_CMD_pfnCmd = &APPL_DMA_LTE_OK;
	APPL_DMA_LTE_CMD_OK.next = &APPL_DMA_LTE_CMD_PSUTTZ;

	APPL_DMA_LTE_CMD_PSUTTZ.EKM_CMD_pcCmd = "*PSUTTZ:";
	APPL_DMA_LTE_CMD_PSUTTZ.EKM_CMD_pcHelp = "Turn On Init";
	APPL_DMA_LTE_CMD_PSUTTZ.EKM_CMD_pfnCmd = &APPL_DMA_LTE_PSUTTZ;
	APPL_DMA_LTE_CMD_PSUTTZ.next = &APPL_DMA_LTE_CMD_Error;

	APPL_DMA_LTE_CMD_Error.EKM_CMD_pcCmd = "ERROR";
	APPL_DMA_LTE_CMD_Error.EKM_CMD_pcHelp = "ERROR";
	APPL_DMA_LTE_CMD_Error.EKM_CMD_pfnCmd = &APPL_DMA_LTE_Error;
	APPL_DMA_LTE_CMD_Error.next = (void *)0;
	//*PSUTTZ:
	APPL_data.deviceID = 0;

	APPL_DMA_LTE_Step_Init[0].Command = "ATE0\r\n";
	APPL_DMA_LTE_Step_Init[1].Command = "AT+SMCONF=\"CLIENTID\",\"id\"\r\n";
	APPL_DMA_LTE_Step_Init[2].Command = "AT+SMCONF=\"KEEPTIME\",60\r\n";
	APPL_DMA_LTE_Step_Init[3].Command = "AT+SMCONF=\"URL\",\"203.151.79.149\",\"1883\"\r\n";
	APPL_DMA_LTE_Step_Init[4].Command = "AT+SMCONF=\"USERNAME\",\"1111\"\r\n";
	APPL_DMA_LTE_Step_Init[5].Command = "AT+SMCONF=\"PASSWORD\",\"1111\"\r\n";
	APPL_DMA_LTE_Step_Init[6].Command = "AT+SMCONF=\"CLEANSS\",1\r\n";
	APPL_DMA_LTE_Step_Init[7].Command = "AT+SMCONF=\"QOS\",0\r\n";
	APPL_DMA_LTE_Step_Init[8].Command = "AT+SMCONF=\"RETAIN\",0\r\n";
	APPL_DMA_LTE_Step_Init[9].Command = "AT+CMNB=3\r\n";
	APPL_DMA_LTE_Step_Init[10].Command = "AT+CNACT=1,\"cmnet\"\r\n";

	APPL_DMA_LTE_MQTT_ONLINE = 1;

	for (int i = 0; i < 10; i++)
	{
		APPL_DMA_LTE_Step_Init[i].Return_Stage = 1;
	}
	APPL_DMA_LTE_Step_Init[0].Return_Stage = 2;
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_printString
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
/*void APPL_DMA_LTE_printString(const uint8_t myString[]) {
	uint8_t i = 0;
	while (myString[i]) {
		while (!(REG_USART0_CSR & US_CSR_TXRDY));
		while (!(REG_USART0_CSR & US_CSR_TXEMPTY));
		REG_USART0_THR |=myString[i];
		i++;
	}
}
*/
/*******************************************************************************
 * Function: APPL_DMA_LTE_printf
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DMA_LTE_printf(const char *format, ...)
{
	// there was an error reading.
	uint8_t SerialBuffer[255];
	memset(SerialBuffer, 0, 255);
	va_list args;
	va_start(args, format);
	vsprintf((char *)SerialBuffer, format, args);
	va_end(args);
	HAL_UART_Transmit(&huart1, SerialBuffer, strlen((char *)SerialBuffer), 10);
	// APPL_DMA_LTE_printString(SerialBuffer);
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_MQTT_PUBLISH
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DMA_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...)
{
	if (APPL_DMA_LTE_MQTT_ONLINE == 1)
	{
		// there was an error reading.
		uint8_t SerialBuffer[255];
		memset(SerialBuffer, 0, 255);
		va_list args;
		va_start(args, format);
		vsprintf((char *)SerialBuffer, format, args);
		va_end(args);
		APPL_DMA_LTE_printf("AT+SMPUB=\"UUMON/%d/%s\",%d,0,0\r\n", APPL_data.deviceID, MQTT_Topic, strlen((char *)SerialBuffer));
		APPL_DMA_LTE_printf("%s\r\n", SerialBuffer);
	}
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_MQTT_PUBLISH_Raw
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DMA_LTE_MQTT_PUBLISH_Raw(const char *MQTT_Topic, uint8_t *data, uint8_t len)
{
	if (APPL_DMA_LTE_MQTT_ONLINE == 1)
	{
		// there was an error reading.
		APPL_DMA_LTE_printf("AT+SMPUB=\"UUMON/%d/%s\",%d,0,0\r\n", 
							APPL_data.deviceID,
							MQTT_Topic,
							(3 * len));
		for (int i = 0; i < len; i++)
		{
			APPL_DMA_LTE_printf("%02x ", data[i]);
		}
		APPL_DMA_LTE_printf("\r\n");
	}
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_SIM7000G_Init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DMA_LTE_SIM7000G_Init(void)
{
	if (APPL_DMA_LTE_Step_Init[APPL_DMA_LTE_Init_Stage].Return_Stage == 0)
	{
		APPL_DMA_LTE_Init_Stage++;
	}
	if (APPL_DMA_LTE_Init_Stage < 10)
	{
		APPL_DMA_LTE_printf("%s", APPL_DMA_LTE_Step_Init[APPL_DMA_LTE_Init_Stage].Command);
	//	APPL_USB_printf("%s", APPL_DMA_LTE_Step_Init[APPL_DMA_LTE_Init_Stage].Command);
	}
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_SIM7000G_Error_Handel
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DMA_LTE_SIM7000G_Error_Handel(void)
{
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_LineProcess
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DMA_LTE_LineProcess(void)
{
	uint8_t commandread[255];
	// APPL_DMA_LTE_printf("Command = %s", APPL_DMA_RS48_Rx.buffer);
	while (EKM_Buffer_readline(&APPL_DMA_LTE_Buffer, commandread, "\r\n"))
	{
		APPL_USB_printf("Command = %s--", commandread);
		if (EKM_CMD_LineProcess((char *)commandread, &APPL_DMA_LTE_CMD_OK, ' ') == EKM_CMD_LINE_BAD_CMD){
			// APPL_DMA_LTE_printf("Command not found");
		}
	}
//	EKM_Buffer_Reset(&APPL_DMA_LTE_Buffer);
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_OK
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_DMA_LTE_OK(int argc, char *argv[])
{
	APPL_DMA_LTE_Step_Init[APPL_DMA_LTE_Init_Stage].Return_Stage--;
	APPL_USB_printf("APPL_DMA_LTE_Init_Stage = %d", APPL_DMA_LTE_Init_Stage);
	return 0;
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_Error
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_DMA_LTE_Error(int argc, char *argv[])
{
	APPL_DMA_LTE_printf("AT+CPOWD=0\r\n");
	if (APPL_DMA_LTE_MQTT_ONLINE == 1)
	{
		APPL_DMA_LTE_SIM7000G_ErrorCount++;
	}
	return 0;
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_APP
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description: APP PDP: DEACTIVE
 *
 * Calling:
 ******************************************************************************/
int APPL_DMA_LTE_APP(int argc, char *argv[])
{
	if (argc > 2)
	{
		if (strstr(argv[2], "DEACTIVE") != NULL)
		{

		}
		else if (strstr(argv[2], "ACTIVE") != NULL)
		{
			
		}
	}
	return 0;
}

/*******************************************************************************
 * Function: APPL_DMA_LTE_PSUTTZ
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description: *PSUTTZ: 22/06/12,14:45:24","+28"
 *
 * Calling:
 ******************************************************************************/
int APPL_DMA_LTE_PSUTTZ(int argc, char *argv[])
{
	
	return 0;
}