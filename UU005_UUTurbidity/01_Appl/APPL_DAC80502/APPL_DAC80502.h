/*
 * APPL_DAC80502.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_DAC80502_H
#define APPL_DAC80502_H

#ifdef __cplusplus
extern "C"
{
#endif

#define APPL_DAC80502_ADD 0x92

#define APPL_DAC80502_NOOP 0x00
#define APPL_DAC80502_DEVID 0x01
#define APPL_DAC80502_SYNC 0x02
#define APPL_DAC80502_CONFIG 0x03
#define APPL_DAC80502_GAIN 0x04
#define APPL_DAC80502_TRIGGER 0x05
#define APPL_DAC80502_BRDCAST 0x06
#define APPL_DAC80502_STATUS 0x07
#define APPL_DAC80502_DAC_A 0x08
#define APPL_DAC80502_DAC_B 0x09

    extern void APPL_DAC80502_init(void);
    extern HAL_StatusTypeDef APPL_DAC80502_write(uint8_t regAddr, uint16_t data);

#ifdef __cplusplus
}
#endif

#endif /* APPL_DAC80502_H_ */