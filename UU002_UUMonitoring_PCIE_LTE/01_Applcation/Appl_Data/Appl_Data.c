/*
 * Appl_Data.c
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
#include "Appl_Data.h"

/*******************************************************************************
 *
 * DEFINE
 *
 ******************************************************************************/
#define APPL_Data_RS485_Buffer_Size 255

//! Structure for an entry in the command list table.
uint8_t APPL_Data_RS485_Buffer[APPL_Data_RS485_Buffer_Size];
uint8_t APPL_Data_RS485_Buffer_pointer = 0;

APPL_DATA_t APPL_data;
//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
static int APPL_Data_ID_Set(uint16_t data);
static int APPL_Data_ID_Get(void);
uint16_t APPL_Data_RS485_CRC_Encode(uint8_t len, uint8_t *pdata);
uint8_t APPL_Data_RS485_CRC_Decode(uint8_t *data, uint8_t len);
uint8_t APPL_Data_RS485_Send(uint8_t *data, uint8_t len);
uint8_t APPL_Data_RS485_Set(uint8_t data);

/*******************************************************************************
 *
 * PRIVATE VARIABLE
 *
 ******************************************************************************/

/*******************************************************************************
 * Function: APPL_Data_Init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Data_Init(void)
{
}

/*******************************************************************************
 * Function: APPL_Data_ID_Set
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
static int APPL_Data_ID_Set(uint16_t data)
{
    HAL_StatusTypeDef ret;
    uint8_t send[3];
    EKM_16Bto8B_t tmp;
    APPL_data.deviceID = data;
    tmp.REG = data;
    send[0] = APPL_DATA_ID0_ADD;
    send[1] = tmp.HREG;
    send[2] = tmp.LREG;
    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_SET);

    ret = HAL_I2C_Master_Transmit(&hi2c1, AT24AA02_Page0 << 1, send, 3, 100);

    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_RESET);
    // APPL_CMD_printf("I2C = 0x%x\r\n", ret);
    return ret;
}

/*******************************************************************************
 * Function: APPL_Data_ID_Get
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
static int APPL_Data_ID_Get(void)
{
    HAL_StatusTypeDef ret;
    uint8_t send[3];
    EKM_16Bto8B_t tmp;

    send[0] = APPL_DATA_ID0_ADD;
    send[1] = 0;
    send[2] = 0;

    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_SET);
    ret = HAL_I2C_Master_Transmit(&hi2c1, AT24AA02_Page0 << 1, send, 1, 100);
    ret |= HAL_I2C_Master_Receive(&hi2c1, AT24AA02_Page0 << 1, send, 2, 100);

    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_RESET);
    tmp.HREG = send[0];
    tmp.LREG = send[1];

    APPL_data.deviceID = tmp.REG;
    // APPL_CMD_printf("I2C = 0x%x\r\n", ret);
    return ret;
}

/*******************************************************************************
 * Function: APPL_Data_RS485_CRC_Encode
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint16_t APPL_Data_RS485_CRC_Encode(uint8_t len, uint8_t *pdata)
{
    uint8_t pos = 0;
    uint16_t crc = 0xFFFF;
    if (pdata != (void *)0)
    {
        return 0;
    }
    for (pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t)pdata[pos]; // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--)
        { // Loop over each bit
            if ((crc & 0x0001) != 0)
            {              // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else           // Else LSB is not set
                crc >>= 1; // Just shift right
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

/*******************************************************************************
 * Function: APPL_Data_RS485_CRC_Decode
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t APPL_Data_RS485_CRC_Decode(uint8_t *data, uint8_t len)
{
    uint8_t pos = 0;
    uint16_t crc = 0xFFFF;
    EKM_16Bto8B_t tmp;
    if (data != (void *)0)
    {
        return HAL_ERROR;
    }
    for (pos = 0; pos < (len - 2); pos++)
    {
        crc ^= (uint16_t)data[pos]; // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--)
        { // Loop over each bit
            if ((crc & 0x0001) != 0)
            {              // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else           // Else LSB is not set
                crc >>= 1; // Just shift right
        }
    }
    if (((uint16_t)data[len - 2] == tmp.HREG) &&
        ((uint16_t)data[len - 1] == tmp.LREG))
    {

        return 1;
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return 0;
}

/*******************************************************************************
 * Function: APPL_Data_RS485_Send
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t APPL_Data_RS485_Send(uint8_t *data, uint8_t len)
{
    EKM_16Bto8B_t tmp;
    HAL_StatusTypeDef ret;
    if (data != (void *)0)
    {
        return HAL_ERROR;
    }
    HAL_GPIO_WritePin(USART2_EN_GPIO_Port, USART2_EN_Pin, GPIO_PIN_SET);
    for (int i = 0; i < len; i++)
    {
        HAL_UART_Transmit(&huart2, &data[i], 1, 10);
        HAL_Delay(1);
    }
    HAL_GPIO_WritePin(USART2_EN_GPIO_Port, USART2_EN_Pin, GPIO_PIN_RESET);
    return ret;
}

/*******************************************************************************
 * Function: APPL_Data_RS485_Set
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t APPL_Data_RS485_Set(uint8_t data)
{
    return EKM_Buffer_Raw_set(APPL_Data_RS485_Buffer,
                              APPL_Data_RS485_Buffer_Size,
                              &APPL_Data_RS485_Buffer_pointer,
                              data);
}

/*******************************************************************************
 * Function: APPL_Data_MODBUS_Set
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t APPL_Data_MODBUS_Set(uint8_t *data, uint8_t len)
{
    int i;
    EKM_16Bto8B_t tmp;
    HAL_StatusTypeDef ret;
    uint8_t send[255];
    send[0] = APPL_DATA_Modbus_ADD;
    send[1] = APPL_DATA_Modbus_ADD + 1;
    if (data != (void *)0)
    {
        return HAL_ERROR;
    }
    if (len >= 250)
    {
        return HAL_ERROR;
    }
    for (i = 0; i < len; i++)
    {
        APPL_data.modsend[i] = data[i];
        send[i + 2] = data[i];
    }
    tmp.REG = APPL_Data_RS485_CRC_Encode(len, data);
    APPL_data.modsend[i++] = tmp.HREG;
    APPL_data.modsend[i++] = tmp.LREG;
    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_SET);
    ret = HAL_I2C_Master_Transmit(&hi2c1, AT24AA02_Page0 << 1, send, i + 2, 100);
    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_RESET);

    return ret;
}

/*******************************************************************************
 * Function: APPL_Data_MODBUS_Get
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t APPL_Data_MODBUS_Get(uint8_t *data)
{
    int i;
    EKM_16Bto8B_t tmp;
    HAL_StatusTypeDef ret;
    uint8_t send[255];

    send[0] = APPL_DATA_ID0_ADD;
    send[1] = 0;
    send[2] = 0;

    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_SET);
    
    ret = HAL_I2C_Master_Transmit(&hi2c1, AT24AA02_Page0 << 1, send, 1, 100);
    ret |= HAL_I2C_Master_Receive(&hi2c1, AT24AA02_Page0 << 1, send, 252, 100);

    HAL_GPIO_WritePin(I2C1_EN_GPIO_Port, I2C1_EN_Pin, GPIO_PIN_RESET);
    APPL_data.modbus_size = send[1];
    for (i = 0; i < APPL_data.modbus_size; i++)
    {
        APPL_data.modsend[i] = send[i + 2];
    }
    // APPL_CMD_printf("I2C = 0x%x\r\n", ret);
    return ret;
}
//***** End of File *****//