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
#include "main.h"
#include "usbd_cdc_if.h"
#include "Appl_USBService.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/
#define APPL_USB_BUF_Size 255

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
int APPL_USB_GPIOW(int argc, char *argv[]);
int APPL_USB_GPIOR(int argc, char *argv[]);
int APPL_USB_ANALOG(int argc, char *argv[]);
int APPL_USB_Address(int argc, char *argv[]);
int APPL_USB_SystemReset(int argc, char *argv[]);

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
uint8_t APPL_USB_Data[APPL_USB_BUF_Size];
EKM_CMD_tCmdLineEntry APPL_USB_CMD_IDN;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_HELP;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_Status;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_GPIOW;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_GPIOR;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_ANALOG;
EKM_CMD_tCmdLineEntry APPL_USB_CMD_SystemReset;

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
    APPL_USB_CMD_HELP.next = &APPL_USB_CMD_SystemReset;

    APPL_USB_CMD_SystemReset.EKM_CMD_pcCmd = "init";
    APPL_USB_CMD_SystemReset.EKM_CMD_pcHelp = "System Control";
    APPL_USB_CMD_SystemReset.EKM_CMD_pfnCmd = &APPL_USB_SystemReset;
    APPL_USB_CMD_SystemReset.next = &APPL_USB_CMD_Status;

    APPL_USB_CMD_Status.EKM_CMD_pcCmd = "status";
    APPL_USB_CMD_Status.EKM_CMD_pcHelp = "status of device";
    APPL_USB_CMD_Status.EKM_CMD_pfnCmd = &APPL_USB_Status;
    APPL_USB_CMD_Status.next = &APPL_USB_CMD_GPIOW;

    APPL_USB_CMD_GPIOW.EKM_CMD_pcCmd = "gpiow";
    APPL_USB_CMD_GPIOW.EKM_CMD_pcHelp = "On off relay";
    APPL_USB_CMD_GPIOW.EKM_CMD_pfnCmd = &APPL_USB_GPIOW;
    APPL_USB_CMD_GPIOW.next = &APPL_USB_CMD_GPIOR;

    APPL_USB_CMD_GPIOR.EKM_CMD_pcCmd = "gpior";
    APPL_USB_CMD_GPIOR.EKM_CMD_pcHelp = "read all input";
    APPL_USB_CMD_GPIOR.EKM_CMD_pfnCmd = &APPL_USB_GPIOR;
    APPL_USB_CMD_GPIOR.next = &APPL_USB_CMD_ANALOG;

    APPL_USB_CMD_ANALOG.EKM_CMD_pcCmd = "analog";
    APPL_USB_CMD_ANALOG.EKM_CMD_pcHelp = "Read Analog Data";
    APPL_USB_CMD_ANALOG.EKM_CMD_pfnCmd = &APPL_USB_ANALOG;
    APPL_USB_CMD_ANALOG.next = (void *)0;

    EKM_Buffer_Setup(&APPL_USB_Rx, APPL_USB_BUF_Size, APPL_USB_Data);
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
    //APPL_USB_printf("Command = %s", APPL_USB_Rx.buffer);
    if (EKM_CMD_RXPeek(APPL_USB_Rx.buffer, 0x0A, APPL_USB_Rx.write) > 0)
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
    uint8_t SerialBuffer[APPL_USB_BUF_Size];
    memset(SerialBuffer, 0, APPL_USB_BUF_Size);
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
    APPL_USB_printf("UU Control Version 1\r\n");
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
    APPL_USB_printf("MQTT IP : %d.%d.%d.%d\r\n", APPL_StatusS.MQTT_IP0, APPL_StatusS.MQTT_IP1, APPL_StatusS.MQTT_IP2, APPL_StatusS.MQTT_IP3);
    APPL_USB_printf("MQTT Port : %d\r\n", APPL_StatusS.MQTT_PORT);
    APPL_USB_printf("MQTT User : %s\r\n", APPL_StatusS.MQTT_USER);
    APPL_USB_printf("MQTT Pass : %s\r\n", APPL_StatusS.MQTT_PASS);
    APPL_USB_printf("Temperature = %d C\r\n", APPL_StatusS.Temperature);
    APPL_USB_printf("Humidity = %d C\r\n", APPL_StatusS.Humidity);
    APPL_USB_printf("Battery = %d \r\n", APPL_StatusS.Batt);
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_GPIOW
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_GPIOW(int argc, char *argv[])
{
    if (argc < 3 || ((argc == 2) && (argv[1][0] == 'h')))
    {
        APPL_USB_printf("Wrtie IO command usage:\n\n");
        APPL_USB_printf("Specify an IO name( Pin , Status )\n");
        APPL_USB_printf("separated by a space.\n\n");
        APPL_USB_printf("For example:\n");
        APPL_USB_printf(" gpiow 1 0\n");
        APPL_USB_printf(" gpiow 1 1\n");
        return 0;
    }

    uint8_t Num_pin = atoi((const char *)argv[1]);
    uint8_t port = atoi((const char *)argv[2]);

    if (Num_pin == 0)
    {
        APPL_IOCON_s.OUT0 = port;
    }
    else if (Num_pin == 1)
    {
        APPL_IOCON_s.OUT1 = port;
    }
    else if (Num_pin == 2)
    {
        APPL_IOCON_s.OUT2 = port;
    }
    else if (Num_pin == 3)
    {
        APPL_IOCON_s.OUT3 = port;
    }
    APPL_USB_printf("GPIO wirte\r\n");
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_GPIOR
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_GPIOR(int argc, char *argv[])
{
    APPL_USB_printf("GPIO read\r\n");
    APPL_USB_printf("Out Pin 0 = %d\r\n", APPL_IOCON_s.OUT0);
    APPL_USB_printf("Out Pin 1 = %d\r\n", APPL_IOCON_s.OUT1);
    APPL_USB_printf("Out Pin 2 = %d\r\n", APPL_IOCON_s.OUT2);
    APPL_USB_printf("Out Pin 3 = %d\r\n", APPL_IOCON_s.OUT3);
    APPL_USB_printf("In Pin 0 = %d\r\n", APPL_IOCON_s.IN0);
    APPL_USB_printf("In Pin 1 = %d\r\n", APPL_IOCON_s.IN1);
    APPL_USB_printf("In Pin 2 = %d\r\n", APPL_IOCON_s.IN2);
    APPL_USB_printf("In Pin 3 = %d\r\n", APPL_IOCON_s.IN3);
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_ANALOG
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_ANALOG(int argc, char *argv[])
{
    APPL_IOCON_ADC_Get(&APPL_IOCON_s);
    APPL_USB_printf("ANALOG read\r\n");
    APPL_USB_printf("AN0 = %04X\r\n", APPL_IOCON_s.ANIN0);
    APPL_USB_printf("AN1 = %04X\r\n", APPL_IOCON_s.ANIN1);
    APPL_USB_printf("AN2 = %04X\r\n", APPL_IOCON_s.ANIN2);
    APPL_USB_printf("AN3 = %04X\r\n", APPL_IOCON_s.ANIN3);
    return 0;
}

/*******************************************************************************
* Function: APPL_USB_SystemReset
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int APPL_USB_SystemReset(int argc, char *argv[])
{

    uint8_t Num_pin = atoi((const char *)argv[1]);

    if (Num_pin == 0)
    {
        APPL_USB_printf("System Shutdown\r\n");
    }
    else if (Num_pin == 1)
    {
        HAL_GPIO_WritePin(UART1_EN_GPIO_Port, UART1_EN_Pin, GPIO_PIN_RESET);
        APPL_USB_printf("LTE Reset\r\n");
        HAL_GPIO_WritePin(UART1_EN_GPIO_Port, UART1_EN_Pin, GPIO_PIN_SET);
    }
    else if (Num_pin == 2)
    {
        HAL_GPIO_WritePin(UART1_EN_GPIO_Port, UART1_EN_Pin, GPIO_PIN_RESET);
        APPL_USB_printf("LTE Off\r\n");
    }
    else if (Num_pin == 3)
    {
        HAL_GPIO_WritePin(UART1_EN_GPIO_Port, UART1_EN_Pin, GPIO_PIN_SET);
        APPL_USB_printf("LTE On\r\n");
    }
    else if (Num_pin == 6)
    {
        APPL_USB_printf("System Reset\r\n");
        HAL_NVIC_SystemReset();
    }

    return 0;
}

//END OF FILE