/*
 * Appl_LTE.h
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_LTE_H_
#define APPL_LTE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "EKM_Buffer.h"

//! Structure for an entry in the command list table.

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern EKM_Buffer_t APPL_LTE_Buffer;
extern int APPL_LTE_RUN;
extern int APPL_LTE_SEND_RUN;
extern void APPL_LTE_Init(void);
extern void APPL_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...);
extern void APPL_LTE_MODBUS_MQTT_PUBLISH(const char *MQTT_Topic, EKM_MODBUS_t *tmp);
extern void APPL_LTE_printf(const char *format, ...);
extern void APPL_LTE_LineProcess(void);

#ifdef __cplusplus
}
#endif

#endif /* APPL_LTE_H_ */