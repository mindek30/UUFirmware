/*
 * Appl_IOCon.c
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
#define APPL_IOCON_BUFFER_SIZE 255

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
APPL_IOCON_t APPL_IOCON_s;
EKM_MODBUS_t APPL_IOCON_MODBUS_RX;
uint8_t APPL_IOCON_RX_Buf_d[APPL_IOCON_BUFFER_SIZE];
uint8_t APPL_IOCON_EN_INTERVAL = 0;
uint8_t APPL_IOCON_MODBUS_Send_Semaphome = 0;

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/
uint16_t APPL_IOCON_Init(void);
uint16_t APPL_IOCON_Get(APPL_IOCON_t *pdata);
uint16_t APPL_IOCON_Set(APPL_IOCON_t *pdata);
uint16_t APPL_IOCON_ADC_Get(APPL_IOCON_t *pdata);
uint16_t APPL_IOCON_ADC_SPI_GET(uint8_t RegAddH, uint8_t RegAddL);
uint16_t APPL_IOCON_ADC_SPI_SET(uint8_t RegAddH, uint8_t RegAddL);

void APPL_IOCON_MODBUS_print(EKM_MODBUS_t *tmp);
void APPL_IOCON_USB_MODBUS_print(EKM_MODBUS_t *tmp);
void APPL_IOCON_LineProcess(void);

/*******************************************************************************
* Function: APPL_IOCON_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_IOCON_Init(void)
{
  APPL_IOCON_s.DIO = 0x00;
  APPL_IOCON_s.ANIN0 = 0x0000;
  APPL_IOCON_s.ANIN1 = 0x0000;
  APPL_IOCON_s.ANIN2 = 0x0000;
  APPL_IOCON_s.ANIN3 = 0x0000;
  APPL_IOCON_Set(&APPL_IOCON_s);
  HAL_GPIO_WritePin(SPI1_RESET_GPIO_Port, SPI1_RESET_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SPI1_RESET_GPIO_Port, SPI1_RESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(SPI1_CSADC_GPIO_Port, SPI1_CSADC_Pin, GPIO_PIN_SET);
  // MODBUS Set
  HAL_GPIO_WritePin(USART2_RS485EN_GPIO_Port, USART2_RS485EN_Pin, GPIO_PIN_RESET);
  EKM_MODBUS_Setup(&APPL_IOCON_MODBUS_RX, APPL_IOCON_RX_Buf_d, APPL_IOCON_BUFFER_SIZE);
  EKM_MODBUS_DataClear(&APPL_IOCON_MODBUS_RX);

  APPL_IOCON_ADC_SPI_SET(0x85, 0x00);
  APPL_IOCON_ADC_SPI_SET(0x0B, ADS8688_REG_PLUS25VREF);
  APPL_IOCON_ADC_SPI_SET(0x0D, ADS8688_REG_PLUS25VREF);
  APPL_IOCON_ADC_SPI_SET(0x0F, ADS8688_REG_PLUS25VREF);
  APPL_IOCON_ADC_SPI_SET(0x11, ADS8688_REG_PLUS25VREF);
  APPL_IOCON_ADC_SPI_SET(0x03, 0x00);
  //APPL_IOCON_ADC_SPI_SET(0x05, 0x07);
  
  return 0;
}

/*******************************************************************************
* Function: APPL_IOCON_Get
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_IOCON_Get(APPL_IOCON_t *pdata)
{
  // GPIO Get
  pdata->IN0 = HAL_GPIO_ReadPin(GPIO_DI0_GPIO_Port, GPIO_DI0_Pin);
  pdata->IN1 = HAL_GPIO_ReadPin(GPIO_DI1_GPIO_Port, GPIO_DI1_Pin);
  pdata->IN2 = HAL_GPIO_ReadPin(GPIO_DI2_GPIO_Port, GPIO_DI2_Pin);
  pdata->IN3 = HAL_GPIO_ReadPin(GPIO_DI3_GPIO_Port, GPIO_DI3_Pin);

  return 0;
}

/*******************************************************************************
* Function: APPL_IOCON_Set
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_IOCON_Set(APPL_IOCON_t *pdata)
{
  // GPIO Write
  HAL_GPIO_WritePin(GPIO_DO0_GPIO_Port, GPIO_DO0_Pin, pdata->OUT0);
  HAL_GPIO_WritePin(GPIO_DO1_GPIO_Port, GPIO_DO1_Pin, pdata->OUT1);
  HAL_GPIO_WritePin(GPIO_DO2_GPIO_Port, GPIO_DO2_Pin, pdata->OUT2);
  HAL_GPIO_WritePin(GPIO_DO3_GPIO_Port, GPIO_DO3_Pin, pdata->OUT3);
  return 0;
}

/*******************************************************************************
* Function: APPL_IOCON_ADC_SPI_SET
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_IOCON_ADC_SPI_SET(uint8_t RegAddH, uint8_t RegAddL)
{
  uint8_t APPL_IOCON_REGRead;
  Bit16ToBit8_t dat;
  // Hardware Receive for ADS8684
  HAL_GPIO_WritePin(SPI1_CSADC_GPIO_Port, SPI1_CSADC_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&RegAddH, 1, 100);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&RegAddL, 1, 100);
  HAL_SPI_Receive(&hspi1, &APPL_IOCON_REGRead, 1, 100);
  HAL_GPIO_WritePin(SPI1_CSADC_GPIO_Port, SPI1_CSADC_Pin, GPIO_PIN_SET);

  // Get Data from ADC
  dat.HREG = 0x00;
  dat.LREG = APPL_IOCON_REGRead;

  return dat.REG;
}

/*******************************************************************************
* Function: APPL_IOCON_ADC_SPI_GET
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_IOCON_ADC_SPI_GET(uint8_t RegAddH, uint8_t RegAddL)
{
  uint8_t APPL_IOCON_REGRead[2] = {0, 0};
  Bit16ToBit8_t dat;
  //RegAddH = 0xC0;
  //RegAddL = 0x00;

  // Hardware Receive for ADS8684
  HAL_GPIO_WritePin(SPI1_CSADC_GPIO_Port, SPI1_CSADC_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&RegAddH, 1, 100);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&RegAddL, 1, 100);
  HAL_SPI_Receive(&hspi1, APPL_IOCON_REGRead, 2, 100);
  HAL_GPIO_WritePin(SPI1_CSADC_GPIO_Port, SPI1_CSADC_Pin, GPIO_PIN_SET);

  // Get Data from ADC
  dat.HREG = APPL_IOCON_REGRead[0];
  dat.LREG = APPL_IOCON_REGRead[1];

  return dat.REG;
}

/*******************************************************************************
* Function: APPL_IOCON_ADC_Get
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_IOCON_ADC_Get(APPL_IOCON_t *pdata)
{

  /*
  uint8_t RegAddH = 0xC0;
  uint8_t RegAddL = 0x00;
  uint8_t APPL_IOCON_REGRead[4] = {0,0,0,0};
  
  // Hardware Receive for ADS8684
  HAL_GPIO_WritePin(SPI1_CSADC_GPIO_Port, SPI1_CSADC_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&RegAddH, 1, 100);
  HAL_SPI_Transmit(&hspi1, (uint8_t *)&RegAddL, 1, 100);
  HAL_SPI_Receive(&hspi1, APPL_IOCON_REGRead, 2, 100);
  HAL_GPIO_WritePin(SPI1_CSADC_GPIO_Port, SPI1_CSADC_Pin, GPIO_PIN_SET);

  // Get Data from ADC
  pdata->ANINH0 = APPL_IOCON_REGRead[1];
  pdata->ANINL0 = APPL_IOCON_REGRead[0];
  */
  //APPL_IOCON_ADC_SPI_GET(0x00,0x00);

  pdata->ANIN0 = APPL_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(1), ADS8688_CMD_REG_NOOP);

  pdata->ANIN1 = APPL_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(2), ADS8688_CMD_REG_NOOP);

  pdata->ANIN3 = APPL_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(3), ADS8688_CMD_REG_NOOP);

  pdata->ANIN2 = APPL_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(0), ADS8688_CMD_REG_NOOP);

  return 0;
}

/*******************************************************************************
* Function: APPL_IOCON_MODBUS_print
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_IOCON_MODBUS_print(EKM_MODBUS_t *tmp)
{
  int i;
  uint8_t Ftemp[255];
  for (i = 0; i < tmp->len; i++)
  {
    Ftemp[i] = tmp->MODdata[i];
    APPL_USB_printf("%02x ", Ftemp[i]);
  }
  APPL_USB_printf("%04x\r\n", tmp->crc);
  Ftemp[i++] = tmp->HCRC;
  Ftemp[i] = tmp->LCRC;
  APPL_IOCON_MODBUS_Send_Semaphome = 1;
  HAL_GPIO_WritePin(USART2_RS485EN_GPIO_Port, USART2_RS485EN_Pin, GPIO_PIN_SET);
  for (i = 0; i < tmp->len + 2; i++)
  {
    HAL_UART_Transmit(&huart2, &Ftemp[i], 1, 10);
    HAL_Delay(1);
  }
  HAL_GPIO_WritePin(USART2_RS485EN_GPIO_Port, USART2_RS485EN_Pin, GPIO_PIN_RESET);
  APPL_IOCON_MODBUS_Send_Semaphome = 0;
}

/*******************************************************************************
* Function: APPL_IOCON_USB_MODBUS_print
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_IOCON_USB_MODBUS_print(EKM_MODBUS_t *tmp)
{
  APPL_USB_printf("Modbus %d : ", tmp->len);
  for (int i = 0; i < tmp->len; i++)
  {
    APPL_USB_printf("%02x ", tmp->MODdata[i]);
  }
  APPL_USB_printf("%04x\r\n", tmp->crc);
}

/*******************************************************************************
* Function: APPL_IOCON_LineProcess
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_IOCON_LineProcess(void)
{
  if (APPL_IOCON_EN_INTERVAL == 1)
  {
    //APPL_IOCON_USB_MODBUS_print(&APPL_IOCON_MODBUS_RX);
    if (EKM_MODBUS_CRC_Decode(&APPL_IOCON_MODBUS_RX) == 1)
    {
      APPL_IOCON_USB_MODBUS_print(&APPL_IOCON_MODBUS_RX);
      APPL_LTE_MODBUS_MQTT_PUBLISH("MOB", &APPL_IOCON_MODBUS_RX);
      EKM_MODBUS_DataClear(&APPL_IOCON_MODBUS_RX);
    }
    if (APPL_IOCON_MODBUS_RX.len >= APPL_IOCON_MODBUS_RX.Maxsize)
    {
      EKM_MODBUS_DataClear(&APPL_IOCON_MODBUS_RX);
      APPL_USB_printf("Modbus Clear\r\n");
    }
    APPL_IOCON_EN_INTERVAL = 0;
  }
}

//END OF FILE