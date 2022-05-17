#ifndef ADS1115_H
#define ADS1115_H

#define ADS1115_IIC_ADDRESS0        (0x48)
#define ADS1115_IIC_ADDRESS1        (0x49)
#define ADS1115_CONVERSIONDELAY     (100)
#define ADS1115_POINTER_MASK        (0x03)
#define ADS1115_POINTER_CONVERT     (0x00)
#define ADS1115_POINTER_CONFIG      (0x01)
#define ADS1115_POINTER_LOWTHRESH   (0x02)
#define ADS1115_POINTER_HITHRESH    (0x03)

#define ADS1115_OS_POS        15
#define ADS1115_OS_MASK      (0x0001)       // Operational status/single-shot conversion start
#define ADS1115_OS_NOEFFECT  (0x0000)       // Write: Bit = 0
#define ADS1115_OS_SINGLE    (0x0001)       // Write: Bit = 1
#define ADS1115_OS_BUSY      (0x0000)       // Read: Bit = 0
#define ADS1115_OS_NOTBUSY   (0x0001)       // Read: Bit = 1

#define ADS1115_MUX_POS       12
#define ADS1115_MUX_MASK     (0x0007)       // Input multiplexer configuration
#define ADS1115_MUX_1        (0x0000)       // P = AIN0, N = AIN1(default)
#define ADS1115_MUX_2        (0x0001)       // P = AIN0, N = AIN3
#define ADS1115_MUX_3        (0x0002)       // P = AIN1, N = AIN3
#define ADS1115_MUX_4        (0x0003)       // P = AIN2, N = AIN3
#define ADS1115_MUX_5        (0x0004)       // P = AIN0, N = GND
#define ADS1115_MUX_6        (0x0005)       // P = AIN1, N = GND
#define ADS1115_MUX_7        (0x0006)       // P = AIN2, N = GND
#define ADS1115_MUX_8        (0x0007)       // P = AIN3, N = GND

#define ADS1115_PGA_POS       9
#define ADS1115_PGA_MASK     (0x0007)       // Programmable gain amplifier configuration
#define ADS1115_PGA_0        (0x0000)       // 6.144V 
#define ADS1115_PGA_1        (0x0001)       // 4.096V 
#define ADS1115_PGA_2        (0x0002)       // 2.048V (default)
#define ADS1115_PGA_3        (0x0003)       // 1.024V 
#define ADS1115_PGA_4        (0x0004)       // 0.512V 
#define ADS1115_PGA_5        (0x0005)       // 0.256V 

#define ADS1115_MODE_POS      8
#define ADS1115_MODE_MASK    (0x0001)       // Device operating mode
#define ADS1115_MODE_CONTIN  (0x0000)       // Continuous conversion mode
#define ADS1115_MODE_SINGLE  (0x0001)       // Power-down single-shot mode (default)

#define ADS1115_DR_POS        5
#define ADS1115_DR_MASK      (0x0007)       // Data rate(samples per second)
#define ADS1115_DR_1         (0x0000)       // 8 SPS
#define ADS1115_DR_2         (0x0001)       // 16 SPS
#define ADS1115_DR_3         (0x0002)       // 32 SPS
#define ADS1115_DR_4         (0x0003)       // 64 SPS
#define ADS1115_DR_5         (0x0004)       // 128 SPS (default)
#define ADS1115_DR_6         (0x0005)       // 250 SPS
#define ADS1115_DR_7         (0x0006)       // 475 SPS
#define ADS1115_DR_8         (0x0007)       // 860 SPS

#define ADS1115_COMP_MODE_POS       4
#define ADS1115_COMP_MODE_MASK      (0x0001)      // Comparator mode
#define ADS1115_COMP_MODE_TRAD      (0x0000)      // Traditional comparator with hysteresis (default)
#define ADS1115_COMP_MODE_WINDOW    (0x0001)      // Window comparator

#define ADS1115_COMP_POL_POS        3
#define ADS1115_COMP_POL_MASK       (0x0001)      // Comparator polarity
#define ADS1115_COMP_POL_ACTVLOW    (0x0000)      // Active low(default)
#define ADS1115_COMP_POL_ACTVHIGH   (0x0001)      // Active high

#define ADS1115_COMP_LAT_POS      2
#define ADS1115_COMP_LAT_MASK    (0x0001)         // Latching comparator
#define ADS1115_COMP_LAT_NONLAT  (0x0000)         // Non-latching comparator (default)
#define ADS1115_COMP_LAT_LATCH   (0x0001)         // Latching comparator

#define ADS1115_COMP_QUE_POS      0
#define ADS1115_COMP_QUE_MASK    (0x0003)         // Comparator queue and disable
#define ADS1115_COMP_QUE_1CONV   (0x0000)         // After one conversions
#define ADS1115_COMP_QUE_2CONV   (0x0001)         // After two conversions
#define ADS1115_COMP_QUE_4CONV   (0x0002)         // After four conversions
#define ADS1115_COMP_QUE_NONE    (0x0003)         // Disable the comparator(default)

extern int APPL_ads1115_readSample(uint16_t *value);
extern int APPL_ads1115_startSampling(uint8_t mux_mode);
extern bool APPL_ads1115_isBusy(void);
extern int32_t APPL_ads1115_uVolt(uint16_t *adcValueX);


#endif