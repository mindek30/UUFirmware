/*
 * EKM_Task.c
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

/*******************************************************************************
*
* INCLUDE
*
******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "EKM_Task.h"



/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/

/////////////////////////////////// Task SESSION //////////////////////////////////
EKM_Task_tTask * EKM_Task_Link = ((void *)0);

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/

/////////////////////////////////// Task SESSION //////////////////////////////////

/*******************************************************************************
* Function: EKM_Task_Registor
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void EKM_Task_Registor(EKM_Task_tTask * EKM_Task_RUNt)
{
    if (EKM_Task_RUNt != ((void *)0))
    {
        if(EKM_Task_Link != ((void *)0)){
            EKM_Task_RUNt->next = EKM_Task_Link->next;
            EKM_Task_Link->next = EKM_Task_RUNt;
        }else{
            EKM_Task_Link = EKM_Task_RUNt;
        }
    }
}

/*******************************************************************************
* Function: EKM_Task_Start
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void EKM_Task_Start(void)
{
    EKM_Task_tTask *pEntry;
    pEntry = EKM_Task_Link;
    while (pEntry != ((void *)0))
    {
        if (pEntry->EKM_Task_Enable == 1)
        {
            pEntry->EKM_Task_Counter++;
        }
        if ((pEntry->EKM_Task_Counter >= pEntry->EKM_Task_Compare) && (pEntry->EKM_Task_tRun != ((void *)0)))
        {
            pEntry->EKM_Task_Counter = 0;
            pEntry->EKM_Task_tRun();
        }
        pEntry = pEntry->next;
    }
}

//END OF FILE