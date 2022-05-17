/*
 * APPL_DAC80502.c
 *
 *  Created on: Oct 18, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 *
 */

#include "main.h"

int APPL_TPL0401X_write(uint8_t Addr, uint8_t data);

/*******************************************************************************
 * Function: APPL_TPL0401X_write
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
int APPL_TPL0401X_write(uint8_t Addr, uint8_t dataX)
{
  HAL_StatusTypeDef ret;
  uint8_t send[2];
  send[0] = APPL_TPL0401X_NOOP;
  send[1] = dataX;
  ret = HAL_I2C_Master_Transmit(&hi2c1, Addr << 1, send, 2, 100);
  return ret;
}
// END OF FILE