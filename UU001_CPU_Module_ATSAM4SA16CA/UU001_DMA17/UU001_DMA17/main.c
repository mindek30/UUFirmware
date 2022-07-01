/*
 * UU001_DMA17.c
 *
 * Created: 17/4/2022 15:46:11
 * Author : Eakawit_kor
 */ 


#include "sam.h"
#include "APPL_DMA/APPL_DMA_OS.h"

uint8_t flag = 0;

void delay (void){
	for (volatile uint16_t x=0; x<65500;x++){
		asm ("nop");
	}
}

void WDT_init(void){
	//REG_RSTC_MR = RSTC_CR_KEY_PASSWD;
	REG_WDT_MR = 0;
}

void clock_init(void){
    //enable RC Internal
    REG_CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCF_12_MHz;
    //master clock source selection - choose main clock
    REG_PMC_MCKR |= PMC_MCKR_CSS_MAIN_CLK;
    //wait until main clock ready
//    while (!(REG_PMC_SR & PMC_SR_MCKRDY));
    //select processer prescaler (0 - no divisor)
    REG_PMC_MCKR |= PMC_MCKR_PRES_CLK_1;
    //select processer prescaler (div 4) 20mhz/4 = 5mhz
//    REG_PMC_MCKR |= PMC_MCKR_PRES_CLK_3;
    //wait until main clock ready
//    while (!(REG_PMC_SR & PMC_SR_MCKRDY));
}

void Systick_Sys_init(void){
	//SysTick->LOAD = 0x00FFFFFF;
	//SysTick->VAL = 0x00FFFFED;
	uint32_t Reload = 0x0001D520;
	SysTick->LOAD = Reload;
	SysTick->VAL = Reload;
	
	SysTick->CTRL = 0x7;
}

void Usart0_Init(void){
	
	REG_PIOA_PDR |= PIO_PDR_P5;
	REG_PIOA_PDR |= PIO_PDR_P6;
	
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P5);
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P6);
	
	REG_PMC_PCER0 |= PMC_PCER0_PID14;
	//REG_USART0_BRGR |= 26; //9600 @ 4MHz
	//REG_USART0_BRGR |= 52; //9600 @ 8MHz
	REG_USART0_BRGR |= 78; //9600 @ 12MHz
	
	REG_USART0_MR = US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_NBSTOP_1_BIT;
	
	REG_USART0_CR = US_CR_TXEN | US_CR_RXEN;
	
	REG_USART0_IER = US_IER_RXRDY;
	NVIC_ClearPendingIRQ(USART0_IRQn);
	NVIC_EnableIRQ(USART0_IRQn);
}

void Usart1_Init(void){
	REG_PIOA_PER |= PIO_PER_P23; //pio enable PA11
	REG_PIOA_OER |= PIO_PER_P23; //set PA11 as output
	
	REG_PIOA_PDR |= PIO_PDR_P21;
	REG_PIOA_PDR |= PIO_PDR_P22;
	
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P21);
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P22);
	
	REG_PMC_PCER0 |= PMC_PCER0_PID15;
	//REG_USART1_BRGR |= 13; //19200 @ 4MHz
	//REG_USART1_BRGR |= 26; //19200 @ 8MHz
	REG_USART1_BRGR |= 39; //19200 @ 12MHz

	
	REG_USART1_MR = US_MR_CHRL_8_BIT | US_MR_PAR_NO | US_MR_NBSTOP_1_BIT;
	
	REG_USART1_CR = US_CR_TXEN | US_CR_RXEN;
	
	REG_USART1_IER = US_IER_RXRDY;
	NVIC_ClearPendingIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
}

void ADC_init(){
    /*enabling the channel automatically gives peripheral control of the pin instead of the PIO
    automatically configures pin to input, pull up enabled and connect to ground*/
	//enable clock for ADC
	REG_PMC_PCER0|= PMC_PCER0_PID29;
	    
    //enable ADC channel0
    REG_ADC_CHER |= ADC_CHER_CH0;
	
    //set ADC clock to 1mhz
    /*datasheet page 1172
    for clock 500khz to 1mhz set ADC_ACR IBCTL =  01
    */
    REG_ADC_ACR |= ADC_ACR_IBCTL(1);
    /*clock prescaler = (fcpu / (2*adc_freq)) - 1
    12,000,000 /  (2*1000000) - 1 = 6
    */
    REG_ADC_MR |= ADC_MR_PRESCAL(6);
	REG_ADC_CR |= ADC_CR_START;
}

void SPI_setMode(uint8_t mode){	
    /*
    Mode		CPOL	NCPHA
    Mode0		0		1
    Mode1		0		0
    Mode2		1		1
    Mode3		1		0
    */
    if (mode == 0){
        REG_SPI_CSR &= ~SPI_CSR_CPOL;
        REG_SPI_CSR |= SPI_CSR_NCPHA;
    }
    else if (mode == 1){
        REG_SPI_CSR &= ~SPI_CSR_CPOL;
        REG_SPI_CSR &= ~SPI_CSR_NCPHA;
    }
    else if (mode == 2){
        REG_SPI_CSR |= SPI_CSR_CPOL;
        REG_SPI_CSR |= SPI_CSR_NCPHA;
    }
    else if (mode == 3){
        REG_SPI_CSR |= SPI_CSR_CPOL;
        REG_SPI_CSR &= ~SPI_CSR_NCPHA;
    }
}

void SPI_init(int bitbang){
	if(bitbang){
		REG_PMC_PCER0 |= PMC_PCER0_PID11;
		REG_PIOA_PER |= PIO_PDR_P11|PIO_PER_P13|PIO_PER_P14; //LTE enable PA29
		REG_PIOA_OER |= PIO_PDR_P11|PIO_PER_P13|PIO_PER_P14; //set PA29 as output
		REG_PIOA_SODR |= PIO_PDR_P11|PIO_PER_P13|PIO_PER_P14; //set PA11 high (LED on)
		
		REG_PIOA_PPDER |= PIO_PPDDR_P12; //disable pull down
		REG_PIOA_PUDR |= PIO_PUDR_P12; //enable internal pull up resistor on PA24
		REG_PIOA_IFER |= PIO_IFER_P12;	//enable glitch filter on PA24 (button debounce)
		
	}else{
		//enable peripheral clock
		REG_PMC_PCER0 |= PMC_PCER0_PID21;
		//set spi master mode
		REG_SPI_MR |= SPI_MR_MSTR;
		//set fixed peripheral select(peripheral chosen in SP_MR.PCS instead of SPI_THR.PCS)
		REG_SPI_MR &= ~SPI_MR_PS;
		//set polarity and clock phase
		SPI_setMode(3);
		//set clock generator (1 = peripheral clock rate), otherwise a divisor
		//SCBR = fperipheral clock / SPCK Bit Rate
		REG_SPI_CSR |= SPI_CSR_SCBR(255);
		//chip select remains low after transfer
		REG_SPI_CSR |= SPI_CSR_CSNAAT;
		// ADC Init Pin
		//REG_PIOA_PER |= PIO_PER_P11; //pio enable PA11
		//REG_PIOA_OER |= PIO_OER_P11; //set PA11 as output
		//give peripheral control of pins (Chip select pins are optional)
		REG_PIOA_PDR |= PIO_PDR_P11; //NPCS0
		//REG_PIOA_PDR |= PIO_PDR_P31; //NPCS1
		REG_PIOA_PDR |= PIO_PDR_P12; //MISO
		REG_PIOA_PDR |= PIO_PDR_P13; //MOSI
		REG_PIOA_PDR |= PIO_PDR_P14; //MOSI
	
		REG_PIOA_ABCDSR &=  ~(PIO_PDR_P12);
		REG_PIOA_ABCDSR &=  ~(PIO_PDR_P13);
		REG_PIOA_ABCDSR &=  ~(PIO_PDR_P14);
	
		//enable SPI
		REG_SPI_CR |= SPI_CR_SPIEN;
	}
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	Systick_Sys_init();
	clock_init();
	WDT_init();
	Usart0_Init();
	Usart1_Init();
	SPI_init(1);
	ADC_init();
	APPL_DMA_OS_Init();
	APPL_DMA_RS485_printf("Waiting for input\r\n");
	while (1)
	{ 
		if(flag == 1){
			APPL_DMA_OS_10ms();
			flag = 0;
		}
	}
}

void SysTick_Handler(void){
	flag = 1;
}

void USART0_Handler(void) {
	// code to run when interrupt triggered
	uint32_t status = REG_USART0_CSR;
	if ((status & US_CSR_RXRDY)){
		//read receive holding register
		uint8_t readByte = REG_USART0_RHR;
		//transmit that byte back
		if ((readByte != 0x0d) && (readByte != 0x0a)){
			EKM_Buffer_Set(&APPL_DMA_LTE_Buffer,readByte);
		}
		if (readByte == 0x0a){
			APPL_DMA_LTE_RES_RUN = 1;
		}
	}
}


void USART1_Handler(void) {
	// code to run when interrupt triggered
	uint32_t status = REG_USART1_CSR;
	if ((status & US_CSR_RXRDY)){
		//read receive holding register
		uint8_t readByte = REG_USART1_RHR;
		//transmit that byte back
		if(readByte!=0x0d){
			EKM_Buffer_Set(&APPL_DMA_RS48_Rx,readByte);
		}
	}
}

/*//when ADC conversion is complete it will trigger an interrupt and interrupt will end by starting a new conversion
void ADC_Handler( void) {
	//read interrupt flag status register
	uint32_t status = REG_ADC_ISR;
	uint32_t adcData;
	if ((status & ADC_IMR_EOC0) > 0){
		//reading this register clears flag in ISR
		adcData = REG_ADC_CDR;
	}
	//start the next conversion
	REG_ADC_CR |= ADC_CR_START;
}//*/