/*
 * Appl_Task.h
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_TASK_H_
#define APPL_TASK_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/
typedef struct Appl_Task
{
  int Mod1;
  int Mod2;
  bool LAMP_Onoff;
  bool TurnON_PUMP;

  uint16_t Turbidity_Driver;

  uint16_t Turb_High;
  uint16_t Turb_Low;
  
  uint16_t GAIN_A;
  uint16_t GAIN_B;
  
  uint16_t RAW_ADC_Ch0;
  uint16_t RAW_ADC_Ch1;
  
}Appl_Task_t;


//! Structure for an entry in the command list table.

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern Appl_Task_t APPL_Task_d;
extern void APPL_Task_Init(void);
extern void APPL_Task_10ms(void);
extern void APPL_Task_LED(void);
/////////////////////////////////// Function //////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* APPL_TASK_H_ */