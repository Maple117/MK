#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "Typedef.h"
#include "WakeUp.h"
#include "Lopsc.h"
#include "Sleep.h"

U8 SleepFlag;

static U8 WakedStt;
static U16 WakedCount;
static U16 WakedCountEnd;
#define CLR_WAKED_STT_TIMEOUT()       { WakedCount = 0;WakedStt = TIMER_IDLE;}
#define SET_WAKED_STT_TIMEOUT(itvl)   { CLR_WAKED_STT_TIMEOUT(); WakedCountEnd = itvl;WakedStt = WAIT_TIMER_OUT;}

void WAKED_TIMER_INT(void)
{
    if (WakedStt == WAIT_TIMER_OUT)
    {
        WakedCount++;
        if (WakedCountEnd == WakedCount)
        {
            //WakedStt = TIMER_OUT;
            CLR_WAKED_STT_TIMEOUT();
            SleepFlag = TRUE;
        }
    }
}

void SleepTask(void)
{
    if (SleepFlag == TRUE)
    {
        CLK_SYSCLKSourceSwitchCmd(ENABLE);
        CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
        while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI)
        {
#ifdef WDG_ENABLE
            Refresh_WWDG_Window();
#endif
        }
        
        Ax5043_Receiver_On_Wor();
        LED_TX = 1;
        LED_RX = 1;
        enableInterrupts(); 
        ITC_SetSoftwarePriority(EXTI4_IRQn, ITC_PriorityLevel_1);
        halt();
        SleepFlag = FALSE;
#ifdef WDG_ENABLE
        Refresh_WWDG_Window(); 
#endif
        CLK_SYSCLKSourceSwitchCmd(ENABLE);
        CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
        while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)
        {
#ifdef WDG_ENABLE
            Refresh_WWDG_Window();
#endif
        }
        LED_TX = 0;
        LED_RX = 0;
        SleepFlag = TRUE;
        //timerC_init();
        enableInterrupts();
    }
}