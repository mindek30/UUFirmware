/*
 * APPL_DMA_LTE.h
 *
 * Created: 22/4/2022 1:46:12
 *  Author: Eakawit_kor
 */ 


#ifndef APPL_DMA_LTE_H_
#define APPL_DMA_LTE_H_

#include "EKM_Buffer.h"

//! Structure for an entry in the command list table.
//! Structure for an entry in the command list table.
typedef struct APPL_DMA_LTE_type
{
    const char *Command;
    uint8_t Return_Stage;
} APPL_DMA_LTE_t;



//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern EKM_Buffer_t APPL_DMA_LTE_Buffer;
extern APPL_DMA_LTE_t APPL_DMA_LTE_Step_Init[15];
extern void APPL_DMA_LTE_Init(void);
extern void APPL_DMA_LTE_SIM7000G_Init(void);
extern void APPL_DMA_LTE_MQTT_PUBLISH(const char *MQTT_Topic, const char *format, ...);
extern void APPL_DMA_LTE_printf(const char *format, ...);
extern void APPL_DMA_LTE_LineProcess(void);


#endif /* APPL_DMA_LTE_H_ */