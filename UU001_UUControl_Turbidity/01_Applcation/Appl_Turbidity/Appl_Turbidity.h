/*
 * Appl_Turbidity.h
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_TURBIDITY_H_
#define APPL_TURBIDITY_H_

#ifdef __cplusplus
extern "C"
{
#endif

    //! Structure for an entry in the command list table.
    typedef struct Turbidity
    {
        uint16_t timemer;
        uint16_t pumptimmer;
        uint8_t Stage;
        uint16_t Mestimmer;
        EKM_MODBUS_t * Turbidity;

    } APPL_Turbidity_t;

    //*****************************************************************************
    // Prototypes for the APIs.
    //*****************************************************************************
    extern APPL_Turbidity_t APPL_Turbidity_s;
    extern void APPL_Turbidity_Init(void);
    extern void APPL_Turbidity_Pump(void);
    extern void APPL_Turbidity_Measurement(void);
    extern void APPL_Turbidity_GETMeasurement(void);
    extern void APPL_Turbidity_Run(void);
    extern uint32_t APPL_Turbidity_uVolt(uint16_t adcValueX);

#ifdef __cplusplus
}
#endif

#endif /* APPL_TURBIDITY_H_ */