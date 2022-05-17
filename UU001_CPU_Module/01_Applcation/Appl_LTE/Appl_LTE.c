/*
 * Appl_LTE.c
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

#define APPL_LTE_BUFFER_MAX 255

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
int APPL_LTE_RUN = 0;
int APPL_LTE_SEND_RUN = 0;
int APPL_LTE_CONNECTWAIT = 0;
int APPL_LTE_MQTT_ONLINE = 0;
uint8_t APPL_LTE_Buffer_d[APPL_LTE_BUFFER_MAX];
EKM_Buffer_t APPL_LTE_Buffer;

void APPL_LTE_Init(void);
void APPL_LTE_SIM7000G_Init(void);
void APPL_LTE_printf(const char *format, ...);
void APPL_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...);
void APPL_LTE_MODBUS_MQTT_PUBLISH(const char *MQTT_Topic, EKM_MODBUS_t *tmp);
void APPL_LTE_LineProcess(void);

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
    uint8_t LTE_wait = 0;
    HAL_GPIO_WritePin(UART1_EN_GPIO_Port,UART1_EN_Pin,GPIO_PIN_RESET);
    EKM_Buffer_Setup(&APPL_LTE_Buffer, APPL_LTE_BUFFER_MAX, APPL_LTE_Buffer_d);
    EKM_Buffer_Reset(&APPL_LTE_Buffer);
    HAL_GPIO_WritePin(UART1_EN_GPIO_Port,UART1_EN_Pin,GPIO_PIN_SET);
    
    for (LTE_wait = 0; LTE_wait < 15; LTE_wait++)
    {
        APPL_Status_LED_Toogle();
        HAL_Delay(500);
        APPL_Status_LED_Toogle();
        HAL_Delay(500);
    }
    APPL_LTE_SIM7000G_Init();
}

/*******************************************************************************
* Function: APPL_LTE_SIM7000G_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_LTE_SIM7000G_Init(void)
{
    uint16_t delayStep = 50;

    APPL_LTE_printf("AT\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("ATE0\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"CLIENTID\",\"id\"\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"KEEPTIME\",60\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"URL\",\"%d.%d.%d.%d\",\"%d\"\r\n",
                    APPL_StatusS.MQTT_IP0,
                    APPL_StatusS.MQTT_IP1,
                    APPL_StatusS.MQTT_IP2,
                    APPL_StatusS.MQTT_IP3, APPL_StatusS.MQTT_PORT);
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"USERNAME\",\"%s\"\r\n", APPL_StatusS.MQTT_USER);
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"PASSWORD\",\"%s\"\r\n", APPL_StatusS.MQTT_USER);
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"CLEANSS\",1\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"QOS\",0\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+SMCONF=\"RETAIN\",0\r\n");
    HAL_Delay(delayStep);
    APPL_LTE_printf("AT+CMNB=3\r\n");
    HAL_Delay(delayStep);
    EKM_Buffer_Reset(&APPL_LTE_Buffer);
    APPL_LTE_printf("AT+CNACT=1,\"cmnet\"\r\n");
    HAL_Delay(delayStep);
}

/*******************************************************************************
* Function: APPL_LTE_printf
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_LTE_printf(const char *format, ...)
{
    // there was an error reading.
    uint8_t SerialBuffer[100];
    memset(SerialBuffer, 0, 100);
    va_list args;
    va_start(args, format);
    vsprintf((char *)SerialBuffer, format, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, SerialBuffer, strlen((char *)SerialBuffer), 10);
    HAL_Delay(1);
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
    if (APPL_LTE_MQTT_ONLINE == 1)
    {
        // there was an error reading.
        uint8_t SerialBuffer[100];
        memset(SerialBuffer, 0, 100);
        va_list args;
        va_start(args, format);
        vsprintf((char *)SerialBuffer, format, args);
        va_end(args);
        APPL_LTE_printf("AT+SMPUB=\"UUCON/%d/%s\",%d,0,0\r\n", APPL_StatusS.IOT_ID, MQTT_Topic, strlen((char *)SerialBuffer));
        APPL_LTE_printf("%s\r\n", SerialBuffer);
    }
}

/*******************************************************************************
* Function: APPL_LTE_MODBUS_MQTT_PUBLISH
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_LTE_MODBUS_MQTT_PUBLISH(const char *MQTT_Topic, EKM_MODBUS_t *tmp)
{
    if (APPL_LTE_MQTT_ONLINE == 1)
    {
        // there was an error reading.
        int i;
        uint8_t Ftemp[255];
        for (i = 0; i < tmp->len; i++)
        {
            Ftemp[i] = tmp->MODdata[i];
        }
        Ftemp[i++] = tmp->HCRC;
        Ftemp[i] = tmp->LCRC;
        APPL_USB_printf("--------------\n\r");
        APPL_LTE_printf("AT+SMPUB=\"UUCON/%d/%s\",%d,0,0\r\n", APPL_StatusS.IOT_ID, MQTT_Topic, (tmp->len + 2) * 3);
        for (i = 0; i < tmp->len + 2; i++)
        {
            APPL_USB_printf("%02x ", Ftemp[i]);
            APPL_LTE_printf("%02X ", Ftemp[i]);
        }
        APPL_USB_printf("--------------\n\r");
    }
}

/*******************************************************************************
* Function: APPL_LTE_LineProcess
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_LTE_LineProcess(void)
{
    //APPL_USB_printf("LTE = %s\r\n",APPL_LTE_Buffer.buffer);
    if (APPL_LTE_RUN == 1)
    {
        // String pre
        uint8_t buffer_COMp[30];
        sprintf((char *)buffer_COMp, "+SMSUB: \"UUCON/CON/%d\",\"C", APPL_StatusS.IOT_ID);
        // USB Debug
        APPL_USB_printf("%s\r\n", APPL_LTE_Buffer.buffer);
        if (strstr((char *)APPL_LTE_Buffer.buffer, "OK") != NULL)
        {
            if (APPL_LTE_CONNECTWAIT == 1)
            {
                APPL_USB_printf("LTE Sub\r\n");
                APPL_LTE_printf("AT+SMSUB=\"UUCON/CON/%d\",0\r\n", APPL_StatusS.IOT_ID);
                APPL_LTE_SEND_RUN = 1; // Run Send
                APPL_LTE_CONNECTWAIT = 2;
            }
            else if (APPL_LTE_CONNECTWAIT == 2)
            {
                APPL_USB_printf("LTE First Sub\r\n");
                APPL_LTE_MQTT_PUBLISH("ST", "ONline");
                APPL_LTE_MQTT_ONLINE = 1;
                APPL_LTE_CONNECTWAIT = 0;
            }
        }
        else if (strstr((char *)APPL_LTE_Buffer.buffer, "APP PDP: DEACTIVE") != NULL)
        {
            APPL_USB_printf("LTE Reday\r\n");
            APPL_USB_printf("LTE CMNET Reconnect\r\n");
            APPL_LTE_printf("AT+CNACT=1,\"cmnet\"\r\n");
        }
        else if (strstr((char *)APPL_LTE_Buffer.buffer, "+APP PDP: ACTIVE") != NULL)
        {
            APPL_USB_printf("LTE Connect\r\n");
            APPL_LTE_printf("AT+SMCONN\r\n");
            APPL_LTE_CONNECTWAIT = 1;
        }
        else if (strstr((char *)APPL_LTE_Buffer.buffer, (char *)buffer_COMp))
        {
            if (APPL_LTE_SEND_RUN == 1)
            {
                char *c = strtok((char *)APPL_LTE_Buffer.buffer, ",");
                c = strtok(NULL, " ");
                memmove(c, c + 1, strlen(c));
                APPL_IOCON_s.DIO = (uint8_t)(strtol(c, (void *)0, 16));
                //APPL_USB_printf("c = %s %d 0x%02X\r\n",c,strlen(c),APPL_IOCON_s.DIO);
            }
        }

        EKM_Buffer_Reset(&APPL_LTE_Buffer);
        APPL_LTE_RUN = 0;
    }
}

//END OF FILE