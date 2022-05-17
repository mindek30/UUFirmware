/*
 * EKM_Buffer.c
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 *
 */

/*******************************************************************************
*
* INCLUDE
*
******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "EKM_Modbus.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
/////////////////////////////////// Modbus SESSION //////////////////////////////////
void EKM_MODBUS_Setup(EKM_MODBUS_t *pdata, uint8_t *Moddata, uint8_t maxsize);
void EKM_MODBUS_DataClear(EKM_MODBUS_t *pdata);
void EKM_MODBUS_Receiver_Set(EKM_MODBUS_t *pdata, uint8_t dat);
uint16_t EKM_MODBUS_CRC_Encode(EKM_MODBUS_t *pdata);
uint8_t EKM_MODBUS_CRC_Decode(EKM_MODBUS_t *pdata);
/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/

/////////////////////////////////// Modbus SESSION //////////////////////////////////

/*******************************************************************************
* Function: EKM_MODBUS_Setup
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void EKM_MODBUS_Setup(EKM_MODBUS_t *pdata, uint8_t *Moddata, uint8_t maxsize)
{
  pdata->len = 0;
  pdata->Maxsize = maxsize;
  pdata->MODdata = Moddata;
  pdata->crc = 0;
}

/*******************************************************************************
* Function: EKM_MODBUS_DataClear
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void EKM_MODBUS_DataClear(EKM_MODBUS_t *pdata)
{
  for (int c = 0; c < pdata->Maxsize; c++)
  {
    pdata->MODdata[c] = 0;
  }
  pdata->len = 0;
  pdata->crc = 0;
}

/*******************************************************************************
* Function: EKM_MODBUS_Receiver_Set
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void EKM_MODBUS_Receiver_Set(EKM_MODBUS_t *pdata, uint8_t dat)
{
  if (pdata->len < pdata->Maxsize)
  {
    pdata->MODdata[pdata->len++] = dat;
  }
}

/*******************************************************************************
* Function: EKM_MODBUS_Receiver_Get
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t EKM_MODBUS_Receiver_Get(EKM_MODBUS_t *pdata)
{
  uint8_t tmp;
  if (pdata->len >0)
  {
    pdata->len--;
    tmp = pdata->MODdata[pdata->len];
    pdata->MODdata[pdata->len] = 0;
    return tmp;
    
  }
  return 0xFF;
}
/*******************************************************************************
* Function: EKM_MODBUS_CRC_Encode
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t EKM_MODBUS_CRC_Encode(EKM_MODBUS_t *pdata)
{
  uint8_t pos = 0;
  pdata->crc = 0xFFFF;
  for (pos = 0; pos < pdata->len; pos++)
  {
    pdata->crc ^= (uint16_t)pdata->MODdata[pos]; // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--)
    { // Loop over each bit
      if ((pdata->crc & 0x0001) != 0)
      {                   // If the LSB is set
        pdata->crc >>= 1; // Shift right and XOR 0xA001
        pdata->crc ^= 0xA001;
      }
      else                // Else LSB is not set
        pdata->crc >>= 1; // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return pdata->crc;
}

/*******************************************************************************
* Function: EKM_MODBUS_CRC_Decode
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t EKM_MODBUS_CRC_Decode(EKM_MODBUS_t *pdata)
{
  uint8_t pos = 0;
  pdata->crc = 0xFFFF;
  for (pos = 0; pos < (pdata->len - 2); pos++)
  {
    pdata->crc ^= (uint16_t)pdata->MODdata[pos]; // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--)
    { // Loop over each bit
      if ((pdata->crc & 0x0001) != 0)
      {            // If the LSB is set
        pdata->crc >>= 1; // Shift right and XOR 0xA001
        pdata->crc ^= 0xA001;
      }
      else         // Else LSB is not set
        pdata->crc >>= 1; // Just shift right
    }
  }
  if (((uint16_t) pdata->MODdata[pdata->len-2] == pdata->HCRC) &&
  ((uint16_t) pdata->MODdata[pdata->len-1] == pdata->LCRC)){
    EKM_MODBUS_Receiver_Get(pdata);
    EKM_MODBUS_Receiver_Get(pdata);
    return 1;
    }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return 0;
}
//END OF FILE