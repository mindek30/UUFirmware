/*
 * APPL_CMD.c
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

/*******************************************************************************
 *
 * DEFINE
 *
 ******************************************************************************/
#define APPL_CMD_BUF_Size 255

/*******************************************************************************
 *
 * GOBAL VARIABLE
 *
 ******************************************************************************/
EKM_Buffer_t APPL_CMD_Rx;

void APPL_CMD_Init(void);
void APPL_CMD_LineProcess(void);
void APPL_CMD_printf(const char *format, ...);

int APPL_CMD_IDN(int argc, char *argv[]);
int APPL_CMD_HELP(int argc, char *argv[]);
int APPL_CMD_SystemReset(int argc, char *argv[]);
int APPL_CMD_LAMP_Onoff(int argc, char *argv[]);
int APPL_CMD_TurnON_PUMP(int argc, char *argv[]);
int APPL_CMD_ShowData(int argc, char *argv[]);
int APPL_CMD_Analog_READ(int argc, char *argv[]);
int APPL_CMD_4_20mA(int argc, char *argv[]);
int APPL_CMD_GAIN_Setup(int argc, char *argv[]);
int APPL_CMD_RS485(int argc, char *argv[]);

/*******************************************************************************
 *
 * PRIVATE VARIABLE
 *
 ******************************************************************************/
uint8_t APPL_CMD_Data[APPL_CMD_BUF_Size];
EKM_CMD_tCmdLineEntry APPL_CMD_IDN_c;
EKM_CMD_tCmdLineEntry APPL_CMD_HELP_c;
EKM_CMD_tCmdLineEntry APPL_CMD_SystemReset_c;
EKM_CMD_tCmdLineEntry APPL_CMD_LAMP_Onoff_c;
EKM_CMD_tCmdLineEntry APPL_CMD_TurnON_PUMP_c;
EKM_CMD_tCmdLineEntry APPL_CMD_ShowData_c;
EKM_CMD_tCmdLineEntry APPL_CMD_Analog_READ_c;
EKM_CMD_tCmdLineEntry APPL_CMD_4_20mA_c;
EKM_CMD_tCmdLineEntry APPL_CMD_GAIN_Setup_c;
EKM_CMD_tCmdLineEntry APPL_CMD_RS485_c;
/*******************************************************************************
 * Function: APPL_CMD_Init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_CMD_Init(void)
{
    APPL_CMD_IDN_c.EKM_CMD_pcCmd = "IDN?";
    APPL_CMD_IDN_c.EKM_CMD_pcHelp = "defined in IEEE Std 488.2-1992";
    APPL_CMD_IDN_c.EKM_CMD_pfnCmd = &APPL_CMD_IDN;
    APPL_CMD_IDN_c.next = &APPL_CMD_HELP_c;

    APPL_CMD_HELP_c.EKM_CMD_pcCmd = "help";
    APPL_CMD_HELP_c.EKM_CMD_pcHelp = "Help command";
    APPL_CMD_HELP_c.EKM_CMD_pfnCmd = &APPL_CMD_HELP;
    APPL_CMD_HELP_c.next = &APPL_CMD_SystemReset_c;

    APPL_CMD_SystemReset_c.EKM_CMD_pcCmd = "init";
    APPL_CMD_SystemReset_c.EKM_CMD_pcHelp = "System Control";
    APPL_CMD_SystemReset_c.EKM_CMD_pfnCmd = &APPL_CMD_SystemReset;
    APPL_CMD_SystemReset_c.next = &APPL_CMD_LAMP_Onoff_c;

    APPL_CMD_LAMP_Onoff_c.EKM_CMD_pcCmd = "lamp";
    APPL_CMD_LAMP_Onoff_c.EKM_CMD_pcHelp = "lamp Control";
    APPL_CMD_LAMP_Onoff_c.EKM_CMD_pfnCmd = &APPL_CMD_LAMP_Onoff;
    APPL_CMD_LAMP_Onoff_c.next = &APPL_CMD_TurnON_PUMP_c;

    APPL_CMD_TurnON_PUMP_c.EKM_CMD_pcCmd = "pump";
    APPL_CMD_TurnON_PUMP_c.EKM_CMD_pcHelp = "Pump Control";
    APPL_CMD_TurnON_PUMP_c.EKM_CMD_pfnCmd = &APPL_CMD_TurnON_PUMP;
    APPL_CMD_TurnON_PUMP_c.next = &APPL_CMD_ShowData_c;

    APPL_CMD_ShowData_c.EKM_CMD_pcCmd = "read";
    APPL_CMD_ShowData_c.EKM_CMD_pcHelp = "Read Status";
    APPL_CMD_ShowData_c.EKM_CMD_pfnCmd = &APPL_CMD_ShowData;
    APPL_CMD_ShowData_c.next = &APPL_CMD_Analog_READ_c;

    APPL_CMD_Analog_READ_c.EKM_CMD_pcCmd = "analog";
    APPL_CMD_Analog_READ_c.EKM_CMD_pcHelp = "Ananlog read";
    APPL_CMD_Analog_READ_c.EKM_CMD_pfnCmd = &APPL_CMD_Analog_READ;
    APPL_CMD_Analog_READ_c.next = &APPL_CMD_4_20mA_c;

    APPL_CMD_4_20mA_c.EKM_CMD_pcCmd = "aout";
    APPL_CMD_4_20mA_c.EKM_CMD_pcHelp = "4-20 mA dirver";
    APPL_CMD_4_20mA_c.EKM_CMD_pfnCmd = &APPL_CMD_4_20mA;
    APPL_CMD_4_20mA_c.next = &APPL_CMD_GAIN_Setup_c;

    APPL_CMD_GAIN_Setup_c.EKM_CMD_pcCmd = "gain";
    APPL_CMD_GAIN_Setup_c.EKM_CMD_pcHelp = "Gain Setup";
    APPL_CMD_GAIN_Setup_c.EKM_CMD_pfnCmd = &APPL_CMD_GAIN_Setup;
    APPL_CMD_GAIN_Setup_c.next = &APPL_CMD_RS485_c;

    APPL_CMD_RS485_c.EKM_CMD_pcCmd = "rs485";
    APPL_CMD_RS485_c.EKM_CMD_pcHelp = "rs485 command";
    APPL_CMD_RS485_c.EKM_CMD_pfnCmd = &APPL_CMD_RS485;
    APPL_CMD_RS485_c.next = (void *)0;

    EKM_Buffer_Setup(&APPL_CMD_Rx, APPL_CMD_BUF_Size, APPL_CMD_Data);
}

/*******************************************************************************
 * Function: APPL_CMD_LineProcess
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_CMD_LineProcess(void)
{
    // APPL_CMD_printf("Command = %s\r\n", APPL_CMD_Rx.buffer);
    if (EKM_CMD_RXPeek(APPL_CMD_Rx.buffer, 0x0A, APPL_CMD_Rx.write) > 0)
    {
        // APPL_CMD_printf("IN Command = %s\r\n", APPL_CMD_Rx.buffer);
        if (EKM_CMD_LineProcess((char *)APPL_CMD_Rx.buffer, &APPL_CMD_IDN_c, ' ') == EKM_CMD_LINE_BAD_CMD)
            APPL_CMD_printf("Command not found");
        EKM_Buffer_Reset(&APPL_CMD_Rx);
    }
}

/*******************************************************************************
 * Function: APPL_CMD_printf
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_CMD_printf(const char *format, ...)
{
    // there was an error reading.
    uint8_t SerialBuffer[APPL_CMD_BUF_Size];
    memset(SerialBuffer, 0, APPL_CMD_BUF_Size);
    va_list args;
    va_start(args, format);
    vsprintf((char *)SerialBuffer, format, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, SerialBuffer, strlen((char *)SerialBuffer), 10);
    HAL_Delay(1);
}

/*******************************************************************************
 * Function: APPL_CMD_IDN
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_IDN(int argc, char *argv[])
{
    APPL_CMD_printf("UU Turbidity\r\n");
    APPL_CMD_printf("Firmware Version %d\r\n", 1);
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_HELP
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_HELP(int argc, char *argv[])
{
    EKM_CMD_tCmdLineEntry *psCmdEntry;
    psCmdEntry = &APPL_CMD_IDN_c;
    APPL_CMD_printf("Command\t:Description\n\r");
    while (psCmdEntry != ((void *)0))
    {
        APPL_CMD_printf("%s\t:%s\n\r", psCmdEntry->EKM_CMD_pcCmd, psCmdEntry->EKM_CMD_pcHelp);
        psCmdEntry = psCmdEntry->next;
    }
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_SystemReset
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_SystemReset(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    uint8_t Num_pin = atoi((const char *)argv[1]);

    if (Num_pin == 0)
    {
        APPL_CMD_printf("System Shutdown\r\n");
    }
    else if (Num_pin == 6)
    {
        APPL_CMD_printf("System Reset\r\n");
        HAL_NVIC_SystemReset();
    }

    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_LAMP_Onoff
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_LAMP_Onoff(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    uint8_t Num_pin = atoi((const char *)argv[1]);
    APPL_Task_d.LAMP_Onoff = Num_pin;
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_LAMP_Onoff
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_TurnON_PUMP(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    uint8_t Num_pin = atoi((const char *)argv[1]);
    APPL_Task_d.TurnON_PUMP = Num_pin;
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_ShowData
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_ShowData(int argc, char *argv[])
{
    APPL_CMD_printf("MODULE 1 ADC Value = %04X\r\n", APPL_Task_d.Mod1);
    APPL_CMD_printf("MODULE 2 ADC Value = %04X\r\n", APPL_Task_d.Mod2);
    APPL_CMD_printf("Pump %s\r\n", (APPL_Task_d.TurnON_PUMP == 1) ? "ON" : "OFF");
    APPL_CMD_printf("Lamp %s\r\n", (APPL_Task_d.LAMP_Onoff == 1) ? "ON" : "OFF");
    APPL_CMD_printf("Turbidity DAC Value = %04X\r\n", APPL_Task_d.Turbidity_Driver);
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_Analog_READ
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_Analog_READ(int argc, char *argv[])
{
    //uint16_t val = 0;
    //APPL_ads1115_startSampling(ADS1115_MUX_5); // P = AIN0, N = GND
    //while (APPL_ads1115_isBusy())
    //    ;
    //APPL_ads1115_readSample(&val);
    //APPL_Task_d.RAW_ADC_Ch0 = val;
    APPL_CMD_printf("Turbidity Ch0 ADC Value : %04X = %d uV\r\n", APPL_Task_d.RAW_ADC_Ch0, APPL_ads1115_uVolt(&APPL_Task_d.RAW_ADC_Ch0));
    //APPL_ads1115_startSampling(ADS1115_MUX_6); // P = AIN0, N = GND
    //while (APPL_ads1115_isBusy())
    //    ;
    //APPL_ads1115_readSample(&val);
    //APPL_Task_d.RAW_ADC_Ch1 = val;
    APPL_CMD_printf("Turbidity Ch1 ADC Value : %04X = %d uV\r\n", APPL_Task_d.RAW_ADC_Ch1, APPL_ads1115_uVolt(&APPL_Task_d.RAW_ADC_Ch1));
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_4_20mA
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_4_20mA(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    uint16_t Num_pin = atoi((const char *)argv[1]);
    APPL_CMD_printf("0x%04x ", APPL_DAC80502_write(APPL_DAC80502_TRIGGER, 0x1010));
    APPL_CMD_printf("0x%04x ", APPL_DAC80502_write(APPL_DAC80502_CONFIG, 0x0000));
    APPL_CMD_printf("0x%04x ", APPL_DAC80502_write(APPL_DAC80502_SYNC, 0x0300));
    APPL_CMD_printf("0x%04x ", APPL_DAC80502_write(APPL_DAC80502_DAC_B, Num_pin));
    APPL_CMD_printf("0x%04x ", APPL_DAC80502_write(APPL_DAC80502_TRIGGER, 0x0008));
    APPL_CMD_printf("DAC Value = %04X\r\n", Num_pin);
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_GAIN_Setup
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_GAIN_Setup(int argc, char *argv[])
{
    if (argc < 3)
        return 1;

    uint16_t xr = atoi((const char *)argv[2]);

    switch (argv[1][0])
    {
    case 'A':
    {
        APPL_Task_d.GAIN_A = xr;
        APPL_TPL0401X_write(APPL_TPL0401X_ADD_A, APPL_Task_d.GAIN_A);
        break;
    }
    case 'B':
    {
        APPL_Task_d.GAIN_B = xr;
        APPL_TPL0401X_write(APPL_TPL0401X_ADD_B, APPL_Task_d.GAIN_B);
        break;
    }
    default:
    {
        break;
    }
    }
    APPL_CMD_printf("GAIN A = %d\r\n", APPL_Task_d.GAIN_A);
    APPL_CMD_printf("GAIN B = %d\r\n", APPL_Task_d.GAIN_B);
    return 0;
}

/*******************************************************************************
 * Function: APPL_CMD_GAIN_Setup
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_CMD_RS485(int argc, char *argv[])
{
    EKM_MODBUS_t tm;
    uint8_t test[10];
    Bit16ToBit8_t tmpx;
    tmpx.REG = APPL_Task_d.RAW_ADC_Ch0;
    EKM_MODBUS_Setup(&tm, test, 5);
    EKM_MODBUS_Receiver_Set(&tm, 0x01);
    EKM_MODBUS_Receiver_Set(&tm, 0x03);
    EKM_MODBUS_Receiver_Set(&tm, 0x02);
    EKM_MODBUS_Receiver_Set(&tm, tmpx.HREG);
    EKM_MODBUS_Receiver_Set(&tm, tmpx.LREG);
    EKM_MODBUS_CRC_Encode(&tm);
    APPL_Module_MODBUS_print(&tm);
    EKM_MODBUS_DataClear(&tm);
    return 0;
}

// END OF FILE