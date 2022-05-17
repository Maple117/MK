#include "stm8l15x.h"
#include "stm8l15x_clk.h"
#include "Spi.h"
#include "clock.h"
#include "Configure.h"
#include "Typedef.h"
#include "command.h"
#include "sysdata.h"
#include "myuart.h"
#include "stdio.h"                
#include "GPIO.h" 
#include "AppLyr.h"
#include "DlkLyr.h"
#include "battery.h"
#include "test.h"
#include "general.h"
#include "effectshow.h"
#include "Radio.h"
#include "AX5043.h"


static U8 TestStt;
static U16 TestCount;
static U16 TestCountEnd;



#define CLR_TEST_STT_TIMEOUT()       { TestCount = 0;TestStt = TIMER_IDLE;}
#define SET_TEST_STT_TIMEOUT(itvl)   { CLR_TEST_STT_TIMEOUT(); TestCountEnd = itvl;TestStt = WAIT_TIMER_OUT;}

void TestTask(void);
void WriteRTCWakeUp(uint32_t Alarm);
void SystemClock_Init(void);
void INT0_init(void);
void Port_Init(void);
void IWDGInit(void);
void RTC_Config(void);
void LowPowerInit(void);

u8 DATA[4]={0,0,0,0};
u8 day,success,frist,mdata,rssi;

void main(void)
{
  SystemClock_Init();         //ϵͳʱ�ӳ�ʼ��
  sysdata.SendHeartTime = SENDHEARTTIME;
  LowPowerInit();
  PWR_Init();
  PWRDeInit();
  RTC_Config();         //�ⲿ32.768Kʼ�ճ�ʼ�� + RTCʵʱʱ������
  WriteRTCWakeUp(2047);         //RTC��ʱ���ѣ�2048 = 1�뻽��һ��
  FlashRead();        //Flash��ʼ��
  Timer_Init();      //��ʱ����ʼ��  
  enableInterrupts();         //�������ж�  
  /*-----------�ն��Ƿ���¼ID�ж�-------------------*/
  DeviceIDCheck();
  LongBeep();
  IWDGInit();           //���Ź���ʼ��
  AX5043_Init();
  TestMod(); 
  while (1)
  {
   IWDG_ReloadCounter();               //ι��

    if(sysdata.NoSleep )
    {
      if(sysdata.NoSleep == 1)
      {
        sysdata.NoSleep = 2;
        BatteryCheckOpen();//�����������
        AX5043_Init();//��ʼ��5043
      }
      AppTask();              //5043���ݽ��շ���
      DlkTask();              //���Ϸ���
      TestTask();
      if(sysdata.f10ms)
      {
        sysdata.f10ms = 0;
        CommandTask();          //���ݴ������
      }
    }
    Battryshow();  
    if(sysdata.f10ms_SP)
    {
      sysdata.f10ms_SP = 0;
      BatteryCheckClose();      //�رյ������
      ShowTask();          //LED,SP�������|| sysdata.usarten

    }
    if(!(sysdata.NoSleep || sysdata.Showtime ||sysdata.RSSIFLAG||sysdata.Electricityflag))
    {
      BatteryCheckClose();      //�رյ������ 
      halt();
    }
  }
}

int8_t axradio_phy_rssioffset = 64;
void rssiget(void)
{
   int8_t r=SpiReadSingleAddressRegister(REG_AX5043_RSSI);
   mdata=r - (int16_t)axradio_phy_rssioffset;
}

void Port_Init(void)
{
   LED1_INIT();
   LED2_INIT();
   LED3_INIT();
   LED4_INIT();
   LED5_INIT();
   LED6_INIT();
   LED7_INIT();
   LED8_INIT();
   LED9_INIT();
   LED10_INIT();
   Buzzer_Init();
}

void INT0_init(void)
{
  GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_In_PU_IT);

  EXTI_SetPinSensitivity((EXTI_Pin_TypeDef)EXTI_Pin_4, EXTI_Trigger_Rising);
  EXTI_ClearITPendingBit (EXTI_IT_Pin4);//����жϱ�־
}

void RTC_Config(void)
{
  CLK_LSEConfig(CLK_LSE_ON);//ʹ��LSE
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);//RTCʱ��ԴLSE��1��Ƶ=32.768K
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);//�ȴ�LSE����
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTCʱ���ſ�ʹ��
  RTC_WaitForSynchro();
  RTC_ClearITPendingBit(RTC_IT_ALRA);
  RTC_ITConfig(RTC_IT_ALRA,ENABLE);//�����ж�
}

void SystemClock_Init(void)
{
  Delay5us(30000);
  //Select HSE as system clock source
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  // HSI = 16MHz
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  //High speed external clock prescaler:
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)
  {
    ;
  }
}
void WriteRTCWakeUp(uint32_t Alarm)
{
  //RTC_AlarmCmd(DISABLE);
  RTC_WakeUpCmd(DISABLE);
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//32.768K/16=2.048k=0.488ms
  //RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);//32.768K/16=2.048k=0.488ms
  RTC_ITConfig(RTC_IT_WUT, ENABLE);//�����ж�
  //RTC_SetWakeUpCounter(120);//120S
  RTC_SetWakeUpCounter(Alarm);//2040/60=34Min
  RTC_ITConfig(RTC_IT_WUT, ENABLE);//���Ѷ�ʱ���ж�ʹ��
  RTC_WakeUpCmd(ENABLE);//RTC����ʹ��
}


void CLK_init(void)
{
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);         // 16M�ڲ�RC��8��Ƶ��ϵͳʱ��Ϊ2M 
}

void IWDGInit(void)
{
  IWDG_Enable();
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(255);
  IWDG_ReloadCounter();
  
}

void LowPowerInit(void)
{
  //����IO����͵�ƽ
  GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);//& ~(0xf8)
  GPIO_Init(GPIOC, GPIO_Pin_All , GPIO_Mode_Out_PP_Low_Slow);//& ~(0x03)
  GPIO_Init(GPIOD, GPIO_Pin_All & ~(GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6 |GPIO_Pin_7), GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_All & ~(GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4 ), GPIO_Mode_Out_PP_Low_Slow); //& ~(0x81)
  GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  Port_Init();        //�˿ڳ�ʼ��
  EXTI_DeInit();
  
  TestContact_Init();
  SPI_init();         //spi��ʼ��
  I2CVccState_Init();
  
//  UartInit();
  
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_IT);
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
  EXTI_SetPinSensitivity (EXTI_Pin_2,EXTI_Trigger_Rising_Falling);//�ⲿ�жϣ��½��ش���
  

  KeyON_Init();
  KeyOFF_Init();
  Key4H_Init();
  EXTI_SetPinSensitivity (EXTI_Pin_5,EXTI_Trigger_Rising_Falling);//�ⲿ�жϣ��½��ش���
  EXTI_SetPinSensitivity (EXTI_Pin_6,EXTI_Trigger_Rising_Falling);//�ⲿ�жϣ��½��ش���
  EXTI_SetPinSensitivity (EXTI_Pin_7,EXTI_Trigger_Rising_Falling);//�ⲿ�жϣ��½��ش���

  PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��
  PWR_UltraLowPowerCmd(ENABLE);//���͹���
  
  Delay5us(60000);
  Delay5us(60000);
  Delay5us(60000);
  Delay5us(60000);

  EXTI_ClearITPendingBit (EXTI_IT_Pin5);//����жϱ�־
  EXTI_ClearITPendingBit (EXTI_IT_Pin6);//����жϱ�־
//  EXTI_ClearITPendingBit (EXTI_IT_Pin7);//����жϱ�־
}


void FastLowPowerInit(void)
{
  PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��
  PWR_UltraLowPowerCmd(ENABLE);//���͹���
}

void TEST_TIMER_INT(void)
{
    if (TestStt == WAIT_TIMER_OUT)
    {
        TestCount++;
        if (TestCountEnd == TestCount)
        {
            TestStt = TIMER_OUT;
        }
    }
}

void TestTask(void)
{
#define TXLEN 8
    if (TestStt == TIMER_OUT)
    {
        CLR_TEST_STT_TIMEOUT();
        
        
        
        SET_TEST_STT_TIMEOUT(100);
        
    }
}

