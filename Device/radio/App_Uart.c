#include "stm8l15x.h"
#include "list.h"
#include "clock.h"
#include "Uart.h"
#include "Radio.h"
#include "AppLyr.h"
#include "Configure.h"
#include "App_Uart.h"

U8 bSetRs232Flag;

static U8 UartStt;
static U8 UartCount;
static U8 UartCountEnd;
#define CLR_UART_STT_TIMEOUT()       { UartCount = 0;UartStt = TIMER_IDLE;}
#define SET_UART_STT_TIMEOUT(itvl)   { CLR_UART_STT_TIMEOUT(); UartCountEnd = itvl;UartStt = WAIT_TIMER_OUT;}

void Uart_Task(void)
{
    if ((bSetRs232Flag == 1) || (UART_DATA_MAX_LEN < u16GetSerialRXQueueUsedSize(SERIAL0)))
    {
        U8 i;
        U8 TxAppBuffer[UART_DATA_MAX_LEN];
        if (bSetRs232Flag != 1)
        {
            if (u16GetSerialRXQueueUsedSize(SERIAL0) < UART_DATA_MAX_LEN)
            {
                SET_UART_STT_TIMEOUT(5);
                return;
            }
        }
        bSetRs232Flag = 0;
        for (i = 0; (!bSerialRXQueueEmtpy(SERIAL0)&&(i < UART_DATA_MAX_LEN)); i++)
        {
            TxAppBuffer[i] = u8GetFromSerialRXQueue(SERIAL0);
        }
        if(i == 0)
        {
            return;
        }
        if(ConfigDataJudge(TxAppBuffer) == FALSE)
        {
            AppLyr_AddTask(i ,TxAppBuffer);
        }
    }
    if (UartReceived == 1)
    {
        UartReceived = 0;
        SET_UART_STT_TIMEOUT(20);
    }
}

void UART_TIMER_INT(void)
{
    if (UartStt == WAIT_TIMER_OUT)
    {
        UartCount++;
        if (UartCountEnd == UartCount)
        {
            CLR_UART_STT_TIMEOUT();
            bSetRs232Flag = 1;
        }
    }
}
