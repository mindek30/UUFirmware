/*
 * APPL_DMA_IOCON.c
 *
 * Created: 20/4/2022 1:52:46
 *  Author: Eakawit_kor
 */ 

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "sam.h"
#include "EKM_Modbus.h"
#include "APPL_DMA_IOCON.h"
#define  APPL_DMA_IOCON_SPI_BITBANGE 1

APPL_DMA_IOCON_t APPL_DMA_IOCON_d;
void APPL_DMA_IOCON_Init(void);
uint16_t APPL_DMA_IOCON_Get(APPL_DMA_IOCON_t *pdata);
uint16_t APPL_DMA_IOCON_Set(APPL_DMA_IOCON_t *pdata);
uint16_t APPL_DMA_IOCON_ADC_SPI_SET(uint8_t RegAddH, uint8_t RegAddL);
uint16_t APPL_DMA_IOCON_ADC_SPI_GET(uint8_t RegAddH, uint8_t RegAddL);
uint16_t APPL_DMA_IOCON_ADC_Get(APPL_DMA_IOCON_t *pdata);
uint16_t APPL_DMA_IOCON_Battery(APPL_DMA_IOCON_t *pdata);
uint16_t APPL_DMA_IOCON_TempAndHumi(APPL_DMA_IOCON_t *pdata);

/*******************************************************************************
* Function: APPL_DMA_IOCON_Init
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_IOCON_Init(void){
	// Information for Device 
	APPL_DMA_IOCON_d.IOT_ID = 19;
	// ip MQTT Broker 27.254.115.123
	APPL_DMA_IOCON_d.MQTT_IP0 = 27;
	APPL_DMA_IOCON_d.MQTT_IP1 = 254;
	APPL_DMA_IOCON_d.MQTT_IP2 = 115;
	APPL_DMA_IOCON_d.MQTT_IP3 = 123;
	APPL_DMA_IOCON_d.MQTT_PORT = 1883;
	strcpy(APPL_DMA_IOCON_d.MQTT_USER, "1111");
	strcpy(APPL_DMA_IOCON_d.MQTT_PASS, "1111");

	// IO Init
	APPL_DMA_IOCON_d.DIO = 0x00;
	// Port B Init
	REG_PMC_PCER0 |= PMC_PCER0_PID12;
	REG_PIOB_PER |= PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P13 | PIO_PER_P14; //set controllable by the PIO controller (disable peripheral)
	REG_PIOB_ODR |= PIO_ODR_P0 | PIO_ODR_P1 | PIO_ODR_P14; //output disable register (set as input for button)
	REG_PIOB_OER |= PIO_OER_P2 | PIO_OER_P3 | PIO_OER_P13; //output enable register
	REG_PIOB_PPDER |= PIO_PPDDR_P0 | PIO_PPDDR_P1 | PIO_PPDDR_P14; //disable pull down
	REG_PIOB_PUDR |= PIO_PUDR_P0 | PIO_PUDR_P1 | PIO_PUDR_P14; //enable internal pull up resistor on PA24
	REG_PIOB_IFER |= PIO_IFER_P0 | PIO_IFER_P1 | PIO_IFER_P14;	//enable glitch filter on PA24 (button debounce)
	// Port C Init
	REG_PMC_PCER0 |= PMC_PCER0_PID13;
	REG_PIOC_PER |= PIO_PER_P24 | PIO_PER_P25 | PIO_PER_P26 | PIO_PER_P29 | PIO_PER_P30 | PIO_PER_P31; //set controllable by the PIO controller (disable peripheral)
	REG_PIOC_ODR |= PIO_ODR_P25 | PIO_ODR_P29 | PIO_ODR_P30; //output disable register (set as input for button)
	REG_PIOC_OER |= PIO_OER_P24 | PIO_OER_P26 | PIO_OER_P31; //output enable register
	REG_PIOC_PPDER |= PIO_PPDDR_P25 | PIO_PPDDR_P29 | PIO_PPDDR_P30; //disable pull down
	REG_PIOC_PUDR |= PIO_PUDR_P25 | PIO_PUDR_P29 | PIO_PUDR_P30; //enable internal pull up resistor on PA24
	REG_PIOC_IFER |= PIO_IFER_P25 | PIO_IFER_P29 | PIO_IFER_P30;	//enable glitch filter on PA24 (button debounce)

	// ADC init
	APPL_DMA_IOCON_d.ANIN0 = 0x0000;
	APPL_DMA_IOCON_d.ANIN1 = 0x0000;
	APPL_DMA_IOCON_d.ANIN2 = 0x0000;
	APPL_DMA_IOCON_d.ANIN3 = 0x0000;
	// ADC Init Pin
	REG_PIOA_PER |= PIO_PER_P15 | PIO_PER_P16; //pio enable PA11
	REG_PIOA_OER |= PIO_OER_P15 | PIO_OER_P16; //set PA11 as output
	// ADC Set pin
	REG_PIOA_CODR |= PIO_PER_P16; //set SPIResetPin low (LED Off)
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	REG_PIOA_SODR |= PIO_PER_P16; //set SPIResetPin high (LED on)
	//REG_PIOA_SODR |= PIO_PER_P11; //set CSADC high (LED on)
	// ADC Init chip
	APPL_DMA_IOCON_ADC_SPI_SET(0x85, 0x00);
	APPL_DMA_IOCON_ADC_SPI_SET(0x0B, ADS8688_REG_PLUS25VREF);
	APPL_DMA_IOCON_ADC_SPI_SET(0x0D, ADS8688_REG_PLUS25VREF);
	APPL_DMA_IOCON_ADC_SPI_SET(0x0F, ADS8688_REG_PLUS25VREF);
	APPL_DMA_IOCON_ADC_SPI_SET(0x11, ADS8688_REG_PLUS25VREF);
	APPL_DMA_IOCON_ADC_SPI_SET(0x03, 0x00);
	//APPL_IOCON_ADC_SPI_SET(0x05, 0x07);*/
	
	// Batterry
	APPL_DMA_IOCON_d.Batt = 0;
		
	// temp & Humid init
	APPL_DMA_IOCON_d.Temperature = 0;
	APPL_DMA_IOCON_d.Humidity = 0;
}

/*******************************************************************************
* Function: APPL_DMA_IOCON_Get
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_DMA_IOCON_Get(APPL_DMA_IOCON_t *pdata)
{
	// GPIO Get
	pdata->IN0 = ((REG_PIOB_PDSR & PIO_PDSR_P0)==0) ? 0:1;
	pdata->IN1 = ((REG_PIOC_PDSR & PIO_PDSR_P29)==0) ? 0:1;
	pdata->IN2 = ((REG_PIOB_PDSR & PIO_PDSR_P1)==0) ? 0:1;
	pdata->IN3 = ((REG_PIOC_PDSR & PIO_PDSR_P30)==0) ? 0:1;
	pdata->ALMIN0 = ((REG_PIOB_PDSR & PIO_ODR_P14)==0)  ? 0:1;
	pdata->ALMIN1 = ((REG_PIOC_PDSR & PIO_ODR_P25)==0) ? 0:1;
	return 0;
}

/*******************************************************************************
* Function: APPL_DMA_IOCON_Set
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_DMA_IOCON_Set(APPL_DMA_IOCON_t *pdata)
{
	// GPIO Write
	if(pdata->OUT0==1){
		REG_PIOB_SODR |= PIO_PER_P2; //set PA11 high (LED on)
	}else{
		REG_PIOB_CODR |= PIO_PER_P2; //set PA11 low (LED Off)
	}
	if(pdata->OUT1==1){
		REG_PIOC_SODR |= PIO_PER_P31; //set PA11 high (LED on)
	}else{
		REG_PIOC_CODR |= PIO_PER_P31; //set PA11 low (LED Off)
	}
	if(pdata->OUT2==1){
		REG_PIOB_SODR |= PIO_PER_P3; //set PA11 high (LED on)
	}else{
		REG_PIOB_CODR |= PIO_PER_P3; //set PA11 low (LED Off)
	}
	if(pdata->OUT3==1){
		REG_PIOC_SODR |= PIO_PER_P26; //set PA11 high (LED on)
	}else{
		REG_PIOC_CODR |= PIO_PER_P26; //set PA11 low (LED Off)
	}
	if(pdata->ALMOUT0==1){
		REG_PIOB_SODR |= PIO_PER_P13; //set PA11 high (LED on)
	}else{
		REG_PIOB_CODR |= PIO_PER_P13; //set PA11 low (LED Off)
	}
	if(pdata->ALMOUT1==1){
		REG_PIOC_SODR |= PIO_PER_P24; //set PA11 high (LED on)
	}else{
		REG_PIOC_CODR |= PIO_PER_P24; //set PA11 low (LED Off)
	}
	
	return 0;
}

void APPL_DMA_SPI_selectPeripheral(uint8_t peripheral){
	//chose peripheral
	//this only works if SPI_MR.PS = 0
	//if SPI_MR.PS = 1 then peripheral selection is done in SPI_THR.PCS
	if (peripheral == 0){
		//choose NPCS0
		REG_SPI_MR |= SPI_MR_PCS(0b1110);
	}
	else if (peripheral ==1){
		//choose NPCS1
		REG_SPI_MR |= SPI_MR_PCS(0b1101);
	}
	else if (peripheral ==2){
		//choose NPCS2
		REG_SPI_MR |= SPI_MR_PCS(0b1011);
	}
	else if (peripheral ==3){
		//choose NPCS3
		REG_SPI_MR |= SPI_MR_PCS(0b0111);
	}else{
		//Disable
		REG_SPI_MR |= SPI_MR_PCS(0b1111);	
	}
}


/*******************************************************************************
* Function: APPL_DMA_IOCON_delay
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
void APPL_DMA_IOCON_delay (void){
	for (volatile uint16_t x=0; x<10;x++){
		asm ("nop");
	}
}

/*******************************************************************************
* Function: APPL_DMA_IOCON_ADC_SPI_SET
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint8_t APPL_DMA_IOCON_byteExchange(uint8_t data){
	if(APPL_DMA_IOCON_SPI_BITBANGE){
		/*
			REG_PIOA_PDR |= PIO_PDR_P11; //NPCS0
			REG_PIOA_PDR |= PIO_PDR_P12; //MISO
			REG_PIOA_PDR |= PIO_PDR_P13; //MOSI
			REG_PIOA_PDR |= PIO_PDR_P14; //SCK
		*/
		uint8_t byte_in = 0;
		uint8_t bit;
		for (bit = 0x80; bit; bit >>= 1) {
			/* Shift-out a bit to the MOSI line */
			if(data & bit){
				REG_PIOA_SODR |= PIO_PDR_P13; //set PA11 high (LED on)
				}else{
				REG_PIOA_CODR |= PIO_PDR_P13; //set PA11 low (LED Off)
			}

			/* Delay for at least the peer's setup time */
			APPL_DMA_IOCON_delay();

			/* Pull the clock line high */
			REG_PIOA_SODR |= PIO_PDR_P14; //set PA11 high (LED on)
				
			/* Shift-in a bit from the MISO line */
			if ((REG_PIOA_PDSR & PIO_PDSR_P12)!=0)
			byte_in |= bit;

			/* Delay for at least the peer's hold time */
			APPL_DMA_IOCON_delay();

			/* Pull the clock line low */
			REG_PIOA_CODR |= PIO_PDR_P14; //set PA11 low (LED Off)
		}
		return byte_in;
	}else{	
		//wait for transmit register to be empty
		while (!(REG_SPI_SR & SPI_SR_TDRE));
		//send data to transmit register
		REG_SPI_TDR |= data;
		//wait for received data to be ready to be read
		while (!(REG_SPI_SR & SPI_SR_RDRF));
		//read received data
		return REG_SPI_RDR;
	}
}


uint8_t APPL_DMA_IOCON_SPI_transfer_byte(uint8_t byte_out)
{
	uint8_t byte_in = 0;
	uint8_t bit;

	for (bit = 0x80; bit; bit >>= 1) {
		/* Shift-out a bit to the MOSI line */
		if(byte_out & bit){
			REG_PIOA_SODR |= PIO_PDR_P13; //set PA11 high (LED on)
			}else{
			REG_PIOA_CODR |= PIO_PDR_P13; //set PA11 low (LED Off)
		}

		/* Delay for at least the peer's setup time */
		APPL_DMA_IOCON_delay();

		/* Pull the clock line high */
		REG_PIOA_SODR |= PIO_PDR_P14; //set PA11 high (LED on)
			
		/* Shift-in a bit from the MISO line */
		if ((REG_PIOA_PDSR & PIO_PDSR_P12)!=0)
		byte_in |= bit;

		/* Delay for at least the peer's hold time */
		APPL_DMA_IOCON_delay();

		/* Pull the clock line low */
		REG_PIOA_CODR |= PIO_PDR_P14; //set PA11 low (LED Off)
	}

	return byte_in;
}
/*******************************************************************************
* Function: APPL_DMA_IOCON_ADC_SPI_SET
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_DMA_IOCON_ADC_SPI_SET(uint8_t RegAddH, uint8_t RegAddL)
{
	Bit16ToBit8_t dat;
	// Hardware Receive for ADS8684
	REG_PIOA_CODR |= PIO_PER_P11; //set CSADC low (LED Off)
	dat.LREG = APPL_DMA_IOCON_byteExchange(RegAddH);
	dat.LREG = APPL_DMA_IOCON_byteExchange(RegAddL);
	dat.LREG = APPL_DMA_IOCON_byteExchange(0x00);
	REG_PIOA_SODR |= PIO_PER_P11; //set CSADC high (LED on)
	
	return dat.REG;
}

/*******************************************************************************
* Function: AAPPL_DMA_IOCON_ADC_SPI_GET
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_DMA_IOCON_ADC_SPI_GET(uint8_t RegAddH, uint8_t RegAddL)
{
	Bit16ToBit8_t dat;
	//RegAddH = 0xC0;
	//RegAddL = 0x00;

	// Hardware Receive for ADS8684
	REG_PIOA_CODR |= PIO_PER_P11; //set CSADC low (LED Off)
	dat.LREG = APPL_DMA_IOCON_byteExchange(RegAddH);
	dat.LREG = APPL_DMA_IOCON_byteExchange(RegAddL);
	dat.HREG = APPL_DMA_IOCON_byteExchange(0x00);
	dat.LREG = APPL_DMA_IOCON_byteExchange(0x00);
	REG_PIOA_SODR |= PIO_PER_P11; //set CSADC high (LED on)
	
	return dat.REG;
}

/*******************************************************************************
* Function: APPL_IOCON_ADC_Get
*
* Parameters:      -
* Returned value:  -
*
* Description:
*
* Calling:
******************************************************************************/
uint16_t APPL_DMA_IOCON_ADC_Get(APPL_DMA_IOCON_t *pdata)
{
	pdata->ANIN0 = 0;
	pdata->ANIN1 = 0;
	pdata->ANIN2 = 0;
	pdata->ANIN3 = 0;
	pdata->ANIN0 = APPL_DMA_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(1), ADS8688_CMD_REG_NOOP);
	pdata->ANIN1 = APPL_DMA_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(2), ADS8688_CMD_REG_NOOP);
	pdata->ANIN3 = APPL_DMA_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(3), ADS8688_CMD_REG_NOOP);
	pdata->ANIN2 = APPL_DMA_IOCON_ADC_SPI_GET(ADS8688_CMD_REG_MAN_CH(0), ADS8688_CMD_REG_NOOP);
  return 0;
}


/*******************************************************************************
* Function: APPL_DMA_IOCON_Battery
*
* Parameters:      -
* Returned value:  -
*
* Description: using for set Battery level
*
* Calling:
******************************************************************************/
uint16_t APPL_DMA_IOCON_Battery(APPL_DMA_IOCON_t *pdata)
{
	pdata->Batt = REG_ADC_CDR;
	REG_ADC_CR |= ADC_CR_START;
	/*
	adc reading/12 bit Resolution=voltage/advref voltage
	266/4095 = voltage/3300 
	voltage = 266 / 4095 x 3300 = 214mv */
	return 0;
}

/*******************************************************************************
* Function: APPL_Status_TempAndHumi
*
* Parameters:      -
* Returned value:  -
*
* Description: Read Temperature form HDC2021
*
* Calling:
******************************************************************************/
uint16_t APPL_DMA_IOCON_TempAndHumi(APPL_DMA_IOCON_t *pdata)
{
	uint8_t temp=0;
	pdata->Temperature = 0x25FF;
	pdata->Humidity = 0x25FF;
			
	/**
     * formulas for conversion of the sensor signals, optimized for fixed point
     * algebra:
     * Temperature = 175 * S_T / 65535 - 45
     * Relative Humidity = 125 * (S_RH / 65535) - 6
     
    uint16_t words[2];
    *temperature = ((21875 * (int32_t)words[0]) >> 13) - 45000;
    *humidity = ((15625 * (int32_t)words[1]) >> 13) - 6000;//*/
	
	return (uint16_t)temp;
}

//END OF FILE