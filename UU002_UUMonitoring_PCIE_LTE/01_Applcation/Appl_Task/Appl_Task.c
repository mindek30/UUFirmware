/*
 * Appl_Task.c
 *
 * Created: 05/06/2022 1:46:27
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
#include "gpio.h"
#include "i2c.h"

#include "EKM_CMD.h"
#include "EKM_Buffer.h"
#include "Appl_Task.h"
#include "Appl_USBService.h"
#include "Appl_Data.h"
#include "APPL_DMA_LTE.h"

/*******************************************************************************
 *
 * DEFINE
 *
 ******************************************************************************/

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
void APPL_Task_Init(void);
void APPL_Task_10ms(void);
void APPL_Task_100ms(void);
void APPL_Task_1s(void);
/*******************************************************************************
 *
 * PRIVATE VARIABLE
 *
 ******************************************************************************/
uint8_t time_10ms = 0;
uint8_t time_100ms = 0;
uint8_t time_1s = 0;


/*******************************************************************************
 * Function: APPL_Task_Init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Task_Init(void)
{
    APPL_USB_Init();
    APPL_USB_printf("USB Init\n\r");
    APPL_Data_Init();
    APPL_USB_printf("Data Init\n\r");
    APPL_DMA_LTE_Init();
    HAL_GPIO_WritePin(USART1_EN_GPIO_Port,USART1_EN_Pin,GPIO_PIN_SET);
    APPL_USB_printf("LTE Init\n\r");
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
void APPL_Task_10ms(void)
{
    time_10ms++;
    APPL_USB_LineProcess();
    switch (time_10ms)
    {
    case 1:
    {
        /* code */
        HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port,LED_STATUS_Pin);
        break;
    }
    case 2:
    {
        /* code */
        APPL_Data_MODBUS_Task();
        break;
    }
    case 3:
    {
        /* code */
        break;
    }
    case 4:
    {
        /* code */
        break;
    }
    case 5:
    {
        /* code */
        break;
    }
    case 6:
    {
        /* code */
        break;
    }
    case 7:
    {
        /* code */
        break;
    }
    case 8:
    {
        /* code */
        break;
    }
    case 9:
    {
        /* code */
        break;
    }
    case 10:
    {
        /* code */
        time_10ms = 0;
        APPL_Task_100ms();
        break;
    }
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
void APPL_Task_100ms(void)
{
    time_100ms++;
    switch (time_100ms)
    {
    case 1:
    {
        /* code */
        break;
    }
    case 2:
    {
        /* code */
        break;
    }
    case 3:
    {
        /* code */
        break;
    }
    case 4:
    {
        /* code */
        break;
    }
    case 5:
    {
        /* code */
        break;
    }
    case 6:
    {
        /* code */
        break;
    }
    case 7:
    {
        /* code */
        break;
    }
    case 8:
    {
        /* code */
        break;
    }
    case 9:
    {
        /* code */
        break;
    }
    case 10:
    {
        /* code */
        time_100ms=0;
        APPL_Task_1s();
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
void APPL_Task_1s(void)
{
    time_1s++;
    switch (time_1s)
    {
    case 1:
    {
        /* code */
        break;
    }
    case 2:
    {
        /* code */
        break;
    }
    case 3:
    {
        /* code */
        break;
    }
    case 4:
    {
        /* code */
        break;
    }
    case 5:
    {
        /* code */
        break;
    }
    case 6:
    {
        /* code */
        break;
    }
    case 7:
    {
        /* code */
        break;
    }
    case 8:
    {
        /* code */
        break;
    }
    case 9:
    {
        /* code */
        break;
    }
    case 10:
    {
        /* code */
        time_1s = 0;
        break;
    }
    }
}

//***** End of File *****//