/*
 * APPL_DAC80502.c
 *
 *  Created on: Oct 18, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 *
 */

#include "main.h"

void APPL_DAC80502_init(void);
HAL_StatusTypeDef APPL_DAC80502_write(uint8_t regAddr, uint16_t data);

/*******************************************************************************
 * Function: APPL_DAC80502_init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_DAC80502_init(void)
{

}

HAL_StatusTypeDef APPL_DAC80502_write(uint8_t regAddr, uint16_t data)
{
  HAL_StatusTypeDef ret;
  Bit16ToBit8_t temp;
  uint8_t send[3];
  temp.REG = data;
  send[0] = regAddr;
  send[1] = temp.HREG;
  send[2] = temp.LREG;
  APPL_CMD_printf("Send = %02X,%02X,%02X\r\n", send[0], send[1], send[2]);
  ret = HAL_I2C_Master_Transmit(&hi2c1, APPL_DAC80502_ADD, send, 3, 100);
  return ret;
}
// END OF FILE