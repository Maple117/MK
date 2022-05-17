/******************** (C) COPYRIGHT  风驰电子嵌入式开发工作室 ********************/
#include "stm8l15x_rst.h"
#include "Typedef.h"
#include "wwdg.h"
//#include "led.h"

//U8 FirstPowerOn;

#define CounterInit 0x7f
#define window      0x50

void WWDG_Configuration(void)
{
    /* WWDG Configuration */
    /* Watchdog Window= 0x7F step to 0x3F step
    = (0x7F-0x3F) * 1 step
    = 64 * 1 step
    = 64 * (12288/2Mhz)
    = 393,216ms
    */
    /* Allowed Window = (0x7F-window) * 1 step
    = (0x7F-0x77) * 1 step
    = 7 * 1 step
    = 7 * (12288/2Mhz)
    = 43.008ms
    */
    /* So the non allowed window starts from 0.0ms to 43.008ms
    and the alowed window starts from 43.008ms to 393,216ms */
    WWDG_Init(CounterInit,window);
#ifdef WDG_ENABLE
    Refresh_WWDG_Window();
#endif
}

void Refresh_WWDG_Window(void)
{
    U8 CounterValue;
    CounterValue = (U8)(WWDG_GetCounter() & 0x7F);

    if (CounterValue < window)
    {
        WWDG_SetCounter(CounterInit);
    }
}