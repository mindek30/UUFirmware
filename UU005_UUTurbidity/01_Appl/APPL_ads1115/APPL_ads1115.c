#include "main.h"
#include "i2c.h"
#include "APPL_ads1115.h"

int APPL_ads1115_readSample(uint16_t *value);
int APPL_ads1115_startSampling(uint8_t mux_mode);
bool APPL_ads1115_isBusy(void);
int32_t APPL_ads1115_uVolt(uint16_t *adcValueX);

static uint16_t APPL_ads1115_getDefaultConfig()
{
  uint16_t config = 0;

  config |= (ADS1115_OS_NOEFFECT & ADS1115_OS_MASK) << ADS1115_OS_POS;
  config |= (ADS1115_MUX_1 & ADS1115_MUX_MASK) << ADS1115_MUX_POS;
  config |= (ADS1115_PGA_1 & ADS1115_PGA_MASK) << ADS1115_PGA_POS;
  config |= (ADS1115_MODE_SINGLE & ADS1115_MODE_MASK) << ADS1115_MODE_POS;
  config |= (ADS1115_DR_8 & ADS1115_DR_MASK) << ADS1115_DR_POS;
  config |= (ADS1115_COMP_MODE_TRAD & ADS1115_COMP_MODE_MASK) << ADS1115_COMP_MODE_POS;
  config |= (ADS1115_COMP_POL_ACTVLOW & ADS1115_COMP_POL_MASK) << ADS1115_COMP_POL_POS;
  config |= (ADS1115_COMP_LAT_NONLAT & ADS1115_COMP_LAT_MASK) << ADS1115_COMP_LAT_POS;
  config |= (ADS1115_COMP_QUE_NONE & ADS1115_COMP_QUE_MASK) << ADS1115_COMP_QUE_POS;

  // printf("ADS1115 : Config = 0x%x\n", config);

  return config;
}

static int APPL_ads1115_write(uint8_t regAddr, uint16_t data)
{
  HAL_StatusTypeDef ret;
  Bit16ToBit8_t temp;
  uint8_t send[3];
  temp.REG = data;
  send[0] = regAddr;
  send[1] = temp.HREG;
  send[2] = temp.LREG;
  ret = HAL_I2C_Master_Transmit(&hi2c1, ADS1115_IIC_ADDRESS0 << 1, send, 3, 100);
  //APPL_CMD_printf("I2C = 0x%x\r\n", ret);
  return ret;
}

static int APPL_ads1115_read(uint8_t regAddr, uint16_t *data)
{
  HAL_StatusTypeDef ret;
  Bit16ToBit8_t temp;
  uint8_t send[2];
  send[0] = regAddr;
  send[1] = 0;
  ret = HAL_I2C_Master_Transmit(&hi2c1, ADS1115_IIC_ADDRESS0 << 1, send, 1, 100);
  ret |= HAL_I2C_Master_Receive(&hi2c1, ADS1115_IIC_ADDRESS0 << 1, send, 2, 100);
  temp.HREG = send[0];
  temp.LREG = send[1];
  *data = temp.REG;
  //APPL_CMD_printf("I2C = 0x%x\r\n", ret);
  return ret;
}

int APPL_ads1115_readSample(uint16_t *value)
{
  HAL_StatusTypeDef ret;
  ret = APPL_ads1115_read(ADS1115_POINTER_CONVERT, value);
  return ret;
}

int APPL_ads1115_startSampling(uint8_t mux_mode)
{
  uint16_t config = APPL_ads1115_getDefaultConfig();
  config |= (ADS1115_OS_SINGLE & ADS1115_OS_MASK) << ADS1115_OS_POS;
  config |= (mux_mode & ADS1115_MUX_MASK) << ADS1115_MUX_POS;
  //APPL_CMD_printf("Data Value = %04X\r\n", config);
  return APPL_ads1115_write(ADS1115_POINTER_CONFIG, config);
}

bool APPL_ads1115_isBusy(void)
{
  HAL_StatusTypeDef ret_code;
  bool busy = true;
  uint16_t value = 0;
  ret_code = APPL_ads1115_read(ADS1115_POINTER_CONFIG, &value);

  // APPL_CMD_printf("I2C = 0x%x\r\n", ret_code);
  if (ret_code == HAL_OK)
  {
    if (value & (ADS1115_OS_MASK << ADS1115_OS_POS))
    {
      busy = false;
    }
  }
  else
  {
    APPL_CMD_printf("ADS1115 : Get Status : Failed\n");
  }

  return busy;
}

int32_t APPL_ads1115_uVolt(uint16_t *adcValueX)
{
  return *adcValueX*4096000/32768;
}

