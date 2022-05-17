/*
 * Appl_Task.c
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
 * GOBAL VARIABLE
 *
 ******************************************************************************/
Appl_Task_t APPL_Task_d;

void APPL_Task_Init(void);
void APPL_Task_10ms(void);
void APPL_Task_100ms(void);
void APPL_Task_1s(void);

void APPL_Task_LED(void);
void APPL_Task_MOD(void);
void APPL_Task_MainVoltage_Read(void);
void APPL_Task_LAMP_ON(uint8_t on);

/*******************************************************************************
 *
 * PRIVATE VARIABLE
 *
 ******************************************************************************/
uint8_t APPL_Task_Time_10ms = 0;
uint8_t APPL_Task_Time_100ms = 0;
uint8_t APPL_Task_Time_1s = 0;

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
  // HAL_Delay(1000);

  APPL_CMD_Init();
  APPL_CMD_printf("USART 1 Init\r\n");

  APPL_Module_Init();
  APPL_CMD_printf("Module Init\r\n");

  HAL_GPIO_WritePin(AN_EN_GPIO_Port, AN_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LAMP_EN_GPIO_Port, LAMP_EN_Pin, GPIO_PIN_RESET);
  APPL_CMD_printf("All System Green\r\n");
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
  switch (APPL_Task_Time_10ms++)
  {
  case 10:
  {
    APPL_Task_100ms();
    APPL_Task_LED();
    APPL_Task_Time_10ms = 0;
    break;
  }
  case 4:
  {
    APPL_Task_LAMP_ON(1);
    break;
  }
  case 5:
  {
    APPL_Task_MainVoltage_Read();
    //APPL_Task_LAMP_ON(0);
    break;
  }  
  case 8:
  {
    APPL_Module_LineProcess();
    break;
  }
  case 9:
  {
    APPL_CMD_LineProcess();
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
void APPL_Task_100ms(void)
{
  APPL_Task_Time_100ms++;
  switch (APPL_Task_Time_100ms)
  {
  case 10:
  {
    // APPL_CMD_printf("Module Init\r\n");
    APPL_Task_1s();
    APPL_Task_Time_100ms = 0;
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
  APPL_Task_Time_1s++;
  switch (APPL_Task_Time_1s)
  {
  case 2:
  {
    break;
  }
  case 15:
  {
    APPL_Task_Time_1s = 0;
    break;
  }
  }
}

/*******************************************************************************
 * Function: APPL_Task_LED
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Task_LED(void)
{
  HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
}

/*******************************************************************************
 * Function: APPL_Task_MOD
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Task_MOD(void)
{
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 5);
  APPL_Task_d.Mod1 = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 5);
  APPL_Task_d.Mod2 = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
}

/*******************************************************************************
 * Function: APPL_Task_MainVoltage_Read
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Task_MainVoltage_Read(void)
{
  uint16_t val = 0;
  APPL_ads1115_startSampling(ADS1115_MUX_5); // P = AIN0, N = GND
  while (APPL_ads1115_isBusy());
  APPL_ads1115_readSample(&val);
  APPL_Task_d.RAW_ADC_Ch0 = val;
  
  APPL_ads1115_startSampling(ADS1115_MUX_6); // P = AIN1, N = GND
  while (APPL_ads1115_isBusy());
  APPL_ads1115_readSample(&val);
  APPL_Task_d.RAW_ADC_Ch1 = val;
  
}

/*******************************************************************************
 * Function: APPL_Task_LAMP_ON
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Task_LAMP_ON(uint8_t on)
{
  HAL_GPIO_WritePin(LAMP_EN_GPIO_Port, LAMP_EN_Pin, on);
}

// END OF FILE