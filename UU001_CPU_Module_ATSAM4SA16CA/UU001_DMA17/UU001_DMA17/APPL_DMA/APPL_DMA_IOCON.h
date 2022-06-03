/*
 * APPL_DMA_IOCON.h
 *
 * Created: 20/4/2022 1:51:49
 *  Author: Eakawit_kor
 */ 


#ifndef APPL_DMA_IOCON_H_
#define APPL_DMA_IOCON_H_

/*******************************************************************************
*
* DEFINE
*
******************************************************************************/
#define ADS8688_CMD_REG(x) (x << 8)
#define ADS8688_CMD_REG_NOOP 0x00
#define ADS8688_CMD_REG_RST 0x85
#define ADS8688_CMD_REG_MAN_CH(chan) (0xC0 | (4 * chan))
#define ADS8688_CMD_REG_MAN_AUX 0xE0
#define ADS8688_CMD_DONT_CARE_BITS 16

#define ADS8688_REG_PLUSMINUS25VREF 0
#define ADS8688_REG_PLUSMINUS125VREF 1
#define ADS8688_REG_PLUSMINUS0625VREF 2
#define ADS8688_REG_PLUS25VREF 5
#define ADS8688_REG_PLUS125VREF 6

#define ADS8688_VREF_MV 4096
#define ADS8688_REALBITS 16
#define ADS8688_MAX_CHANNELS 8

//! Structure for an entry in the command list table.
typedef struct APPL_DMA_IOCON
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
	// Humidi
	uint16_t Batt;
	// IO
	union
	{
		struct
		{
			uint8_t IN0 : 1;
			uint8_t IN1 : 1;
			uint8_t IN2 : 1;
			uint8_t IN3 : 1;
			uint8_t OUT0 : 1;
			uint8_t OUT1 : 1;
			uint8_t OUT2 : 1;
			uint8_t OUT3 : 1;
		};
		struct
		{
			uint8_t DIN : 4;
			uint8_t DOUT : 4;
		};

		uint8_t DIO;
	};
	// IO
	union
	{
		struct
		{
			uint8_t ALMIN0 : 1;
			uint8_t ALMIN1 : 1;
			uint8_t ALMIN2 : 1;
			uint8_t ALMIN3 : 1;
			uint8_t ALMOUT0 : 1;
			uint8_t ALMOUT1 : 1;
			uint8_t ALMOUT2 : 1;
			uint8_t ALMOUT3 : 1;
		};
		struct
		{
			uint8_t ALMIN : 4;
			uint8_t ALMOUT : 4;
		};

		uint8_t ALMDIO;
	};
	// Analog Ch0
	union
	{
		struct
		{
			uint16_t ANINH0 : 8;
			uint16_t ANINL0 : 8;
		};
		uint16_t ANIN0;
	};

	// Analog Ch1
	union
	{
		struct
		{
			uint16_t ANINH1 : 8;
			uint16_t ANINL1 : 8;
		};
		uint16_t ANIN1;
	};

	// Analog Ch2
	union
	{
		struct
		{
			uint16_t ANINH2 : 8;
			uint16_t ANINL2 : 8;
		};
		uint16_t ANIN2;
	};

	// Analog Ch3
	union
	{
		struct
		{
			uint16_t ANINH3 : 8;
			uint16_t ANINL3 : 8;
		};
		uint16_t ANIN3;
	};
} APPL_DMA_IOCON_t;
//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern APPL_DMA_IOCON_t APPL_DMA_IOCON_d;
extern void APPL_DMA_IOCON_Init(void);
extern uint16_t APPL_DMA_IOCON_Get(APPL_DMA_IOCON_t *pdata);
extern uint16_t APPL_DMA_IOCON_Set(APPL_DMA_IOCON_t *pdata);
extern uint16_t APPL_DMA_IOCON_Battery(APPL_DMA_IOCON_t *pdata);
extern uint16_t APPL_DMA_IOCON_TempAndHumi(APPL_DMA_IOCON_t *pdata);
extern uint16_t APPL_DMA_IOCON_ADC_SPI_SET(uint8_t RegAddH, uint8_t RegAddL);
extern uint16_t APPL_DMA_IOCON_ADC_SPI_GET(uint8_t RegAddH, uint8_t RegAddL);
extern uint16_t APPL_DMA_IOCON_ADC_Get(APPL_DMA_IOCON_t *pdata);




#endif /* APPL_DMA_IOCON_H_ */