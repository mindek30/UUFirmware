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
#define APPL_DMA_LTE_INIT_STAGE_MAX 12
#define APPL_DMA_LTE_TIMEOUT_MAX 100

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
uint16_t APPL_DMA_LTE_SIM7000G_Init_Stage = 0;
uint16_t APPL_DMA_LTE_TimemerCount = 0;
uint16_t APPL_DMA_LTE_TimeoutCount = 0;

int APPL_DMA_LTE_RES_RUN = 0;
int APPL_DMA_LTE_MQTT_ONLINE = 0;
int APPL_DMA_LTE_CONNECTWAIT = 0;
int APPL_DMA_LTE_ERROR_COUNT = 0;

uint8_t APPL_DMA_LTE_Buffer_d[APPL_DMA_LTE_BUFFER_MAX];
EKM_Buffer_t APPL_DMA_LTE_Buffer;

void APPL_DMA_LTE_Init(void);
void APPL_DMA_LTE_SIM7000G_Init(void);
void APPL_DMA_LTE_printf(const char *format, ...);
void APPL_DMA_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...);
void APPL_DMA_LTE_EN(uint8_t onff);
void APPL_DMA_LTE_LineProcess(void);


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
void APPL_DMA_LTE_Init(void)
{
	APPL_DMA_LTE_MQTT_ONLINE = 0;
	EKM_Buffer_Setup(&APPL_DMA_LTE_Buffer, APPL_DMA_LTE_BUFFER_MAX, APPL_DMA_LTE_Buffer_d);
	EKM_Buffer_Reset(&APPL_DMA_LTE_Buffer);
	
	REG_PIOA_PER |= PIO_PER_P29|PIO_PER_P30; //LTE enable PA29
	REG_PIOA_OER |= PIO_PER_P29|PIO_PER_P30; //set PA29 as output
	
	APPL_DMA_LTE_EN(1);
}

/*******************************************************************************
* Function: APPL_DMA_LTE_EN
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_LTE_EN(uint8_t onff){
	if(onff%2==1){
		REG_PIOA_SODR |= PIO_PER_P29; //set PA11 high (LED on)
		}else{
		REG_PIOA_CODR |= PIO_PER_P29; //set PA11 low (LED Off)
	}
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
void APPL_DMA_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...)
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
	if(APPL_DMA_LTE_SIM7000G_Init_Stage>APPL_DMA_LTE_INIT_STAGE_MAX){
		if(APPL_DMA_LTE_ERROR_COUNT>3){
			APPL_DMA_RS485_printf("Reset LTE \r\n");
			APPL_DMA_LTE_printf("AT+CPOWD=0\r\n");
			APPL_DMA_LTE_TimemerCount = 0;
			APPL_DMA_LTE_SIM7000G_Init_Stage = 0;
			APPL_DMA_LTE_MQTT_ONLINE = 0;
			APPL_DMA_LTE_CONNECTWAIT = 0;
			APPL_DMA_LTE_TimeoutCount = 0;
			APPL_DMA_LTE_ERROR_COUNT = 0;
		}else if ((APPL_DMA_LTE_TimeoutCount>0)&&(APPL_DMA_LTE_MQTT_ONLINE==0)){
		    APPL_DMA_RS485_printf("Time Out. %d \r\n",APPL_DMA_LTE_TimeoutCount++);
			if(APPL_DMA_LTE_TimeoutCount>APPL_DMA_LTE_TIMEOUT_MAX){
				APPL_DMA_LTE_ERROR_COUNT = 4;
				APPL_DMA_RS485_printf(" Time Out at %d Sec\r\n",APPL_DMA_LTE_TimeoutCount);
			}
		}
		return;
	}else if(APPL_DMA_LTE_TimemerCount==1){
		APPL_DMA_LTE_TimemerCount++;
		APPL_DMA_RS485_printf("Time Count %d \r\n",APPL_DMA_LTE_TimemerCount);
		APPL_DMA_LTE_EN(0);
	}else if(APPL_DMA_LTE_TimemerCount==2){
		APPL_DMA_LTE_TimemerCount++;
		APPL_DMA_RS485_printf("Time Count %d \r\n",APPL_DMA_LTE_TimemerCount);
		APPL_DMA_LTE_EN(1);
	}else if(APPL_DMA_LTE_TimemerCount==3){
		APPL_DMA_LTE_TimemerCount++;
		APPL_DMA_RS485_printf("Time Count %d \r\n",APPL_DMA_LTE_TimemerCount);
		APPL_DMA_LTE_EN(0);
	}else if(APPL_DMA_LTE_TimemerCount==15){
		APPL_DMA_LTE_SIM7000G_Init_Stage++;
		switch(APPL_DMA_LTE_SIM7000G_Init_Stage){
			case 0: {
				APPL_DMA_LTE_printf("ATE0\r\n");
				break;
			} 
			case 1: {
				APPL_DMA_LTE_printf("ATE0\r\n");
				break;
    		}
    		case 2: {
	    		APPL_DMA_LTE_printf("ATE0\r\n");
	    		break;
    		}
			case 3: {
				APPL_DMA_LTE_printf("AT+SMCONF=\"CLIENTID\",\"id\"\r\n");
				break;
   			} 
			case 4: {
   				APPL_DMA_LTE_printf("AT+SMCONF=\"KEEPTIME\",60\r\n");
   				break;
    		} 
			case 5: {
    			APPL_DMA_LTE_printf("AT+SMCONF=\"URL\",\"%d.%d.%d.%d\",\"%d\"\r\n",
					APPL_DMA_IOCON_d.MQTT_IP0,
					APPL_DMA_IOCON_d.MQTT_IP1,
					APPL_DMA_IOCON_d.MQTT_IP2,
					APPL_DMA_IOCON_d.MQTT_IP3, APPL_DMA_IOCON_d.MQTT_PORT);
					break;
    		} 
			case 6: {
    			APPL_DMA_LTE_printf("AT+SMCONF=\"USERNAME\",\"%s\"\r\n", APPL_DMA_IOCON_d.MQTT_USER);
    			break;
    		} 
			case 7: {
    			APPL_DMA_LTE_printf("AT+SMCONF=\"PASSWORD\",\"%s\"\r\n", APPL_DMA_IOCON_d.MQTT_USER);
    			break;
    		} 
			case 8: {
    			APPL_DMA_LTE_printf("AT+SMCONF=\"CLEANSS\",1\r\n");
    			break;
    		} 
			case 9: {
    			APPL_DMA_LTE_printf("AT+SMCONF=\"QOS\",0\r\n");
    			break;
    		} 
			case 10: {
    			APPL_DMA_LTE_printf("AT+SMCONF=\"RETAIN\",0\r\n");
    			break;
    		} 
			case 11: {
    			APPL_DMA_LTE_printf("AT+CMNB=3\r\n");
    			break;
    		} 
			case 12: {
    			EKM_Buffer_Reset(&APPL_DMA_LTE_Buffer);
				APPL_DMA_LTE_printf("AT+CNACT=1,\"cmnet\"\r\n");
				APPL_DMA_LTE_TimeoutCount = 1;
				break;
			}
		}
		APPL_DMA_RS485_printf("Stage No. %d \r\n",APPL_DMA_LTE_SIM7000G_Init_Stage);
	}else{
		APPL_DMA_LTE_TimemerCount++;
		APPL_DMA_RS485_printf("Time Count %d \r\n",APPL_DMA_LTE_TimemerCount);
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
	if(APPL_DMA_LTE_RES_RUN == 1){
		APPL_DMA_RS485_printf("LTE=%s--\r\n",APPL_DMA_LTE_Buffer.buffer);
		if(APPL_DMA_LTE_SIM7000G_Init_Stage>APPL_DMA_LTE_INIT_STAGE_MAX){
			if (strstr((char *)APPL_DMA_LTE_Buffer.buffer, "OK") != NULL)
			{
				if (APPL_DMA_LTE_CONNECTWAIT == 1)
				{
					APPL_DMA_LTE_MQTT_ONLINE = 1;
					APPL_DMA_LTE_CONNECTWAIT = 0;
					APPL_DMA_LTE_TimeoutCount = 0;
				}
			}
			else if (strstr((char *)APPL_DMA_LTE_Buffer.buffer, "+APP PDP: DEACTIVE") != NULL)
			{
				APPL_DMA_LTE_TimeoutCount = 1;
				APPL_DMA_LTE_ERROR_COUNT++;
				APPL_DMA_LTE_MQTT_ONLINE = 0;
				APPL_DMA_LTE_printf("AT+CNACT=1,\"cmnet\"\r\n");
			}
			else if (strstr((char *)APPL_DMA_LTE_Buffer.buffer, "+APP PDP: ACTIVE") != NULL)
			{
				APPL_DMA_LTE_printf("AT+SMCONN\r\n");
				APPL_DMA_LTE_TimeoutCount = 1;
				APPL_DMA_LTE_CONNECTWAIT = 1;
			}
			else if (strstr((char *)APPL_DMA_LTE_Buffer.buffer, "ERROR") != NULL)
			{
				APPL_DMA_LTE_ERROR_COUNT++;
			}
		}
		EKM_Buffer_Reset(&APPL_DMA_LTE_Buffer);
		APPL_DMA_LTE_RES_RUN = 0;
	}
}
