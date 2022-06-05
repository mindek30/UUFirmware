/*
 * EKM_Buffer.h
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 *
 *  Update on: 03 Jun, 2022
 *      Update :
 *          1. Clear bug increment from EKM_Buffer_Index_Increment function.
 *          2. Clear bug increment from EKM_Buffer_Set_Muti, EKM_Buffer_Get_Muti.
 *          3. Clear bug form EKM_Buffer_Index_Decrement.
 *      Remove :
 *          1. Comment
 *      Add :
 *          1. EKM_Buffer_readline
 *          2. EKM_Buffer_Raw_Reset
 *          3. EKM_Buffer_Raw_set
 *  
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
    uint8_t SizeMax;
    uint8_t full;
    int8_t LineCount;
    uint8_t *buffer;
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
extern uint8_t EKM_Buffer_Raw_Reset(uint8_t *c, uint8_t *Raw_pointer, uint8_t Size);
extern uint8_t EKM_Buffer_Raw_set(uint8_t *c, uint8_t *Size, uint8_t *Raw_pointer, uint8_t buf);
extern uint8_t EKM_Buffer_readline(EKM_Buffer_t *tmp, uint8_t *readline, const char *varsplit);

#ifdef __cplusplus
}
#endif

#endif /* EKM_BUFFER_H_ */