#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm8l15x.h"
#include  <iostm8l151k4.h>
#include "Typedef.h"


#define nSEL_Dir_out()   GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast)
#define SpiCLKOut()      GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast)
#define SpiSDOOut()      GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast)
#define SpiSDIIn()       GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT)
#define SDI              PB_IDR_IDR7
#define SDO              PB_ODR_ODR6
#define SCK              PB_ODR_ODR5
#define CSN              PB_ODR_ODR3


#define LED1_INIT()            GPIO_Init(GPIOE, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast)
#define LED2_INIT()            GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast)
#define LED3_INIT()            GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast)
#define LED4_INIT()            GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast)
#define LED5_INIT()            GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast)
#define LED6_INIT()            GPIO_Init(GPIOE, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast)
#define LED7_INIT()            GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast)
#define LED8_INIT()            GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Fast)
#define LED9_INIT()            GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP_High_Fast)
#define LED10_INIT()           GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast)

#define LED1             PE_ODR_ODR0
#define LED2             PE_ODR_ODR1
#define LED3             PE_ODR_ODR2
#define LED4             PE_ODR_ODR3
#define LED5             PE_ODR_ODR4
#define LED6             PE_ODR_ODR5
#define LED7             PD_ODR_ODR0
#define LED8             PD_ODR_ODR1
#define LED9             PD_ODR_ODR2
#define LED10            PD_ODR_ODR3

#define Buzzer_Init()       GPIO_Init(GPIOA, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast)
#define BUZZER_STATE        PA_ODR_ODR7

#define I2CVccState_Init()     GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast)
#define I2C_VCC_STATE          PD_ODR_ODR4

#define KeyON_Init()    GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_In_FL_IT);
#define KEY_ON                 PD_IDR_IDR5
#define KeyOFF_Init()    GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_In_FL_IT);
#define KEY_OFF                PD_IDR_IDR6
#define Key4H_Init()    GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_In_FL_IT);
#define KEY_4H                 PD_IDR_IDR7

#define TestContact_Init()   GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);
#define TEST_CONTACT         PB_IDR_IDR0


#endif
