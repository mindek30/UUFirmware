/*
 * EKM_Buffer.h
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef EKM_BUFFER_H_
#define EKM_BUFFER_H_

#ifdef __cplusplus
extern "C"
{
#endif

//! Structure for an entry in the command list table.
typedef struct ring_t{
    uint8_t read;
    uint8_t write;
    uint8_t Size;
    uint8_t full;
    uint8_t * buffer;
}EKM_Buffer_t;

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************

/////////////////////////////////// Buffer SESSION //////////////////////////////////
extern void EKM_Buffer_Setup(EKM_Buffer_t *tmp, uint8_t Size, uint8_t *buffer);
extern void EKM_Buffer_Reset(EKM_Buffer_t *tmp);
extern uint8_t EKM_Buffer_Set(EKM_Buffer_t *tmp, uint8_t c);
extern uint8_t EKM_Buffer_Get(EKM_Buffer_t *tmp, uint8_t *c);
extern uint8_t EKM_Buffer_Set_Muti(EKM_Buffer_t *tmp, uint8_t *buffer, uint8_t N);
extern uint8_t EKM_Buffer_Get_Muti(EKM_Buffer_t *tmp, uint8_t *buffer, uint8_t N);
extern uint8_t EKM_Buffer_Element(EKM_Buffer_t * tmp);

#ifdef __cplusplus
}
#endif

#endif /* EKM_BUFFER_H_ */