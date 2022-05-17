#include "stm8l15x_clk.h"
#include "stm8l15x_spi.h"
#include "Typedef.h"
#include "spi.h"
#include "clock.h"
#include "GPIO.h"



void SPI_init(void)
{
    nSEL_Dir_out();
    nSEL_Dir_out();
    SpiCLKOut();
    SpiSDOOut();
    SpiSDIIn();	
    
    INT0_init();        //外部中断初始化
    
    //GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow);
    GPIOF->ODR |= GPIO_Pin_0;
    Delay5us(3000);
}

void SPI_OFF(void)
{
 
  GPIOB->ODR |= GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  //GPIOB->ODR &= ~(GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
  
  GPIOF->ODR &= ~GPIO_Pin_0;
  
  //GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_In_FL_No_IT);
  GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Slow);
  Delay5us(300);
}




