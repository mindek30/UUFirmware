/*
 * Appl_Module.h
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_MODULE_H_
#define APPL_MODULE_H_

#ifdef __cplusplus
extern "C"
{
#endif

//! Structure for an entry in the command list table.


//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern EKM_MODBUS_t APPL_Module_MODBUS_RX;
extern uint8_t APPL_Module_EN_INTERVAL;
extern uint8_t APPL_Module_MODBUS_Send_Semaphome;

extern void APPL_Module_Init(void);
extern void APPL_Module_printf(const char *format, ...);
extern void APPL_Module_MODBUS_print(EKM_MODBUS_t *tmp);
extern void APPL_Module_USB_MODBUS_print(EKM_MODBUS_t *tmp);
extern void APPL_Module_LineProcess(void);


#ifdef __cplusplus
}
#endif

#endif /* APPL_MODULE_H_ */