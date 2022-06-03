/*
 * APPL_DMA_OS.h
 *
 * Created: 19/4/2022 22:28:18
 *  Author: Eakawit_kor
 */ 


#ifndef APPL_DMA_OS_H_
#define APPL_DMA_OS_H_


#include "EKM_Buffer.h"
#include "EKM_Modbus.h"
#include "APPL_DMA_RS485.h"
#include "APPL_DMA_IOCON.h"
#include "APPL_DMA_LTE.h"

extern void APPL_DMA_OS_LEDTask(uint8_t onff);
extern void APPL_DMA_OS_Init(void);
extern void APPL_DMA_OS_10ms(void);

#endif /* APPL_DMA_OS_H_ */