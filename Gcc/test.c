#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//! Structure for an entry in the command list table.
typedef struct APPL_DMA_LTE_type
{
    const char *Command;
    uint8_t Return_Stage;
} APPL_DMA_LTE_t;

APPL_DMA_LTE_t APPL_DMA_LTE_Step_Init[15];

void APPL_DMA_LTE_Init(void)
{
    APPL_DMA_LTE_Step_Init[0].Command = "ATE0\r\n";
    APPL_DMA_LTE_Step_Init[1].Command = "AT+SMCONF=\"CLIENTID\",\"id\"\r\n";
    APPL_DMA_LTE_Step_Init[2].Command = "AT+SMCONF=\"KEEPTIME\",60\r\n";
    APPL_DMA_LTE_Step_Init[3].Command = "AT+SMCONF=\"URL\",\"203.151.79.149\",\"1883\"\r\n";
    APPL_DMA_LTE_Step_Init[4].Command = "AT+SMCONF=\"USERNAME\",\"1111\"\r\n";
    APPL_DMA_LTE_Step_Init[5].Command = "AT+SMCONF=\"PASSWORD\",\"1111\"\r\n";
    APPL_DMA_LTE_Step_Init[6].Command = "AT+SMCONF=\"CLEANSS\",1\r\n";
    APPL_DMA_LTE_Step_Init[7].Command = "AT+SMCONF=\"QOS\",0\r\n";
    APPL_DMA_LTE_Step_Init[8].Command = "AT+SMCONF=\"RETAIN\",0\r\n";
    APPL_DMA_LTE_Step_Init[9].Command = "AT+CMNB=3\r\n";
    APPL_DMA_LTE_Step_Init[10].Command = "AT+CNACT=1,\"cmnet\"\r\n";

    for(int i = 0;i<10;i++){
        APPL_DMA_LTE_Step_Init[i].Return_Stage = 0;
    }
    APPL_DMA_LTE_Step_Init[0].Return_Stage = 2;
}

int main()
{
    APPL_DMA_LTE_Init();
    for (int i = 0; i < 10; i++){
        printf("%s", APPL_DMA_LTE_Step_Init[i].Command);
    }
    return 0;
}
