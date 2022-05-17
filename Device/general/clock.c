#include "stm8l15x_tim1.h"
#include "stm8l15x_clk.h"
#include "clock.h"

U16 count;
U16 count1; 
u32 t;
/* 
TIM4 configuration:
      - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
      clock used is 16 MHz / 128 = 125 000 Hz
     - With 125 000 Hz we can generate time base:
         max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
         min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
     - In this examle we need to generate a time base equal to 1 ms
      so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 
*/
void timerC_init(void)
{   
    // Enable TIM4 CLK 
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    // Time base configuration
    TIM2_TimeBaseInit(TIM2_Prescaler_128,TIM2_CounterMode_Up,124);
    // Clear TIM4 update flag
    TIM2_ClearFlag(TIM2_FLAG_Update);
    //Enable update interrupt
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);
    TIM2_Cmd(ENABLE);
}

/*---------------------------------------------------------------------------*/
void Timer_Init(void)
{
     count = 0; 
    timerC_init();
}

void Delayms(u16 time)
{ 
 u16 timer;
 timer=time;
 count1 = 0; 
 while(count1<timer)
 {
  IWDG_ReloadCounter(); 
 }
}

void Delay5us(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount--);
}




