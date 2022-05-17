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


  //! Structure for an entry in the command list table.

  //*****************************************************************************
  // Prototypes for the APIs.
  //*****************************************************************************
  extern void APPL_Task_Init(void);
  extern void APPL_Task_10ms(void);
  /////////////////////////////////// Function //////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* APPL_TASK_H_ */