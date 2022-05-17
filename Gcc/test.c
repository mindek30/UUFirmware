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
//#include "EKM_Buffer.h"

/*******************************************************************************
 *
 * DEFINE
 *
 ******************************************************************************/

//! Structure for an entry in the command list table.
typedef struct ring_t
{
    uint8_t read;
    uint8_t write;
    uint8_t Size;
    uint8_t SizeMax;
    uint8_t full;
    int8_t LineCount;
    uint8_t *buffer;
} EKM_Buffer_t;

/*******************************************************************************
 *
 * GOBAL VARIABLE
 *
 ******************************************************************************/
/////////////////////////////////// BUFFER SESSION //////////////////////////////////

void EKM_Buffer_Setup(EKM_Buffer_t *tmp, uint8_t Size, uint8_t *buffer);
void EKM_Buffer_Reset(EKM_Buffer_t *tmp);
static void EKM_Buffer_Index_Increment(EKM_Buffer_t *tmp);
static void EKM_Buffer_Index_Decrement(EKM_Buffer_t *tmp);
uint8_t EKM_Buffer_Element(EKM_Buffer_t *tmp);
uint8_t EKM_Buffer_Set(EKM_Buffer_t *tmp, uint8_t c);
uint8_t EKM_Buffer_Get(EKM_Buffer_t *tmp, uint8_t *c);
uint8_t EKM_Buffer_Set_Muti(EKM_Buffer_t *tmp, uint8_t *buffer, uint8_t N);
uint8_t EKM_Buffer_Get_Muti(EKM_Buffer_t *tmp, uint8_t *buffer, uint8_t N);
uint8_t EKM_Buffer_Raw_Reset(uint8_t *c, uint8_t *Raw_pointer, uint8_t Size);
uint8_t EKM_Buffer_Raw_set(uint8_t *c, uint8_t *Size, uint8_t *Raw_pointer, uint8_t buf);

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
    if (tmp == ((void *)0))
        return;
    tmp->buffer = buffer;
    tmp->SizeMax = Size;
    tmp->Size = 0;
    tmp->LineCount = 0;
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
    if (tmp == ((void *)0))
        return;
    tmp->read = 0;
    tmp->write = 0;
    tmp->full = 0;
    tmp->Size = 0;
    tmp->LineCount = 0;
    memset(tmp->buffer, 0, tmp->SizeMax * sizeof(uint8_t));
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
    if (tmp == ((void *)0))
        return;
    if (tmp->full == 1)
    {
        tmp->read = (tmp->read + 1) % tmp->SizeMax;
    }
    tmp->write = (tmp->write + 1) % tmp->SizeMax;
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
    if (tmp == ((void *)0))
        return;
    tmp->full = 0;
    tmp->read = (tmp->read + 1) % tmp->SizeMax;
    tmp->Size = EKM_Buffer_Element(tmp);
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
    if (tmp == ((void *)0))
        return 1;
    if (((tmp->full == 0) && (tmp->write == tmp->read)) == 0)
    {
        *c = tmp->buffer[tmp->read];
        EKM_Buffer_Index_Decrement(tmp);
        return 0;
    }
    return 1;
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
    if (tmp == ((void *)0))
        return 0;
    for (int i = 0; i < N; i++)
    {
        if (((tmp->full == 0) && (tmp->write == tmp->read)) == 0)
        {
            buffer[i] = tmp->buffer[tmp->read];
            EKM_Buffer_Index_Decrement(tmp);
            tmp->Size = EKM_Buffer_Element(tmp);
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
uint8_t EKM_Buffer_Set(EKM_Buffer_t *tmp, uint8_t c)
{
    if (tmp == ((void *)0))
        return 1;
    if (tmp->full == 0)
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
    if (tmp == ((void *)0))
        return 1;
    for (int i = 0; i < N; i++)
    {
        if (tmp->full == 0)
        {
            tmp->buffer[tmp->write] = buffer[i];
            EKM_Buffer_Index_Increment(tmp);
            tmp->Size = EKM_Buffer_Element(tmp);
        }
        else
        {
            return N;
        }
    }
    return 0;
}

/*******************************************************************************
 * Function: EKM_Buffer_Element
 *
 * Parameters:      - tmp       : pointer for Buffer type use in process read line.
 * Returned value:  - size      : size of buffer.
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t EKM_Buffer_Element(EKM_Buffer_t *tmp)
{
    uint8_t size = tmp->Size;
    if (tmp == ((void *)0))
        return 0;
    if ((tmp->full) == 0)
    {
        if (tmp->write >= tmp->read)
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

/*******************************************************************************
 * Function: EKM_Buffer_Raw_Reset
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t EKM_Buffer_Raw_Reset(uint8_t *c, uint8_t *Raw_pointer, uint8_t Size)
{
    if (c == ((void *)0))
        return 1;
    if (Raw_pointer == ((void *)0))
        return 1;
    *Raw_pointer = 0;
    memset(c, 0, Size);
    return 0;
}

/*******************************************************************************
 * Function: EKM_Buffer_Raw_set
 *
 * Parameters:      - tmp       : pointer for Buffer type use in process read line.
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint8_t EKM_Buffer_Raw_set(uint8_t *c, uint8_t *Size, uint8_t *Raw_pointer, uint8_t buf)
{
    if (c == ((void *)0))
        return 1;
    if (Size == ((void *)0))
        return 1;
    if (Raw_pointer == ((void *)0))
        return 1;

    if (*Raw_pointer < *Size)
    {
        c[*Raw_pointer++] = buf;
    }
    else
    {
        EKM_Buffer_Raw_Reset(c, Raw_pointer, *Size);
        c[*Raw_pointer++] = buf;
    }
    return 0;
}

/*******************************************************************************
 * Function: EKM_Buffer_readline
 *
 * Parameters:      - tmp       : pointer for Buffer type use in process read line.
 *                  - readline  : pointer for get line data.
 *                  - varsplit  : char for end line, use for since 2 character.
 * Returned value:  - 1         : continue line.
 *                  - 0         : last line.
 *
 * Description: This's read line for buffer, Please use EKM_Buffer type for get data
 *              and check run this function time for processing.
 *
 * Calling:
 ******************************************************************************/
uint8_t EKM_Buffer_readline(EKM_Buffer_t *tmp, uint8_t *readline, const char *varsplit)
{
    if (tmp == ((void *)0))
        return 1;
    if (readline == ((void *)0))
        return 1;

    uint8_t p = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t linecount = 0;
    uint8_t firstLine = 0;
    uint8_t vs = strlen(varsplit);
    uint8_t vb = 0;
    uint8_t index_read = tmp->read;
    uint8_t Buffer_temp[tmp->SizeMax];
    tmp->LineCount = 0;

    memset(Buffer_temp, 0, tmp->SizeMax);
    for (i = 0; i < EKM_Buffer_Element(tmp); i++)
    {
        uint8_t t = 0;
        Buffer_temp[i] = tmp->buffer[index_read];
        index_read = (index_read + 1) % tmp->SizeMax;
        if (Buffer_temp[i] == varsplit[vs - 1])
        {
            t = 1;
            for (j = vs; j > 0; j--)
            {
                if (Buffer_temp[i + j - 2] != varsplit[j - 1])
                {
                    t = 0;
                    break;
                }
            }
        }
        if (t == 1)
        {
            if (firstLine == 0)
                firstLine = i;
        }
        tmp->LineCount += t;
    }

    if (tmp->LineCount > 0)
    {
        EKM_Buffer_Get_Muti(tmp, readline, firstLine + vs - 1);
        vb = strlen(readline) - 1;
        for (i = 0; i < vb; i++)
        {
            if (i > vb - vs)
            {
                readline[i] = 0;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

#define test_buffer_max 255

void chartohex(uint8_t *x)
{
    int i;
    for (i = 0; i < strlen((char *)x); i++)
    {
        printf("pos[%d]:%02X ", i, x[i]);
    }
}

int main()
{
    int8_t line = 1;
    uint8_t xx[] = "AT\r\r\n\r\nOK\r\naa";
    // uint8_t xx[] = "ATasdfasefe\r\n";
    EKM_Buffer_t test;
    uint8_t test_buffer[test_buffer_max];
    uint8_t test_buffer_r[test_buffer_max];
    EKM_Buffer_Setup(&test, test_buffer_max, test_buffer);
    EKM_Buffer_Set_Muti(&test, xx, strlen(xx));
    printf("Hello World\r\n");
    memset(test_buffer_r, 0, test_buffer_max);
    while (EKM_Buffer_readline(&test, test_buffer_r, "\r\n"))
    {
        printf("\n------------------------\n");
        chartohex(test_buffer_r);
        printf("\n%s\n%d", test_buffer_r, line);
        memset(test_buffer_r, 0, test_buffer_max);
        printf("\n------------------------\n");
    }
    chartohex(test.buffer);
    memset(test_buffer_r, 0, test_buffer_max);
    while (EKM_Buffer_readline(&test, test_buffer_r, "\r\n"))
    {
        printf("\n------------------------\n");
        chartohex(test_buffer_r);
        printf("\n%s\n%d", test_buffer_r, line);
        memset(test_buffer_r, 0, test_buffer_max);
        printf("\n------------------------\n");
    }
    
    return 0;
}
