#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "EKM_Modbus.h"

int main(void)
{
  Bit16ToBit8_t dex;
  uint8_t Lo;
  uint8_t Hi;

  dex.REG = 0xABCD;
  Lo = (uint8_t)dex.LREG;
  Hi = (uint8_t)dex.HREG;
  
  printf("%x, %x\n",Lo,Hi);
  system("pause");
  return 0;
}
