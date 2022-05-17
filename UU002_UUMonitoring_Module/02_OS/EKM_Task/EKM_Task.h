/*
 * EKM_Task.h
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef EKM_TASK_H_
#define EKM_TASK_H_

#ifdef __cplusplus
extern "C"
{
#endif

/////////////////////////////////// OS SESSION //////////////////////////////////

// Command line function callback type.
typedef void (*EKM_Task_Run)(void);

//! Structure for an entry in the command list table.
typedef struct EKM_Task_tTask
{
    uint8_t EKM_Task_ID;
    uint8_t EKM_Task_Counter;
    uint8_t EKM_Task_Compare;
    union {
        struct 
        {
            uint8_t EKM_Task_Enable: 1;
            uint8_t EKM_Task_Ready: 1;
            uint8_t EKM_Task_Stage: 6;
        };
        uint8_t EKM_Task_Para;
    };
    EKM_Task_Run EKM_Task_tRun; 
    struct EKM_Task_tTask * next;     
} EKM_Task_tTask;


/////////////////////////////////// Task SESSION //////////////////////////////////

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern void EKM_Task_Registor(EKM_Task_tTask * EKM_OS_RUNt);
extern void EKM_Task_Start(void);

#ifdef __cplusplus
}
#endif

#endif /* EKM_TASK_H_ */