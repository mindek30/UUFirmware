/*
 * APPL_DMA_RS485.c
 *
 * Created: 19/4/2022 22:06:06
 *  Author: Eakawit_kor
 */ 

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sam.h"
#include "APPL_DMA_RS485.h"
#include "APPL_DMA_IOCON.h"
#include "APPL_DMA_LTE.h"

#define APPL_DMA_RS48_BUF_Size 50

EKM_Buffer_t APPL_DMA_RS48_Rx;
uint8_t APPL_DMA_RS48_Data[APPL_DMA_RS48_BUF_Size];

EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_IDN;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_HELP;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_Status;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_GPIOW;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_ANALOG;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_SystemReset;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_CTMB;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_Test;
EKM_CMD_tCmdLineEntry APPL_DMA_RS485_CMD_LTE_Test;

void APPL_DMA_RS485_Init(void);
void APPL_DMA_RS485_printByte(uint8_t data);
void APPL_DMA_RS485_printString(const uint8_t myString[]);
void APPL_DMA_RS485_printf(const char *format, ...);
void APPL_DMA_RS485_LTE_EN(uint8_t onff);
void APPL_DMA_RS485_LineProcess(void);

int APPL_DMA_RS485_IDN(int argc, char *argv[]);
int APPL_DMA_RS485_HELP(int argc, char *argv[]);
int APPL_DMA_RS485_Status(int argc, char *argv[]);
int APPL_DMA_RS485_GPIOW(int argc, char *argv[]);
int APPL_DMA_RS485_ANALOG(int argc, char *argv[]);
int APPL_DMA_RS485_SystemReset(int argc, char *argv[]);
int APPL_DMA_RS485_Test(int argc, char *argv[]);
int APPL_DMA_RS485_CTMB(int argc, char *argv[]);
int APPL_DMA_RS485_LTE_TEST(int argc, char *argv[]);

/*******************************************************************************
* Function: APPL_DMA_RS485_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_RS485_Init(void)
{

	EKM_Buffer_Setup(&APPL_DMA_RS48_Rx, APPL_DMA_RS48_BUF_Size, APPL_DMA_RS48_Data);
	
	    APPL_DMA_RS485_CMD_IDN.EKM_CMD_pcCmd = "IDN?";
	    APPL_DMA_RS485_CMD_IDN.EKM_CMD_pcHelp = "defined in IEEE Std 488.2-1992";
	    APPL_DMA_RS485_CMD_IDN.EKM_CMD_pfnCmd = &APPL_DMA_RS485_IDN;
	    APPL_DMA_RS485_CMD_IDN.next = &APPL_DMA_RS485_CMD_HELP;

	    APPL_DMA_RS485_CMD_HELP.EKM_CMD_pcCmd = "help";
	    APPL_DMA_RS485_CMD_HELP.EKM_CMD_pcHelp = "Help command";
	    APPL_DMA_RS485_CMD_HELP.EKM_CMD_pfnCmd = &APPL_DMA_RS485_HELP;
	    APPL_DMA_RS485_CMD_HELP.next = &APPL_DMA_RS485_CMD_SystemReset;

	    APPL_DMA_RS485_CMD_SystemReset.EKM_CMD_pcCmd = "init";
	    APPL_DMA_RS485_CMD_SystemReset.EKM_CMD_pcHelp = "System Control";
	    APPL_DMA_RS485_CMD_SystemReset.EKM_CMD_pfnCmd = &APPL_DMA_RS485_SystemReset;
	    APPL_DMA_RS485_CMD_SystemReset.next = &APPL_DMA_RS485_CMD_Status;

	    APPL_DMA_RS485_CMD_Status.EKM_CMD_pcCmd = "status";
	    APPL_DMA_RS485_CMD_Status.EKM_CMD_pcHelp = "status of device";
	    APPL_DMA_RS485_CMD_Status.EKM_CMD_pfnCmd = &APPL_DMA_RS485_Status;
	    APPL_DMA_RS485_CMD_Status.next = &APPL_DMA_RS485_CMD_GPIOW;

	    APPL_DMA_RS485_CMD_GPIOW.EKM_CMD_pcCmd = "gpiow";
	    APPL_DMA_RS485_CMD_GPIOW.EKM_CMD_pcHelp = "On off relay";
	    APPL_DMA_RS485_CMD_GPIOW.EKM_CMD_pfnCmd = &APPL_DMA_RS485_GPIOW;
	    APPL_DMA_RS485_CMD_GPIOW.next = &APPL_DMA_RS485_CMD_ANALOG;

	    APPL_DMA_RS485_CMD_ANALOG.EKM_CMD_pcCmd = "analog";
	    APPL_DMA_RS485_CMD_ANALOG.EKM_CMD_pcHelp = "Read Analog Data";
	    APPL_DMA_RS485_CMD_ANALOG.EKM_CMD_pfnCmd = &APPL_DMA_RS485_ANALOG;
	    APPL_DMA_RS485_CMD_ANALOG.next = &APPL_DMA_RS485_CMD_CTMB;

	    APPL_DMA_RS485_CMD_CTMB.EKM_CMD_pcCmd = "CTMB";
	    APPL_DMA_RS485_CMD_CTMB.EKM_CMD_pcHelp = "Change to modbus";
	    APPL_DMA_RS485_CMD_CTMB.EKM_CMD_pfnCmd = &APPL_DMA_RS485_CTMB;
	    APPL_DMA_RS485_CMD_CTMB.next = &APPL_DMA_RS485_CMD_Test;

	    APPL_DMA_RS485_CMD_Test.EKM_CMD_pcCmd = "test";
	    APPL_DMA_RS485_CMD_Test.EKM_CMD_pcHelp = "Test function";
	    APPL_DMA_RS485_CMD_Test.EKM_CMD_pfnCmd = &APPL_DMA_RS485_Test;
	    APPL_DMA_RS485_CMD_Test.next = &APPL_DMA_RS485_CMD_LTE_Test;

	    APPL_DMA_RS485_CMD_LTE_Test.EKM_CMD_pcCmd = "lte";
	    APPL_DMA_RS485_CMD_LTE_Test.EKM_CMD_pcHelp = "LTE Test function";
	    APPL_DMA_RS485_CMD_LTE_Test.EKM_CMD_pfnCmd = &APPL_DMA_RS485_LTE_TEST;
	    APPL_DMA_RS485_CMD_LTE_Test.next =(void *)0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_printByte
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_RS485_printByte(uint8_t data){
	//wait for ready
	while (!(REG_USART1_CSR & US_CSR_TXRDY));
	while (!(REG_USART1_CSR & US_CSR_TXEMPTY));
	REG_USART1_THR |= data;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_printString
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_RS485_printString(const uint8_t myString[]) {
	uint8_t i = 0;
	while (myString[i]) {
		APPL_DMA_RS485_printByte(myString[i]);
		asm("nop");
		i++;
	}
}

/*******************************************************************************
* Function: APPL_DMA_RS485_printf
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_RS485_printf(const char *format, ...)
{
	// there was an error reading.
	uint8_t SerialBuffer[APPL_DMA_RS48_BUF_Size];
	memset(SerialBuffer, 0, APPL_DMA_RS48_BUF_Size);
	REG_PIOA_SODR |= PIO_PER_P23;
	va_list args;
	va_start(args, format);
	vsprintf((char *)SerialBuffer, format, args);
	va_end(args);
	APPL_DMA_RS485_printString(SerialBuffer);
	REG_PIOA_CODR |= PIO_PER_P23;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_LTE_EN
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_RS485_LTE_EN(uint8_t onff){
	if(onff%2==1){
		REG_PIOA_SODR |= PIO_PER_P29; //set PA11 high (LED on)
		}else{
		REG_PIOA_CODR |= PIO_PER_P29; //set PA11 low (LED Off)
	}
}

/*******************************************************************************
* Function: APPL_DMA_RS485_LineProcess
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_RS485_LineProcess(void)
{
	//APPL_DMA_RS485_printf("Command = %s", APPL_DMA_RS48_Rx.buffer);
	if (EKM_CMD_RXPeek(APPL_DMA_RS48_Rx.buffer, 0x0A, APPL_DMA_RS48_Rx.write) > 0)
	{
		APPL_DMA_RS485_printf("Command = %s\r\n", APPL_DMA_RS48_Rx.buffer);
		if (EKM_CMD_LineProcess((char *)APPL_DMA_RS48_Rx.buffer, &APPL_DMA_RS485_CMD_IDN, ' ') == EKM_CMD_LINE_BAD_CMD)
		APPL_DMA_RS485_printf("Command not found");
		EKM_Buffer_Reset(&APPL_DMA_RS48_Rx);
	}
}

/*******************************************************************************
* Function: APPL_DMA_RS485_IDN
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_IDN(int argc, char *argv[])
{
	APPL_DMA_RS485_printf("UU Control Version 1\r\n");
	APPL_DMA_RS485_printf("Firmware Version %d\r\n", 1);
	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_HELP
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_HELP(int argc, char *argv[])
{
	EKM_CMD_tCmdLineEntry *psCmdEntry;
	psCmdEntry = &APPL_DMA_RS485_CMD_IDN;
	APPL_DMA_RS485_printf("Command\t:Description\n\r");
	while (psCmdEntry != ((void *)0))
	{
		APPL_DMA_RS485_printf("%s\t:%s\n\r", psCmdEntry->EKM_CMD_pcCmd, psCmdEntry->EKM_CMD_pcHelp);
		psCmdEntry = psCmdEntry->next;
	}
	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_Status
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_Status(int argc, char *argv[])
{
	APPL_DMA_RS485_printf("Status Report\r\n");
	APPL_DMA_RS485_printf("Device ID : %d\r\n", APPL_DMA_IOCON_d.IOT_ID);
	APPL_DMA_RS485_printf("MQTT IP : %d.%d.%d.%d\r\n", APPL_DMA_IOCON_d.MQTT_IP0, 
													APPL_DMA_IOCON_d.MQTT_IP1, 
													APPL_DMA_IOCON_d.MQTT_IP2, 
													APPL_DMA_IOCON_d.MQTT_IP3);
	APPL_DMA_RS485_printf("MQTT Port : %d\r\n", APPL_DMA_IOCON_d.MQTT_PORT);
	APPL_DMA_RS485_printf("MQTT User : %s\r\n", APPL_DMA_IOCON_d.MQTT_USER);
	APPL_DMA_RS485_printf("MQTT Pass : %s\r\n", APPL_DMA_IOCON_d.MQTT_PASS);
	APPL_DMA_RS485_printf("Temperature = %d C\r\n", APPL_DMA_IOCON_d.Temperature);
	APPL_DMA_RS485_printf("Humidity = %d C\r\n", APPL_DMA_IOCON_d.Humidity);
	APPL_DMA_RS485_printf("Battery = %d \r\n", APPL_DMA_IOCON_d.Batt);
	
	APPL_DMA_RS485_printf("AN0 = %04X\r\n", APPL_DMA_IOCON_d.ANIN0);
	APPL_DMA_RS485_printf("AN1 = %04X\r\n", APPL_DMA_IOCON_d.ANIN1);
	APPL_DMA_RS485_printf("AN2 = %04X\r\n", APPL_DMA_IOCON_d.ANIN2);
	APPL_DMA_RS485_printf("AN3 = %04X\r\n", APPL_DMA_IOCON_d.ANIN3);
	
	APPL_DMA_RS485_printf("DOut Pin 0 = %d\r\n", APPL_DMA_IOCON_d.OUT0);
	APPL_DMA_RS485_printf("DOut Pin 1 = %d\r\n", APPL_DMA_IOCON_d.OUT1);
	APPL_DMA_RS485_printf("DOut Pin 2 = %d\r\n", APPL_DMA_IOCON_d.OUT2);
	APPL_DMA_RS485_printf("DOut Pin 3 = %d\r\n", APPL_DMA_IOCON_d.OUT3);
	APPL_DMA_RS485_printf("DIn Pin 0 = %d\r\n", APPL_DMA_IOCON_d.IN0);
	APPL_DMA_RS485_printf("DIn Pin 1 = %d\r\n", APPL_DMA_IOCON_d.IN1);
	APPL_DMA_RS485_printf("DIn Pin 2 = %d\r\n", APPL_DMA_IOCON_d.IN2);
	APPL_DMA_RS485_printf("DIn Pin 3 = %d\r\n", APPL_DMA_IOCON_d.IN3);
	
	APPL_DMA_RS485_printf("ALMOut Pin 0 = %d\r\n", APPL_DMA_IOCON_d.ALMOUT0);
	APPL_DMA_RS485_printf("ALMOut Pin 1 = %d\r\n", APPL_DMA_IOCON_d.ALMOUT1);
	APPL_DMA_RS485_printf("ALMIn Pin 0 = %d\r\n", APPL_DMA_IOCON_d.ALMIN0);
	APPL_DMA_RS485_printf("ALMIn Pin 1 = %d\r\n", APPL_DMA_IOCON_d.ALMIN1);
	
	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_GPIOW
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_GPIOW(int argc, char *argv[])
{
	if (argc < 3 || ((argc == 2) && (argv[1][0] == 'h')))
	{
		APPL_DMA_RS485_printf("Wrtie IO command usage:\n\n");
		APPL_DMA_RS485_printf("Specify an IO name( Pin , Status )\n");
		APPL_DMA_RS485_printf("separated by a space.\n\n");
		APPL_DMA_RS485_printf("For example:\n");
		APPL_DMA_RS485_printf(" gpiow 1 0\n");
		APPL_DMA_RS485_printf(" gpiow 1 1\n");
		return 0;
	}

	uint8_t Num_pin = atoi((const char *)argv[1]);
	uint8_t port = atoi((const char *)argv[2]);

	if (Num_pin == 0)
	{
		APPL_DMA_IOCON_d.OUT0 = port;
	}
	else if (Num_pin == 1)
	{
		APPL_DMA_IOCON_d.OUT1 = port;
	}
	else if (Num_pin == 2)
	{
		APPL_DMA_IOCON_d.OUT2 = port;
	}
	else if (Num_pin == 3)
	{
		APPL_DMA_IOCON_d.OUT3 = port;
	}
	APPL_DMA_RS485_printf("GPIO wirte\r\n");
	return 0;
}

/*******************************************************************************
* Function: AAPPL_DMA_RS485_ANALOG
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_ANALOG(int argc, char *argv[])
{
	APPL_DMA_IOCON_ADC_Get(&APPL_DMA_IOCON_d);
	APPL_DMA_RS485_printf("ANALOG read\r\n");
	APPL_DMA_RS485_printf("AN0 = %04X\r\n", APPL_DMA_IOCON_d.ANIN0);
	APPL_DMA_RS485_printf("AN1 = %04X\r\n", APPL_DMA_IOCON_d.ANIN1);
	APPL_DMA_RS485_printf("AN2 = %04X\r\n", APPL_DMA_IOCON_d.ANIN2);
	APPL_DMA_RS485_printf("AN3 = %04X\r\n", APPL_DMA_IOCON_d.ANIN3);
	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_SystemReset
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_SystemReset(int argc, char *argv[])
{

	uint8_t Num_pin = atoi((const char *)argv[1]);

	if (Num_pin == 0)
	{
		APPL_DMA_RS485_printf("System Shutdown\r\n");
	}
	else if (Num_pin == 1)
	{
		APPL_DMA_RS485_LTE_EN(0);
		APPL_DMA_RS485_printf("LTE Reset\r\n");
		APPL_DMA_RS485_LTE_EN(1);
	}
	else if (Num_pin == 2)
	{
		APPL_DMA_RS485_LTE_EN(0);
		APPL_DMA_RS485_printf("LTE Off\r\n");
	}
	else if (Num_pin == 3)
	{
		APPL_DMA_RS485_LTE_EN(1);
		APPL_DMA_RS485_printf("LTE On\r\n");
	}
	else if (Num_pin == 6)
	{
		APPL_DMA_RS485_printf("System Reset\r\n");
		NVIC_SystemReset();
	}

	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_CTMB
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_CTMB(int argc, char *argv[])
{
	APPL_DMA_RS485_printf("Change To Modbus\r\n");
	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_Test
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_Test(int argc, char *argv[])
{
	APPL_DMA_RS485_printf("-----------Test Function-----------\r\n");
	APPL_DMA_IOCON_TempAndHumi(&APPL_DMA_IOCON_d);
	APPL_DMA_RS485_printf("Temperature = %d C\r\n", APPL_DMA_IOCON_d.Temperature);
	APPL_DMA_RS485_printf("Humidity = %d C\r\n", APPL_DMA_IOCON_d.Humidity);
	APPL_DMA_RS485_printf("-----------Test Function-----------\r\n");
	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_RS485_LTE_TEST
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_DMA_RS485_LTE_TEST(int argc, char *argv[])
{
	APPL_DMA_RS485_printf("-----------Test Function-----------\r\n");
	APPL_DMA_RS485_printf("Data send %02x:%04X:%04X:%04X:%04X\r\n", 
		APPL_DMA_IOCON_d.DIO,
		APPL_DMA_IOCON_d.ANIN0,
		APPL_DMA_IOCON_d.ANIN1,
		APPL_DMA_IOCON_d.ANIN2,
		APPL_DMA_IOCON_d.ANIN3);
	APPL_DMA_LTE_MQTT_PUBLISH("IO", "%02x:%04X:%04X:%04X:%04X", 
		APPL_DMA_IOCON_d.DIO,
		APPL_DMA_IOCON_d.ANIN0,
		APPL_DMA_IOCON_d.ANIN1,
		APPL_DMA_IOCON_d.ANIN2,
		APPL_DMA_IOCON_d.ANIN3);
	APPL_DMA_RS485_printf("-----------Test Function-----------\r\n");
	return 0;
}

// END OF FILE