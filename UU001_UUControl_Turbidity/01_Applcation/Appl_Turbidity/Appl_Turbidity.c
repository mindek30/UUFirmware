/*
 * Appl_Turbidity.c
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 *
 */

/*******************************************************************************
 *
 * INCLUDE
 *
 ******************************************************************************/
#include "main.h"
#include "Appl_USBService.h"

/*******************************************************************************
 *
 * DEFINE
 *
 ******************************************************************************/
//#define APPL_Turbidity_TimmerMAX 1800 // Half Hour
#define APPL_Turbidity_TimmerMAX 900 // 15 minute
//#define APPL_Turbidity_TimmerMAX 300     // 5 minute
#define APPL_Turbidity_Pump_TimmerMAX 120 // Pump run 90 Sec
#define APPL_Turbidity_MesTimmerMAX 15    // wait to get run 90 Sec
#define APPL_Turbidity_TimmerCounterMax APPL_Turbidity_TimmerMAX - (APPL_Turbidity_Pump_TimmerMAX + APPL_Turbidity_MesTimmerMAX)
/*******************************************************************************
 *
 * GOBAL VARIABLE
 *
 ******************************************************************************/
APPL_Turbidity_t APPL_Turbidity_s;
EKM_MODBUS_t APPL_Turbidity_tmp;
uint8_t APPL_Turbidity_Buffer[20];

void APPL_Turbidity_Init(void);
void APPL_Turbidity_Pump(void);
void APPL_Turbidity_Measurement(void);
void APPL_Turbidity_GETMeasurement(void);
void APPL_Turbidity_Run(void);
uint32_t APPL_Turbidity_uVolt(uint16_t adcValueX);

/*******************************************************************************
 *
 * PRIVATE VARIABLE
 *
 ******************************************************************************/

/*******************************************************************************
 * Function: APPL_Turbidity_Init
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Turbidity_Init(void)
{
  APPL_Turbidity_s.timemer = 0;
  EKM_MODBUS_Setup(&APPL_Turbidity_tmp, APPL_Turbidity_Buffer, 10);
  EKM_MODBUS_Receiver_Set(&APPL_Turbidity_tmp, 0x01);
  EKM_MODBUS_Receiver_Set(&APPL_Turbidity_tmp, 0x03);
  EKM_MODBUS_Receiver_Set(&APPL_Turbidity_tmp, 0x00);
  EKM_MODBUS_Receiver_Set(&APPL_Turbidity_tmp, 0x02);
  EKM_MODBUS_Receiver_Set(&APPL_Turbidity_tmp, 0x00);
  EKM_MODBUS_Receiver_Set(&APPL_Turbidity_tmp, 0x02);
  EKM_MODBUS_CRC_Encode(&APPL_Turbidity_tmp);
  APPL_Turbidity_s.Turbidity = &APPL_Turbidity_tmp;
}

/*******************************************************************************
 * Function: APPL_Turbidity_Pump
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Turbidity_Pump(void)
{
  APPL_Turbidity_s.pumptimmer++;
  if (APPL_Turbidity_s.pumptimmer == APPL_Turbidity_Pump_TimmerMAX)
  {
    APPL_IOCON_s.OUT0 = 0;
    APPL_Turbidity_s.Stage++;
    APPL_Turbidity_s.pumptimmer = 0;
  }
  else
  {
    APPL_IOCON_s.OUT0 = 1;
  }
}

/*******************************************************************************
 * Function: APPL_Turbidity_Measurement
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Turbidity_Measurement(void)
{
  APPL_Turbidity_s.Mestimmer++;
  if (APPL_Turbidity_s.Mestimmer == APPL_Turbidity_MesTimmerMAX)
  {
    APPL_IOCON_MODBUS_print(&APPL_Turbidity_tmp);
    APPL_LTE_MQTT_PUBLISH("TUB", "%04X:%u", APPL_IOCON_s.ANIN3, APPL_Turbidity_uVolt(APPL_IOCON_s.ANIN3));
    APPL_USB_printf("Turbidity Function %04X : %u uV\r\n", APPL_IOCON_s.ANIN3, APPL_Turbidity_uVolt(APPL_IOCON_s.ANIN3));
    APPL_Turbidity_s.Stage++;
    APPL_Turbidity_s.Mestimmer = 0;
  }
}

/*******************************************************************************
 * Function: APPL_Turbidity_Run
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Turbidity_Run(void)
{
  // APPL_USB_printf("Stage %d\r\n",APPL_Turbidity_s.Stage);
  switch (APPL_Turbidity_s.Stage)
  {
  case 0:
  {
    APPL_Turbidity_s.timemer++;
    if (APPL_Turbidity_s.timemer >= APPL_Turbidity_TimmerCounterMax)
    {
      APPL_USB_printf("Turbidity Function Start\r\n");
      APPL_Turbidity_s.timemer = 0;
      APPL_Turbidity_s.Stage++;
    }
    break;
  }
  case 1:
  {
    APPL_USB_printf("Turbidity Function pump on\r\n");
    APPL_Turbidity_Pump();
    break;
  }
  case 2:
  {
    APPL_Turbidity_Measurement();
    break;
  }
  default:
  {
    //APPL_USB_printf("Turbidity Function %04X : %u uV\r\n", APPL_IOCON_s.ANIN3, APPL_Turbidity_uVolt(APPL_IOCON_s.ANIN3));
    APPL_USB_printf("Turbidity Function Done\r\n");
    APPL_Turbidity_s.Stage = 0;
    break;
  }
  }
}


/*******************************************************************************
 * Function: APPL_Turbidity_GETMeasurement
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
void APPL_Turbidity_GETMeasurement(void)
{
    APPL_IOCON_MODBUS_print(&APPL_Turbidity_tmp);
}

/*******************************************************************************
 * Function: APPL_Turbidity_uVolt
 *
 * Parameters:      -
 * Returned value:  -
 *
 * Description:
 *
 * Calling:
 ******************************************************************************/
uint32_t APPL_Turbidity_uVolt(uint16_t adcValueX)
{
  //return (adcValueX * 10240000 / 65536);
  return (adcValueX * 156);
}

// END OF FILE