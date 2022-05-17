/*
 * Appl_Module.c
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

/*******************************************************************************
 *
 * DEFINE
 *
 ******************************************************************************/
#define APPL_Module_BUFFER_SIZE 255

/*******************************************************************************
 *
 * GOBAL VARIABLE
 *
 ******************************************************************************/
EKM_MODBUS_t APPL_Module_MODBUS_RX;
uint8_t APPL_Module_RX_Buf_d[APPL_Module_BUFFER_SIZE];
uint8_t APPL_Module_EN_INTERVAL = 0;
uint8_t APPL_Module_MODBUS_Send_Semaphome = 0;

void APPL_Module_Init(void);
void APPL_Module_MODBUS_print(EKM_MODBUS_t *tmp);
void APPL_Module_LineProcess(void);
void APPL_Module_CMD_MODBUS_print(EKM_MODBUS_t *tmp);

/*******************************************************************************
 *
 * PRIVATE VARIABLE
 *
 ******************************************************************************/

/*******************************************************************************
 * Function: APPL_Module_Init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Module_Init(void)
{
  HAL_GPIO_WritePin(USART2_DE_GPIO_Port, USART2_DE_Pin, GPIO_PIN_RESET);
  EKM_MODBUS_Setup(&APPL_Module_MODBUS_RX, APPL_Module_RX_Buf_d, APPL_Module_BUFFER_SIZE);
  EKM_MODBUS_DataClear(&APPL_Module_MODBUS_RX);
}

/*******************************************************************************
 * Function: APPL_Module_MODBUS_print
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Module_MODBUS_print(EKM_MODBUS_t *tmp)
{
  int i;
  uint8_t Ftemp[255];
  for (i = 0; i < tmp->len; i++)
  {
    Ftemp[i] = tmp->MODdata[i];
    APPL_CMD_printf("%02x ", Ftemp[i]);
  }
  APPL_CMD_printf("%04x\r\n", tmp->crc);
  Ftemp[i++] = tmp->HCRC;
  Ftemp[i] = tmp->LCRC;
  APPL_Module_MODBUS_Send_Semaphome = 1;
  HAL_GPIO_WritePin(USART2_DE_GPIO_Port, USART2_DE_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  for (i = 0; i < tmp->len + 2; i++)
  {
    HAL_UART_Transmit(&huart2, &Ftemp[i], 1, 100);
    HAL_Delay(1);
  }
  HAL_GPIO_WritePin(USART2_DE_GPIO_Port, USART2_DE_Pin, GPIO_PIN_RESET);
  APPL_Module_MODBUS_Send_Semaphome = 0;
}

/*******************************************************************************
 * Function: APPL_Module_CMD_MODBUS_print
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Module_CMD_MODBUS_print(EKM_MODBUS_t *tmp)
{
  APPL_CMD_printf("Module %d : ", tmp->len);
  for (int i = 0; i < tmp->len; i++)
  {
    APPL_CMD_printf("%02x ", tmp->MODdata[i]);
  }
  APPL_CMD_printf("%04x\r\n", tmp->crc);
}

/*******************************************************************************
 * Function: APPL_Module_LineProcess
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Module_LineProcess(void)
{
  if (APPL_Module_EN_INTERVAL == 1)
  {
     //APPL_Module_CMD_MODBUS_print(&APPL_Module_MODBUS_RX);
    // APPL_CMD_printf("Module Run\r\n");
    if (EKM_MODBUS_CRC_Decode(&APPL_Module_MODBUS_RX) == 1)
    {
      //APPL_Module_CMD_MODBUS_print(&APPL_Module_MODBUS_RX);
      if (APPL_Module_MODBUS_RX.MODdata[0] == 0x01)
      {
        if (APPL_Module_MODBUS_RX.MODdata[1] == 0x03)
        {

          EKM_MODBUS_t tm;
          uint8_t test[20];
          if ((APPL_Module_MODBUS_RX.MODdata[2] == 0x00) &&
              (APPL_Module_MODBUS_RX.MODdata[3] == 0x01))
          {
            Bit32ToBit8_t tmpx;
            tmpx.REG = APPL_ads1115_uVolt(&APPL_Task_d.RAW_ADC_Ch0);
            EKM_MODBUS_Setup(&tm, test, 7);
            EKM_MODBUS_Receiver_Set(&tm, 0x01);
            EKM_MODBUS_Receiver_Set(&tm, 0x03);
            EKM_MODBUS_Receiver_Set(&tm, 0x02);
            EKM_MODBUS_Receiver_Set(&tm, tmpx.HHREG);
            EKM_MODBUS_Receiver_Set(&tm, tmpx.HLREG);
            EKM_MODBUS_Receiver_Set(&tm, tmpx.LHREG);
            EKM_MODBUS_Receiver_Set(&tm, tmpx.LLREG);
            EKM_MODBUS_CRC_Encode(&tm);
            APPL_Module_MODBUS_print(&tm);
            EKM_MODBUS_DataClear(&tm);
          }
          else if ((APPL_Module_MODBUS_RX.MODdata[2] == 0x00) &&
                   (APPL_Module_MODBUS_RX.MODdata[3] == 0x02))
          {
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
          }
        }
      }
      EKM_MODBUS_DataClear(&APPL_Module_MODBUS_RX);
    }
    if (APPL_Module_MODBUS_RX.len >= APPL_Module_MODBUS_RX.Maxsize)
    {
      EKM_MODBUS_DataClear(&APPL_Module_MODBUS_RX);
      APPL_CMD_printf("Module Clear\r\n");
    }
    APPL_Module_EN_INTERVAL = 0;
  }
}

// END OF FILE