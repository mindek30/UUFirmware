/*
 * Appl_Data.h
 *
 * Created: 22/4/2022 1:46:12
 *  Author: Eakawit_kor
 */ 


#ifndef APPL_DATA_H_
#define APPL_DATA_H_

#define AT24AA02_Page0          (0x50)
#define AT24AA02_Page1          (0x51)
#define AT24AA02_Page2          (0x52)
#define AT24AA02_Page3          (0x53)
#define AT24AA02_Page4          (0x54)
#define AT24AA02_Page5          (0x55)
#define AT24AA02_Page6          (0x56)
#define AT24AA02_Page7          (0x57)

#define APPL_DATA_ID0_ADD        0

#define APPL_DATA_Modbus        255
#define APPL_DATA_Modbus_ADD    APPL_DATA_ID0_ADD + 2

//! Structure for an entry in the command list table.
typedef struct APPL_DATA
{
  uint8_t IO;
  uint16_t deviceID;
  uint16_t batteryLv;
  uint8_t modsend[APPL_DATA_Modbus];
  uint8_t modbus_size;
} APPL_DATA_t;

extern APPL_DATA_t APPL_data;
//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern void APPL_Data_Init(void);
extern uint8_t APPL_Data_ID_Set(uint16_t data);
extern uint8_t APPL_Data_ID_Get(void);
extern uint8_t APPL_Data_MODBUS_Set(uint8_t *data, uint8_t len);
extern uint8_t APPL_Data_MODBUS_Get(void);
extern uint8_t APPL_Data_RS485_Send(void);
extern uint8_t APPL_Data_RS485_Set(uint8_t data);
extern void APPL_Data_MODBUS_Task(void);


#endif /* APPL_DATA_H_ */