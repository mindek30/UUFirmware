/*
 * EKM_Buffer.c
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 *
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
#include "EKM_Buffer.h"

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/

/*******************************************************************************
*
* GOBAL VARIABLE
*
******************************************************************************/
/////////////////////////////////// BUFFER SESSION //////////////////////////////////

void EKM_Buffer_Setup(EKM_Buffer_t *tmp, uint8_t Size, uint8_t *buffer);
void EKM_Buffer_Reset(EKM_Buffer_t *tmp);
uint8_t EKM_Buffer_Set(EKM_Buffer_t *tmp, uint8_t c);
uint8_t EKM_Buffer_Get(EKM_Buffer_t *tmp, uint8_t *c);
uint8_t EKM_Buffer_Element(EKM_Buffer_t * tmp);

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/

/////////////////////////////////// BUFFER SESSION //////////////////////////////////

/*******************************************************************************
* Function: EKM_Buffer_Setup
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void EKM_Buffer_Setup(EKM_Buffer_t *tmp, uint8_t Size, uint8_t *buffer)
{
    if(tmp == ((void *)0)) return;
    tmp->buffer = buffer;
    tmp->Size = Size;
    EKM_Buffer_Reset(tmp);
}

/*******************************************************************************
* Function: EKM_Buffer_Reset
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void EKM_Buffer_Reset(EKM_Buffer_t *tmp)
{
    if(tmp == ((void *)0)) return;
    tmp->read = 0;
    tmp->write = 0;
    tmp->full = 0;
    memset(tmp->buffer, 0, tmp->Size * sizeof(uint8_t));
}

/*******************************************************************************
* Function: EKM_Buffer_Index_Increment
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
static void EKM_Buffer_Index_Increment(EKM_Buffer_t *tmp)
{
    if(tmp == ((void *)0)) return;
    if (tmp->full == 1)
    {
        tmp->read = (tmp->read + 1) % tmp->Size;
    }
    tmp->write = (tmp->write + 1) % tmp->Size;
    tmp->full = (tmp->write == tmp->read);
}

/*******************************************************************************
* Function: EKM_Buffer_Index_Decrement
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
static void EKM_Buffer_Index_Decrement(EKM_Buffer_t *tmp)
{
    if(tmp == ((void *)0)) return;
    tmp->full = 0;
    tmp->read = (tmp->read + 1) % tmp->Size;
}

/*******************************************************************************
* Function: EKM_Buffer_Get_Muti
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t EKM_Buffer_Get_Muti(EKM_Buffer_t *tmp, uint8_t *buffer, uint8_t N)
{
    if(tmp == ((void *)0)) return 0;
    for(int i = 0 ; i < N ; i++){
        if(((tmp->full == 0) && (tmp->write == tmp->read))==0){
            tmp->full = 0;
            buffer[i] = tmp->buffer[tmp->read];
            tmp->read = (tmp->read + 1) % tmp->Size;
            continue;
        }
        return N;
    }
    return 0;
}

/*******************************************************************************
* Function: EKM_Buffer_Set
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t EKM_Buffer_Set(EKM_Buffer_t * tmp, uint8_t c)
{
    if(tmp == ((void *)0)) return 1;
    if(tmp->full == 0)
    {
        tmp->buffer[tmp->write] = c;
        EKM_Buffer_Index_Increment(tmp);
        return 0;
    }
    return 1;
}

/*******************************************************************************
* Function: EKM_Buffer_Set_Muti
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t EKM_Buffer_Set_Muti(EKM_Buffer_t *tmp, uint8_t *buffer, uint8_t N)
{
    if(tmp == ((void *)0)) return 1;
    for(int i = 0 ; i < N ; i++){
        if (tmp->full == 1)
        {
            return N;
        }
        tmp->buffer[tmp->write] = buffer[i];
        tmp->write = (tmp->write + 1) % tmp->Size;
        tmp->full = (tmp->write == tmp->read);
    }
    return 1;
}

/*******************************************************************************
* Function: EKM_Buffer_Get
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t EKM_Buffer_Get(EKM_Buffer_t *tmp, uint8_t *c)
{
    if(tmp == ((void *)0)) return 1;
    if(((tmp->full == 0) && (tmp->write == tmp->read))==0){
        *c = tmp->buffer[tmp->read];
        EKM_Buffer_Index_Decrement(tmp);
        return 0;
    }
    return 1;
}

/*******************************************************************************
* Function: EKM_Buffer_Element
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t EKM_Buffer_Element(EKM_Buffer_t * tmp)
{
	uint8_t size = tmp->Size;
	if(tmp == ((void *)0)) return 0;
    if((tmp->full)==0)
	{
		if(tmp->write >= tmp->read)
		{
			size = (tmp->write - tmp->read);
		}
		else
		{
			size = (tmp->Size + tmp->write - tmp->read);
		}
	}
	return size;
}

//END OF FILE