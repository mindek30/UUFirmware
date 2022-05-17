/*
 * EKM_Modbus.h
 *
 *  Created on: Jan 14, 2016
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef EKM_MODBUS_H_
#define EKM_MODBUS_H_

#ifdef __cplusplus
extern "C"
{
#endif

//! Structure for an entry in the command list table.

typedef struct EKM_MODBUS
{
  uint8_t len; // data inbus
  uint8_t Maxsize;
  uint8_t *MODdata;
  union
  {
    struct
    {
      uint16_t HCRC : 8;
      uint16_t LCRC : 8;
    };
    uint16_t crc;
  };

} EKM_MODBUS_t;


typedef union Bit16ToBit8
{
  struct
  {
    uint16_t LREG : 8;
    uint16_t HREG : 8;
  };
  uint16_t REG;
} Bit16ToBit8_t;

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************

/////////////////////////////////// Modbus SESSION //////////////////////////////////
extern void EKM_MODBUS_Setup(EKM_MODBUS_t *pdata, uint8_t *Moddata, uint8_t maxsize);
extern void EKM_MODBUS_DataClear(EKM_MODBUS_t *pdata);
extern void EKM_MODBUS_Receiver_Set(EKM_MODBUS_t *pdata, uint8_t dat);
extern uint16_t EKM_MODBUS_CRC_Encode(EKM_MODBUS_t *pdata);
extern uint8_t EKM_MODBUS_CRC_Decode(EKM_MODBUS_t *pdata);

#ifdef __cplusplus
}
#endif

#endif /* EKM_MODBUS_H_ */