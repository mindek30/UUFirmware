/*
 * Appl_USBService.h
 *
 *  Created on: Aug 22, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_USBSERVICE_H_
#define APPL_USBSERVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif

//! Structure for an entry in the command list table.


//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************

/////////////////////////////////// Buffer SESSION //////////////////////////////////
extern EKM_Buffer_t APPL_USB_Rx;
extern void APPL_USB_Init(void);
extern void APPL_USB_LineProcess(void);
extern void APPL_USB_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* APPL_USBSERVICE_H_ */