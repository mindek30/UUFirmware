/*
 * TestSerial.h
 *
 *  Created on: Jan 14, 2020
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef TESTSERIAL_H_
#define TESTSERIAL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

extern int SerialRW(HANDLE hComm, char * xr, const char * format, ... );


#ifdef __cplusplus
}
#endif

#endif /* TESTSERIAL_H_ */