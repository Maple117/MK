#include "Typedef.h"
#include "test.h"
#include "sysdata.h"
#include "GPIO.h"
#include "clock.h"
#include "command.h"
#include "Spi.h"
#include "Uart.h"
#include "Radio.h"
#include "AX5043.h"
#include "AppLyr.h"
#include "App_Uart.h"
#include "DlkLyr.h"
#include "wwdg.h"
#include "Configure.h"
#include "Sleep.h"
#include "WakeUp.h"
#include "Lopsc.h"
#include "myuart.h"




u16 count12=0;
u16 Number = 0;
u16 Time = 0;
void TestMain(void)
{
  Timer_Init();      //定时器初始化
  SPI_init();         //spi初始化
  AppLyr_QueueInit();         //消息队列初始化
  SET_TEST_STT_TIMEOUT(100);    //计时服务启动
  
  LED1=open;
  LED2=open;
  LED4=open; 
  LED6=open;  
  LED8=open;
  LED10=open; 
  Delayms(500); 
  LED1=close;
  LED2=close;
  LED4=close; 
  LED6=close;  
  LED8=close;
  LED10=close;  
  Delayms(500); 
  
  LED3=open;
  LED5=open; 
  LED7=open;  
  LED9=open;
  Delayms(500);       
  LED3=close;
  LED5=close; 
  LED7=close;  
  LED9=close;  
  Delayms(500); 
  
  if (InitAX5043() != AXRADIO_ERR_NOERROR) while (1);            //5043初始化
  SetReceiveMode();
  AX5043ReceiverON();
  SpiWriteLongAddressRegister(REG_AX5043_FRAMING  , 0x06 );
  while(1)
  {
    IWDG_ReloadCounter();               //喂狗    2019 11 23新加的
    AppTask();              //5043数据接收服务
    DlkTask();              //故障服务
    TestTask();
    if(sysdata.f10ms)
    {
      sysdata.f10ms = 0;
      
      if(Time < 500)
      {
        Time ++;
        if(Time >= 500)
        {
          if(count12 > 10)
          {
            LED2=open;
          }
          else
          {
            LED3=open;
          }
        }
      }
    }
  }
}



void DeviceIDCheck(void)
{
  if(ConfigData.DeviceID == 30000001)
  {
    LED9=open ;
    Delayms(200);
    LED9_INIT();
  }
  else
  {   
    LED8=open ;
    Delayms(200);
    LED8_INIT();
  }
}

void TestMod(void)
{
  if( TEST_CONTACT== 0) TestMain();
  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Slow);  
}