/**
  ******************************************************************************
  * @file    SPI/SPI_FLASH/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    13-May-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "clock.h"
#include "Uart.h"
#include "App_Uart.h"
#include "WakeUp.h"
#include "DlkLyr.h"
#include "GPIO.h"
#include "sysdata.h"
#include "command.h"
#include "battery.h"
#include "general.h"
#include "battery.h"    
#include "effectshow.h"
/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup SPI_FLASH
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
__IO ITStatus UsartTransferStatus = RESET;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  RTC_ClearITPendingBit(RTC_IT_WUT);
  //RTC_WakeUpCmd(DISABLE);//使能唤醒功
  LowVoltageConfirmation();
  BattryAlarm();
  HeartAlarm();
  HeartTimeCheck();
  WaitTime();
  LongKeyCheck();
  LBT_work();


   
  
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */

INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  if(PWR_GetFlagStatus(PWR_FLAG_PVDIF) != RESET)
  {
    PWR_PVDClearITPendingBit();
    PVDRespond();
  }
}    


/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler, 6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler, 7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
    //EXTI_ClearITPendingBit(EXTI_IT_Pin0);
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  
  EXTI_ClearITPendingBit(EXTI_IT_Pin0);

}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
 
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
#include "myuart.h"
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  //UartInit();
  
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT);
//  sysdata.usarten=3;
//  UartInit();
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}


#ifndef NEW_ENABLE
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
#endif
{
#ifndef NEW_ENABLE
    EXTI_ClearITPendingBit(EXTI_IT_Pin3);
#endif
    
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  
  EXTI_ClearITPendingBit(EXTI_IT_Pin5);
  KeyOnRespond();
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  EXTI_ClearITPendingBit(EXTI_IT_Pin6);
  KeyOffRespond();
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  
  EXTI_ClearITPendingBit(EXTI_IT_Pin7);
  Key4HRespond();
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler, 16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
{
    TIM2_ClearITPendingBit(TIM2_IT_Update);
    count++;
    count1++; 
    sysdata.c10 ++;
    Send_TIMER_INT();
    UART_TIMER_INT();
    TEST_TIMER_INT(); 

    if(sysdata.c10 == 10)
    {    


      sysdata.c10 = 0;
      sysdata.f10ms = 1;
      sysdata.f10ms_SP = 1;
    }
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler, 20)
{
    ;
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler, 22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    /* Cleat Interrupt Pending bit */
    // TIM1_ClearITPendingBit(TIM1_IT_Update);

    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
// CmRecv_State = Cm_RX_IDLE;

/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler, 24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/*
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
    TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
#ifdef USE_STM8L1526_EVAL
    /* Write one byte to the transmit data register */
    /* Disable the USART Transmit Complete interrupt */
#endif /* USE_STM8L1526_EVAL */

    USART_ClearITPendingBit(USART1, USART_IT_TC);
    UartSendAsync();
    USART_ClearITPendingBit(USART1, USART_IT_TXE);
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
  */USART_ClearITPendingBit (USART1,USART_IT_RXNE);//清中断标志
  
  UartDataGet();
      
   
//   if(SUART.buf_len>0) 
//   {
//     SUART.buf[SUART.buf_len]=data ;
//     SUART.buf_len++;
//     if(SUART.buf[2]=='r'&&SUART.buf[3]=='e'&&SUART.buf[4]=='c'&&SUART.buf[5]=='o'&&SUART.buf[6]=='r'&&SUART.buf[7]=='d')
//     {
//       sysdata.usarten=2;
//       SUART.buf_len=0;
//     }   
//     if(SUART.buf_len>9)
//     {
//        SUART.buf_len=0;
//     }     
//   }
//   if(SUART.buf_len == 0)
//   {
//     if(data == '{')
//     {
//      SUART.buf[SUART.buf_len ++] = data;
//     }
//   }
//   else
//   {
//     if(SUART.buf_len < 15)
//     {
//      SUART.buf[SUART.buf_len ++] = data;
//     }
//     else SUART.buf_len = 0;
//   }
   //USART_SendData8 (USART1,USART_ReceiveData8 (USART1));//将接收到的数据发送出去
   
   
   
//    /* In order to detect unexpected events during development,
//       it is recommended to set a breakpoint on the following instruction.
//    */
//#ifdef USE_STM8L1526_EVAL
//    /* Read one byte from the receive data register and send it back */
//    /* Disable the Read data register not empty interrupt */
//#endif /* USE_STM8L1526_EVAL */
//
//    UartRxISR();
//    UartReceived = 1;
//    // OVERRUN FLAG
//    USART_GetFlagStatus(USART1, USART_FLAG_OR);
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @}
  */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/