/*
 * Appl_IOCon.h
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_IOCON_H_
#define APPL_IOCON_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/
#define ADS8688_CMD_REG(x) (x << 8)
#define ADS8688_CMD_REG_NOOP 0x00
#define ADS8688_CMD_REG_RST 0x85
#define ADS8688_CMD_REG_MAN_CH(chan) (0xC0 | (4 * chan))
#define ADS8688_CMD_REG_MAN_AUX 0xE0
#define ADS8688_CMD_DONT_CARE_BITS 16

#define ADS8688_REG_PLUSMINUS25VREF 0
#define ADS8688_REG_PLUSMINUS125VREF 1
#define ADS8688_REG_PLUSMINUS0625VREF 2
#define ADS8688_REG_PLUS25VREF 5
#define ADS8688_REG_PLUS125VREF 6

#define ADS8688_VREF_MV 4096
#define ADS8688_REALBITS 16
#define ADS8688_MAX_CHANNELS 8

  //! Structure for an entry in the command list table.
  typedef struct APPL_IOCON
  {
    union
    {
      struct
      {
        uint8_t IN0 : 1;
        uint8_t IN1 : 1;
        uint8_t IN2 : 1;
        uint8_t IN3 : 1;
        uint8_t OUT0 : 1;
        uint8_t OUT1 : 1;
        uint8_t OUT2 : 1;
        uint8_t OUT3 : 1;
      };
      struct
      {
        uint8_t DIN : 4;
        uint8_t DOUT : 4;
      };

      uint8_t DIO;
    };

    union
    {
      struct
      {
        uint16_t ANINH0 : 8;
        uint16_t ANINL0 : 8;
      };
      uint16_t ANIN0;
    };

    union
    {
      struct
      {
        uint16_t ANINH1 : 8;
        uint16_t ANINL1 : 8;
      };
      uint16_t ANIN1;
    };

    union
    {
      struct
      {
        uint16_t ANINH2 : 8;
        uint16_t ANINL2 : 8;
      };
      uint16_t ANIN2;
    };

    union
    {
      struct
      {
        uint16_t ANINH3 : 8;
        uint16_t ANINL3 : 8;
      };
      uint16_t ANIN3;
    };

  } APPL_IOCON_t;

  //*****************************************************************************
  // Prototypes for the APIs.
  //*****************************************************************************
  extern APPL_IOCON_t APPL_IOCON_s;
  extern EKM_MODBUS_t APPL_IOCON_MODBUS_RX;
  extern uint8_t APPL_IOCON_EN_INTERVAL;
  extern uint8_t APPL_IOCON_MODBUS_Send_Semaphome;
  /////////////////////////////////// Function //////////////////////////////////
  extern uint16_t APPL_IOCON_Init(void);
  extern uint16_t APPL_IOCON_Get(APPL_IOCON_t *pdata);
  extern uint16_t APPL_IOCON_Set(APPL_IOCON_t *pdata);
  extern uint16_t APPL_IOCON_ADC_Get(APPL_IOCON_t *pdata);

  extern void APPL_IOCON_MODBUS_print(EKM_MODBUS_t *tmp);
  extern void APPL_IOCON_USB_MODBUS_print(EKM_MODBUS_t *tmp);
  extern void APPL_IOCON_LineProcess(void);

#ifdef __cplusplus
}
#endif

#endif /* APPL_IOCON_H_ */