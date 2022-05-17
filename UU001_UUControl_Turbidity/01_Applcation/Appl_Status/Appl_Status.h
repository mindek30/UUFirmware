/*
 * Appl_Status.h
 *
 *  Created on: July 1, 2021
 *      Author: Eakawit
 *      Email: mindek30@gmail.com
 */

#ifndef APPL_STATUS_H_
#define APPL_STATUS_H_

#ifdef __cplusplus
extern "C"
{
#endif

    //! Structure for an entry in the command list table.
    typedef struct APPL_STATUS
    {
        uint16_t IOT_ID;
        uint16_t MQTT_PORT;
        uint8_t MQTT_IP0;
        uint8_t MQTT_IP1;
        uint8_t MQTT_IP2;
        uint8_t MQTT_IP3;
        EKM_MODBUS_t *MODBUS_SEND;
        char MQTT_USER[8];
        char MQTT_PASS[8];
        //Temperature
        union
        {
            uint16_t Temperature;
            struct
            {
                uint16_t TemperH : 8;
                uint16_t TemperL : 8;
            };
        };
        // Humidi
        union
        {
            uint16_t Humidity;
            struct
            {
                uint16_t HumidH : 8;
                uint16_t HumidL : 8;
            };
        };

        uint16_t Batt;
    } APPL_STATUS_t;

    //*****************************************************************************
    // Prototypes for the APIs.
    //*****************************************************************************
    extern APPL_STATUS_t APPL_StatusS;
    extern void APPL_Status_Init(void);
    extern void APPL_Status_LED_Toogle(void);
    extern void APPL_Status_Battery(APPL_STATUS_t *pdata);
    extern void APPL_Status_TempAndHumi(APPL_STATUS_t *pdata);

#ifdef __cplusplus
}
#endif

#endif /* APPL_STATUS_H_ */