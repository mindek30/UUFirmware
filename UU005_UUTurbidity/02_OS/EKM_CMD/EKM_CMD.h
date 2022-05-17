/*
 * EKM_OS.h
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 * 
 *  Update on: Aug 15, 2021
 *      Update :
 *          1. int EKM_CMD_LineProcess(char *pcCmdLine, EKM_CMD_tCmdLineEntry *psCmdEntry, const char varsplit)
 *             Add varsplit for split of Parameters
 *      Remove :
 *          1. void EKM_OS_CMD_Registor(EKM_CMD_tCmdLineEntry * EKM_OS_CMD);
 *             This function is unnecessary for registor Command function.
 *          2. EKM_CMD_tCmdLineEntry *EKM_OS_CMD_Link = ((void *)0);
 *             This Var use is unnecessary same (1.)
 * 
 */

#ifndef EKM_CMD_H_
#define EKM_CMD_H_

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \addtogroup cmdline_api
//! @{
//
//*****************************************************************************
#define EKM_CMD_BUFFER_SIZE  256

//! Defines the value that is returned if the command is not found.
#define EKM_CMD_LINE_BAD_CMD         (-1) 

//! Defines the value that is returned if there are too many arguments.
#define EKM_CMD_LINE_TOO_MANY_ARGS   (-2)

//! Defines the value that is returned if there are too few arguments.
#define EKM_CMD_LINE_TOO_FEW_ARGS   (-3)

//! Defines the value that is returned if an argument is invalid.
#define EKM_CMD_LINE_INVALID_ARG   (-4)

//! Defines the value that is maximum argument.
#ifndef EKM_CMD_LINE_MAX_ARGS
#define EKM_CMD_LINE_MAX_ARGS 8
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

/////////////////////////////////// CMD SESSION //////////////////////////////////

// Command line function callback type.
typedef int (*EKM_CMD_pfnCmdLine)(int argc, char *argv[]);

//! Structure for an entry in the command list table.
typedef struct EKM_CMD_tCmdLineEntry
{
    const char *EKM_CMD_pcCmd;
    EKM_CMD_pfnCmdLine EKM_CMD_pfnCmd;
    const char *EKM_CMD_pcHelp;
    struct EKM_CMD_tCmdLineEntry * next;
} EKM_CMD_tCmdLineEntry;

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************

extern int EKM_CMD_RXPeek(unsigned char *g_pcUARTRxBuffer, unsigned char ucChar, int size);
extern int EKM_CMD_LineProcess(char *pcCmdLine, EKM_CMD_tCmdLineEntry *psCmdEntry, char varspilt);

#ifdef __cplusplus
}
#endif

#endif /* EKM_CMD_H_ */