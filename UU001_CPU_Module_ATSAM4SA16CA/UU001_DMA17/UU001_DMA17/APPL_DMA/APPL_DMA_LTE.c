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
#include "sam.h"
#include "EKM_Modbus.h"
#include "EKM_CMD.h"
#include "EKM_Buffer.h"
#include "APPL_DMA_RS485.h"
#include "APPL_DMA_IOCON.h"
#include "APPL_DMA_LTE.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/

#define APPL_DMA_LTE_BUFFER_MAX 128

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
int APPL_DMA_LTE_SIM7000G_Init_Stage = 0;
uint16_t APPL_DMA_LTE_SIM7000G_ErrorCount = 0;
uint16_t APPL_DMA_LTE_TimemerCount = 0;

int APPL_DMA_LTE_MQTT_ONLINE = 0;

uint8_t APPL_DMA_LTE_Buffer_d[APPL_DMA_LTE_BUFFER_MAX];
EKM_Buffer_t APPL_DMA_LTE_Buffer;

void APPL_DMA_LTE_Init(void);
void APPL_DMA_LTE_SIM7000G_Init(void);
void APPL_DMA_LTE_printf(const char *format, ...);
void APPL_DMA_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...);
void APPL_DMA_LTE_LineProcess(void);
void APPL_DMA_LTE_SIM7000G_Error_Handel(void);

EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_Sok;
EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_OK;
EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_Error;
EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_APP;
EKM_CMD_tCmdLineEntry APPL_DMA_LTE_CMD_PAPP;

int APPL_DMA_LTE_OK(int argc, char *argv[]);
int APPL_DMA_LTE_Error(int argc, char *argv[]);
int APPL_DMA_LTE_APP(int argc, char *argv[]);

/*******************************************************************************
* Function: APPL_LTE_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_LTE_Init(void)
{
	APPL_DMA_LTE_MQTT_ONLINE = 0;
	APPL_DMA_LTE_SIM7000G_ErrorCount=0;
	EKM_Buffer_Setup(&APPL_DMA_LTE_Buffer, APPL_DMA_LTE_BUFFER_MAX, APPL_DMA_LTE_Buffer_d);
	EKM_Buffer_Reset(&APPL_DMA_LTE_Buffer);
	
	APPL_DMA_LTE_CMD_OK.EKM_CMD_pcCmd = "ok";
	APPL_DMA_LTE_CMD_OK.EKM_CMD_pcHelp = "ok function";
	APPL_DMA_LTE_CMD_OK.EKM_CMD_pfnCmd = &APPL_DMA_LTE_OK;
	APPL_DMA_LTE_CMD_OK.next = &APPL_DMA_LTE_CMD_Sok;
	
	APPL_DMA_LTE_CMD_Sok.EKM_CMD_pcCmd = "OK";
	APPL_DMA_LTE_CMD_Sok.EKM_CMD_pcHelp = "ok function";
	APPL_DMA_LTE_CMD_Sok.EKM_CMD_pfnCmd = &APPL_DMA_LTE_OK;
	APPL_DMA_LTE_CMD_Sok.next = &APPL_DMA_LTE_CMD_Error;
	
	APPL_DMA_LTE_CMD_Error.EKM_CMD_pcCmd = "error";
	APPL_DMA_LTE_CMD_Error.EKM_CMD_pcHelp = "error function";
	APPL_DMA_LTE_CMD_Error.EKM_CMD_pfnCmd = &APPL_DMA_LTE_Error;
	APPL_DMA_LTE_CMD_Error.next = &APPL_DMA_LTE_CMD_PAPP;
	
	APPL_DMA_LTE_CMD_PAPP.EKM_CMD_pcCmd = "+APP";
	APPL_DMA_LTE_CMD_PAPP.EKM_CMD_pcHelp = "+APP Disable";
	APPL_DMA_LTE_CMD_PAPP.EKM_CMD_pfnCmd = &APPL_DMA_LTE_APP;
	APPL_DMA_LTE_CMD_PAPP.next = &APPL_DMA_LTE_CMD_APP;
	
	APPL_DMA_LTE_CMD_APP.EKM_CMD_pcCmd = "APP";
	APPL_DMA_LTE_CMD_APP.EKM_CMD_pcHelp = "APP Disable";
	APPL_DMA_LTE_CMD_APP.EKM_CMD_pfnCmd = &APPL_DMA_LTE_APP;
	APPL_DMA_LTE_CMD_APP.next = (void *)0;
	
	REG_PIOA_PER |= PIO_PER_P29|PIO_PER_P30; //LTE enable PA29
	REG_PIOA_OER |= PIO_PER_P29|PIO_PER_P30; //set PA29 as output
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
void APPL_DMA_LTE_printString(const uint8_t myString[]) {
	uint8_t i = 0;
	while (myString[i]) {
		while (!(REG_USART0_CSR & US_CSR_TXRDY));
		while (!(REG_USART0_CSR & US_CSR_TXEMPTY));
		REG_USART0_THR |=myString[i];
		i++;
	}
}

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
	uint8_t SerialBuffer[100];
	memset(SerialBuffer, 0, 100);
	va_list args;
	va_start(args, format);
	vsprintf((char *)SerialBuffer, format, args);
	va_end(args);
	APPL_DMA_LTE_printString(SerialBuffer);
}

/*******************************************************************************
* Function: APPL_LTE_MQTT_PUBLISH
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...)
{
	if (APPL_DMA_LTE_MQTT_ONLINE == 1)
	{
		// there was an error reading.
		uint8_t SerialBuffer[100];
		memset(SerialBuffer, 0, 100);
		va_list args;
		va_start(args, format);
		vsprintf((char *)SerialBuffer, format, args);
		va_end(args);
		APPL_DMA_LTE_printf("AT+SMPUB=\"UUCON/%d/%s\",%d,0,0\r\n", APPL_DMA_IOCON_d.IOT_ID, MQTT_Topic, strlen((char *)SerialBuffer));
		APPL_DMA_LTE_printf("%s\r\n", SerialBuffer);
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
	//APPL_DMA_RS485_printf("Stage of LTE = %d\n\r",APPL_DMA_LTE_SIM7000G_Init_Stage);
	switch(APPL_DMA_LTE_SIM7000G_Init_Stage){
		case 0:{
			REG_PIOA_CODR |= PIO_PER_P29; //set PA11 low (LED Off)
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			REG_PIOA_SODR |= PIO_PER_P29; //set PA11 high (LED on)
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 1:{
			// wait to enable 15 sec
			APPL_DMA_LTE_TimemerCount++;
			if(APPL_DMA_LTE_TimemerCount == 15){
				APPL_DMA_LTE_SIM7000G_Init_Stage++;
				APPL_DMA_LTE_TimemerCount=0;
			}
			break;
		}
		case 2:{
			// wait to enable 15 sec
			APPL_DMA_LTE_printf("AT\r\n");
			APPL_DMA_LTE_SIM7000G_ErrorCount++;
			break;
		}
		case 3:{
			APPL_DMA_LTE_printf("ATE0\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 5:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"CLIENTID\",\"id\"\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 7:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"KEEPTIME\",60\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 9:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"URL\",\"%d.%d.%d.%d\",\"%d\"\r\n",
				APPL_DMA_IOCON_d.MQTT_IP0,
				APPL_DMA_IOCON_d.MQTT_IP1,
				APPL_DMA_IOCON_d.MQTT_IP2,
				APPL_DMA_IOCON_d.MQTT_IP3, 
				APPL_DMA_IOCON_d.MQTT_PORT);
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 11:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"USERNAME\",\"%s\"\r\n", APPL_DMA_IOCON_d.MQTT_USER);
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 13:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"PASSWORD\",\"%s\"\r\n", APPL_DMA_IOCON_d.MQTT_PASS);
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 15:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"CLEANSS\",1\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 17:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"QOS\",0\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 19:{
			APPL_DMA_LTE_printf("AT+SMCONF=\"RETAIN\",0\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 21:{
			APPL_DMA_LTE_printf("AT+CMNB=3\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 23:{
			APPL_DMA_LTE_printf("AT+CNACT=1,\"cmnet\"\r\n");
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}
		case 25:{
			//APPL_DMA_LTE_MQTT_ONLINE = 1;
			APPL_DMA_LTE_SIM7000G_ErrorCount = 0;
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
			break;
		}		
		case 27:{
			// Wait for LTE EN
			break;
		}
		default:{
			APPL_DMA_LTE_SIM7000G_ErrorCount++;
			break;
		}
	}
	APPL_DMA_LTE_SIM7000G_Error_Handel();
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
	if(APPL_DMA_LTE_SIM7000G_ErrorCount>=30){
		APPL_DMA_RS485_printf("Error Handel event \n\r");
		//APPL_DMA_LTE_printf("Error Handel event \n\r");
		APPL_DMA_LTE_SIM7000G_Init_Stage = 0;
		APPL_DMA_LTE_SIM7000G_ErrorCount = 0;
	}
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
	//APPL_DMA_LTE_printf("Command = %s", APPL_DMA_RS48_Rx.buffer);
	if (EKM_CMD_RXPeek(APPL_DMA_LTE_Buffer.buffer, 0x0A, APPL_DMA_LTE_Buffer.write) > 0)
	{
		//APPL_DMA_RS485_printf("Command = %s", APPL_DMA_LTE_Buffer.buffer);
		if (EKM_CMD_LineProcess((char *)APPL_DMA_LTE_Buffer.buffer, &APPL_DMA_LTE_CMD_OK, ' ') == EKM_CMD_LINE_BAD_CMD)
		//APPL_DMA_LTE_printf("Command not found");
		EKM_Buffer_Reset(&APPL_DMA_LTE_Buffer);
	}
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
	APPL_DMA_LTE_SIM7000G_ErrorCount = 0;
	if(APPL_DMA_LTE_SIM7000G_Init_Stage<27){
		APPL_DMA_LTE_SIM7000G_Init_Stage++;
	}
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
	if(APPL_DMA_LTE_MQTT_ONLINE==1){
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
	if(argc>2){
		if(strstr(argv[2], "DEACTIVE") != NULL){
			APPL_DMA_LTE_SIM7000G_Init_Stage = 24;
		}else if(strstr(argv[2], "ACTIVE") != NULL){
			APPL_DMA_LTE_MQTT_ONLINE = 1;
			APPL_DMA_LTE_SIM7000G_Init_Stage++;
		}
	}
	return 0;
}