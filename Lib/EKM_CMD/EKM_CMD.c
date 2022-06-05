/*
 * EKM_OS.c
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 * 
 * 
 *  Update on: Aug 15, 2021
 *      Update :
 *          1. int EKM_CMD_LineProcess(char *pcCmdLine, EKM_CMD_tCmdLineEntry *psCmdEntry, const char varsplit)
 *             Add varsplit for split of Parameters
 *          2. if (*pcChar == varsplit) // spilt
 *             change from (*pcChar == ' ') to (*pcChar == varsplit) 
 *      Remove :
 *          1. void EKM_OS_CMD_Registor(EKM_CMD_tCmdLineEntry * EKM_OS_CMD);
 *             This function is unnecessary for registor Command function.
 *          2. EKM_CMD_tCmdLineEntry *EKM_OS_CMD_Link = ((void *)0);
 *             This var is unnecessary same (1.)
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
#include "EKM_CMD.h"

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

/*******************************************************************************
*
* PRIVATE VARIABLE
*
******************************************************************************/

/////////////////////////////////// CMD SESSION //////////////////////////////////

EKM_CMD_tCmdLineEntry *EKM_OS_CMD_Link = ((void *)0);
static char *EKM_CMDg_ppcArgv[EKM_CMD_LINE_MAX_ARGS + 1];
int EKM_CMD_LineProcess(char *pcCmdLine, EKM_CMD_tCmdLineEntry *psCmdEntry, char varspilt);


/*******************************************************************************
* Function: EKM_OS_Tunnel_RXPeek
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
int EKM_CMD_RXPeek(unsigned char *g_pcUARTRxBuffer, unsigned char ucChar, int size)
{
    for (int iCount = 0; iCount < size; iCount++)
    {
        if (g_pcUARTRxBuffer[iCount] == ucChar)
        {
            g_pcUARTRxBuffer[iCount] = 0;
            return (iCount);
        }
    }
    return (-1);
}

/*******************************************************************************
* Function: EKM_CMD_LineProcess
*
* Parameters:      - (char) pcCmdLine = string of command
*                  - (EKM_CMD_tCmdLineEntry) psCmdEntry = pointer of struct for function command
*                  - (char) varsplit = char for character split of Parameters 
* Returned value:  int
*
* Description: Update 15/08/2021 00:44 add varsplit to change every command type
*
* Calling:
******************************************************************************/
int EKM_CMD_LineProcess(char *pcCmdLine, EKM_CMD_tCmdLineEntry *psCmdEntry, const char varsplit)
{
    char *pcChar;
    uint_fast8_t ui8Argc;
    bool bFindArg = true;
    ui8Argc = 0;
    pcChar = pcCmdLine;
    while (*pcChar)
    {
        if (*pcChar == varsplit) // spilt 
        {
            
            *pcChar = 0;
            bFindArg = true;
        }
        else
        {
            if (bFindArg)
            {
                if (ui8Argc < EKM_CMD_LINE_MAX_ARGS)
                {
                    EKM_CMDg_ppcArgv[ui8Argc] = pcChar;
                    ui8Argc++;
                    bFindArg = false;
                }
                else
                {
                    return (EKM_CMD_LINE_TOO_MANY_ARGS);
                }
            }
        }
        pcChar++;
    }

    if (ui8Argc)
    {
        while (psCmdEntry != ((void *)0))
        {
            if (!strcmp(EKM_CMDg_ppcArgv[0], psCmdEntry->EKM_CMD_pcCmd) && (psCmdEntry->EKM_CMD_pcCmd != ((void *)0)))
            {
                return (psCmdEntry->EKM_CMD_pfnCmd(ui8Argc, EKM_CMDg_ppcArgv));
            }
            psCmdEntry = psCmdEntry->next;
        }
    }
    return (EKM_CMD_LINE_BAD_CMD);
}

//END OF FILE