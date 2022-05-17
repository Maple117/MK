
/*******************************************************************************/
#include "stm8l15x.h"
#include "GPIO.h"
#include "myuart.h"
#include "Typedef.h"
#include "stdio.h"
#include "command.h"
#include "configure.h"
#include "clock.h"
#include "sysdata.h"
#include "stdio.h"



void AppLyr_AddTask(U8 u8len ,U8 *pData);

SUART_STRUCT SUART;

int fputc(int ch, FILE *f)
{
  USART_SendData8(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等
  while (!(USART1->SR & USART_FLAG_TXE));
  return (ch);
}


void UartInit(void)
{
  
  disableInterrupts();         //关闭总中断
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow);//UART-TX
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);//开启USART时钟
  USART_Init(USART1,600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx));//设置USART参数600，8N1，接收/发送
  USART_ITConfig (USART1,USART_IT_RXNE,ENABLE);//使能接收中断
  USART_Cmd (USART1,ENABLE);//使能USART
  USART_ClearITPendingBit (USART1,USART_IT_RXNE);//清中断标志
  USART_ReceiveData8 (USART1);
  enableInterrupts(); 
}

void UartDeInit(void)
{
  USART_Cmd (USART1,DISABLE);//~使能USART
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,DISABLE);//~开启USART时钟
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,DISABLE);
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_IT);
  GPIOC->ODR &= ~GPIO_Pin_3;
}

void UartSendStr(unsigned char * buf,unsigned char len)
{
  for(unsigned char i = 0 ; i < len ; i ++)
  {
    USART_SendData8 (USART1,buf[i]);
    while(!USART_GetFlagStatus (USART1,USART_FLAG_TXE));//等待发送完毕
  }
}

/////////////////////////////////////////////////////

void UartShow(void)
{
//  if(sysdata.usarten==1) 
//  {  
//    if(adderss>0)
//    {
//      u16 m=adderss;
//      adderss=0;
//      I2C_VCC_STATE= 1; 
//      printf("\n\rDeviceID =%ld\n\r",ConfigData.DeviceID );
//      printf("\n\rNumber\tDay\tShake\tTx1\tTx2\tRSSI\tLBT\n\r");
//      while(adderss<m)
//      {
//        IWDG_ReloadCounter(); 
//        RssiRead();
//        day=DATA[0]/16;
//        success=DATA[0]%16;
//        sysdata.SendNumer=DATA[1]/16;
//        sysdata.delaytime=DATA[1]%16;
//        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n\r",adderss,day,success,sysdata.SendNumer,sysdata.delaytime,DATA[2]-255,DATA[3]-255);             
//        DATA[0]=0;
//        DATA[1]=0;
//        DATA[2]=0;
//        DATA[3]=0;
//        adderss++;
//        Delayms(1);
//      }
//      adderss=m;
//      I2C_VCC_STATE= 0;
//    }
//    else
//    {
//      printf("No data record\n\r");
//      Delayms(5);
//    }
//    sysdata.usarten=0;
//    UartDeInit();  
//  }
//  else if(sysdata.usarten==2)
//  {
//    IWDG_ReloadCounter(); 
//    printf("Wait a moment,the record is clearing\n\r");
//    I2C_VCC_STATE=1;
//    RssiClear();
//    printf("Cleared successfully\n\r");
//    I2C_VCC_STATE=0;
//    sysdata.usarten=0;
//    UartDeInit(); 
//  }
//  else if(sysdata.usarten==4)
//  {
//    ConfigData.DeviceID =0;
//    ConfigData.DeviceID +=(SUART.buf[2]-48)*10000000;
//    ConfigData.DeviceID +=(SUART.buf[3]-48)*1000000;
//    ConfigData.DeviceID +=(SUART.buf[4]-48)*100000;
//    ConfigData.DeviceID +=(SUART.buf[5]-48)*10000;
//    ConfigData.DeviceID +=(SUART.buf[6]-48)*1000;
//    ConfigData.DeviceID +=(SUART.buf[7]-48)*100;
//    ConfigData.DeviceID +=(SUART.buf[8]-48)*10;
//    ConfigData.DeviceID +=(SUART.buf[9]-48);
//    printf("DeviceID=%ld",ConfigData.DeviceID);
//    FlashWrite();
//    sysdata.usarten=0;
//    UartDeInit(); 
//  }
}

void UartDataGet(void)
{
  u8 data; 
  data = USART_ReceiveData8 (USART1);
  if(SUART.buf_len == 0)
  {
    if(data == 'S')
    {
      SUART.buf[SUART.buf_len ++] = data;
    }
  }
  else
  {
    if(data != '\n')
    {
      SUART.buf[SUART.buf_len ++] = data;
    }
    else 
    {
      SUART.buf_len = 0;
      sysdata.usarten=SUART.buf[1]-0x30;
    }
  }
}


