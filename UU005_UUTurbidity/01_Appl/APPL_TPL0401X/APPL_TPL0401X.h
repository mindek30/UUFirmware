/*
 * APPL_TPL0401X.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_TPL0401X_H
#define APPL_TPL0401X_H

#ifdef __cplusplus
extern "C"
{
#endif

#define APPL_TPL0401X_ADD_A 0x2E
#define APPL_TPL0401X_ADD_B 0x3E

#define APPL_TPL0401X_NOOP 0x00

    extern int APPL_TPL0401X_write(uint8_t Addr, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* APPL_TPL0401X_H_ */