/*
 * APPL_DMA_OS.c
 *
 * Created: 19/4/2022 22:28:36
 *  Author: Eakawit_kor
 */ 

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sam.h"
#include "APPL_DMA_OS.h"
#include "APPL_DMA_IOCON.h"
#include "APPL_DMA_LTE.h"

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
void APPL_DMA_OS_Init(void);
void APPL_DMA_OS_LEDTask(uint8_t onff);
void APPL_DMA_OS_10ms(void);
void APPL_DMA_OS_100ms(void);
void APPL_DMA_OS_1s(void);
void APPL_DMA_OS_1m(void);
/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
uint8_t APPL_DMA_OS_Time_10ms = 0;
uint8_t APPL_DMA_OS_Time_100ms = 0;
uint8_t APPL_DMA_OS_Time_1s = 0;
uint8_t APPL_DMA_OS_Time_1m = 0;

/*******************************************************************************
* Function: APPL_DMA_OS_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_OS_Init(void)
{
	REG_PIOA_PER |= PIO_PER_P31; //pio enable PA11
	REG_PIOA_OER |= PIO_PER_P31; //set PA11 as output
	APPL_DMA_RS485_Init();
	APPL_DMA_RS485_printf("RS485 Init\r\n");
	APPL_DMA_IOCON_Init();
	APPL_DMA_RS485_printf("IOControl Init\r\n");
	APPL_DMA_LTE_Init();
	APPL_DMA_RS485_printf("LTE Init\r\n");
}


/*******************************************************************************
* Function: APPL_DMA_OS_LEDTask
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_OS_LEDTask(uint8_t onff){
	if(onff%2==0){
		REG_PIOA_SODR |= PIO_PER_P31; //set PA11 high (LED on)
		
	}else{
		REG_PIOA_CODR |= PIO_PER_P31; //set PA11 low (LED Off)
	}
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
void APPL_DMA_OS_10ms(void)
{
	APPL_DMA_OS_Time_10ms++;
	switch (APPL_DMA_OS_Time_10ms)
	{
		case 10:
		{	
			APPL_DMA_OS_LEDTask(APPL_DMA_OS_Time_100ms);
			APPL_DMA_OS_100ms();
			APPL_DMA_OS_Time_10ms = 0;
			break;
		}
		case 1:
		{
			APPL_DMA_IOCON_Get(&APPL_DMA_IOCON_d);
			APPL_DMA_IOCON_Battery(&APPL_DMA_IOCON_d);
			break;
		}
		case 2:
		{	
			APPL_DMA_IOCON_ADC_Get(&APPL_DMA_IOCON_d);
			break;
		}
		case 3:
		{
			APPL_DMA_IOCON_TempAndHumi(&APPL_DMA_IOCON_d);
			break;
		}
		case 4:
		{
			APPL_DMA_RS485_LineProcess();
			break;
		}
		case 7:
		{
			APPL_DMA_LTE_LineProcess();
			break;
		}
		case 6:
		{
			APPL_DMA_IOCON_Set(&APPL_DMA_IOCON_d);
			break;
		}
	}
	
	
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
void APPL_DMA_OS_100ms(void)
{
	APPL_DMA_OS_Time_100ms++;
	switch (APPL_DMA_OS_Time_100ms)
	{
		case 10:
		{	
			APPL_DMA_OS_1s();
			APPL_DMA_LTE_SIM7000G_Init();
			APPL_DMA_OS_Time_100ms = 0;
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
void APPL_DMA_OS_1s(void)
{
	APPL_DMA_OS_Time_1s++;
	switch (APPL_DMA_OS_Time_1s)
	{	
		case 60:
		{	
			APPL_DMA_OS_1m();
			APPL_DMA_OS_Time_1s = 0;
			break;
		}
		case 2:
		{
			break;
		}
	}
}

/*******************************************************************************
* Function: APPL_Task_1m
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_OS_1m(void)
{
	APPL_DMA_OS_Time_1m++;
	switch (APPL_DMA_OS_Time_1m)
	{
		case 5:
		{
			APPL_DMA_RS485_printf("Data 5m send %02x:%04X:%04X:%04X:%04X\r\n", 
				APPL_DMA_IOCON_d.DIO,
				APPL_DMA_IOCON_d.ANIN0,
				APPL_DMA_IOCON_d.ANIN1,
				APPL_DMA_IOCON_d.ANIN2,
				APPL_DMA_IOCON_d.ANIN3);
			APPL_DMA_LTE_MQTT_PUBLISH("IO", "%02x:%04X:%04X:%04X:%04X", APPL_DMA_IOCON_d.DIO,
			APPL_DMA_IOCON_d.ANIN0,
			APPL_DMA_IOCON_d.ANIN1,
			APPL_DMA_IOCON_d.ANIN2,
			APPL_DMA_IOCON_d.ANIN3);
			
			APPL_DMA_OS_Time_1m = 0;
			break;
		}
		case 2:
		{
			break;
		}
	}
}
