/*
 * Appl_USBService.c
 *
 *  Created on: Aug 22, 2021
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
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "EKM_Buffer.h"
#include "EKM_CMD.h"
#include "EKM_Modbus.h"
#include "usbd_cdc_if.h"
#include "Appl_USBService.h"
#include "Appl_LTE.h"
#include "Appl_Status.h"
#include "Appl_Module.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
EKM_Buffer_t APPL_USB_Rx;

void APPL_USB_Init(void);
void APPL_USB_LineProcess(void);
void APPL_USB_printf(const char *format, ...);
int APPL_USB_IDN(int argc, char *argv[]);
int APPL_USB_HELP(int argc, char *argv[]);
int APPL_USB_Status(int argc, char *argv[]);
int APPL_USB_Address(int argc, char *argv[]);
int APPL_USB_LTE_Send(int argc, char *argv[]);

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
uint8_t APPL_USB_Data[100];
EKM_CMD_tCmdLineEntry APPL_USB_CMD_IDN;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_HELP;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_Status;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_Address;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_LTE_Send;

/*******************************************************************************
* Function: APPL_USB_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_USB_Init(void)
{
    APPL_USB_CMD_IDN.EKM_CMD_pcCmd = "IDN?";
    APPL_USB_CMD_IDN.EKM_CMD_pcHelp = "defined in IEEE Std 488.2-1992";
    APPL_USB_CMD_IDN.EKM_CMD_pfnCmd = &APPL_USB_IDN;
    APPL_USB_CMD_IDN.next = &APPL_USB_CMD_HELP;

    APPL_USB_CMD_HELP.EKM_CMD_pcCmd = "help";
    APPL_USB_CMD_HELP.EKM_CMD_pcHelp = "Help command";
    APPL_USB_CMD_HELP.EKM_CMD_pfnCmd = &APPL_USB_HELP;
    APPL_USB_CMD_HELP.next = &APPL_USB_CMD_Status;

    APPL_USB_CMD_Status.EKM_CMD_pcCmd = "status";
    APPL_USB_CMD_Status.EKM_CMD_pcHelp = "status of device";
    APPL_USB_CMD_Status.EKM_CMD_pfnCmd = &APPL_USB_Status;
    APPL_USB_CMD_Status.next = &APPL_USB_CMD_Address;

    APPL_USB_CMD_Address.EKM_CMD_pcCmd = "RS485";
    APPL_USB_CMD_Address.EKM_CMD_pcHelp = "RS485 To Device";
    APPL_USB_CMD_Address.EKM_CMD_pfnCmd = &APPL_USB_Address;
    APPL_USB_CMD_Address.next = &APPL_USB_CMD_LTE_Send;

    APPL_USB_CMD_LTE_Send.EKM_CMD_pcCmd = "LTE";
    APPL_USB_CMD_LTE_Send.EKM_CMD_pcHelp = "AT Command To SIMCOM7000G";
    APPL_USB_CMD_LTE_Send.EKM_CMD_pfnCmd = &APPL_USB_LTE_Send;
    APPL_USB_CMD_LTE_Send.next = (void *)0;

    EKM_Buffer_Setup(&APPL_USB_Rx, 100, APPL_USB_Data);
}

/*******************************************************************************
* Function: APPL_USB_LineProcess
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_USB_LineProcess(void)
{
    if (EKM_CMD_RXPeek(APPL_USB_Rx.buffer, 0x0D, APPL_USB_Rx.write) > 0)
    {
        //APPL_USB_printf("Command = %s", APPL_USB_Rx.buffer);
        if (EKM_CMD_LineProcess((char *)APPL_USB_Rx.buffer, &APPL_USB_CMD_IDN, ' ') == EKM_CMD_LINE_BAD_CMD)
            APPL_USB_printf("Command not found");
        EKM_Buffer_Reset(&APPL_USB_Rx);
    }
}

/*******************************************************************************
* Function: APPL_USB_printf
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_USB_printf(const char *format, ...)
{
    // there was an error reading.
    uint8_t SerialBuffer[100];
    memset(SerialBuffer, 0, 100);
    va_list args;
    va_start(args, format);
    vsprintf((char *)SerialBuffer, format, args);
    va_end(args);
    CDC_Transmit_FS(SerialBuffer, strlen((char *)SerialBuffer));
    HAL_Delay(1);
}

/*******************************************************************************
* Function: APPL_USB_IDN
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_IDN(int argc, char *argv[])
{
    APPL_USB_printf("UU Monitoring Version 1\r\n");
    APPL_USB_printf("Firmware Version %d\r\n", 1);
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_HELP
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_HELP(int argc, char *argv[])
{
    EKM_CMD_tCmdLineEntry *psCmdEntry;
    psCmdEntry = &APPL_USB_CMD_IDN;
    APPL_USB_printf("Command\t:Description\n\r");
    while (psCmdEntry != ((void *)0))
    {
        APPL_USB_printf("%s\t:%s\n\r", psCmdEntry->EKM_CMD_pcCmd, psCmdEntry->EKM_CMD_pcHelp);
        psCmdEntry = psCmdEntry->next;
    }
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_Status
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_Status(int argc, char *argv[])
{
    APPL_USB_printf("Status Report\r\n");
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_Address
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_Address(int argc, char *argv[])
{
    if(argc > 1){
        APPL_USB_printf("RS485 Command =  %s\r\n",argv[1]);
        APPL_Module_printf("%s",argv[1]);
    }
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_LTE_Send
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_LTE_Send(int argc, char *argv[])
{
    if(argc > 1){
        //APPL_USB_printf("LTE Command =  %s\r\n",argv[1]);
        APPL_LTE_printf("%s\r\n",argv[1]);
    }
    return 0;
}
//END OF FILE