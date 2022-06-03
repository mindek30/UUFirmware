/*
 * APPL_DMA_RS485.h
 *
 * Created: 19/4/2022 22:06:32
 *  Author: Eakawit_kor
 */ 


#ifndef APPL_DMA_RS485_H_
#define APPL_DMA_RS485_H_

#include "EKM_Buffer.h"
#include "EKM_CMD.h"
#include "EKM_Modbus.h"

extern EKM_Buffer_t APPL_DMA_RS48_Rx;

extern void APPL_DMA_RS485_Init(void);
extern void APPL_DMA_RS485_printf(const char *format, ...);
extern void APPL_DMA_RS485_LineProcess(void);


#endif /* APPL_DMA_RS485_H_ */