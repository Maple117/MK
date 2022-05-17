#include "Typedef.h"
#include "sysdata.h"
#include "GPIO.h"
#include "led.h"
#include "command.h"
#include "general.h"
#include "effectshow.h"
#include "Radio.h"
#include "Spi.h"
#include "AX5043.h"
#include "AppLyr.h"
#include "clock.h"
#include "Configure.h"
#include "stdio.h"


static u16 HeartAlarmTime = 0;   //������������
static u32 HeartTime = SENDHEARTTIME_INIT;
//������������///////////////////////////////////////// 
void SystemClock_Init(void);



//��������///////////////////////////////////////////
void BattryAlarm(void)
{
  if(sysdata.PVDBatteryFlag && sysdata.Alarm == 0)        //��������
  {
    sysdata.PVDBatteryFlag ++;
    if(sysdata.BattryAlarm == 2)
    {
      if(sysdata.PVDBatteryFlag >= 30)
      {
        sysdata.PVDBatteryFlag = 1;
        SPSet(1);
        LEDSet(7,1);
      }
    }
  }
}

//��������///////////////////////////////////////////
void HeartAlarm(void)
{
  if(sysdata.PVDBatteryFlag == 0 && sysdata.HeartAlarm)        //�ܱ�����������������
  {
    HeartAlarmTime ++;
    if(HeartAlarmTime >= 15)
    {
      HeartAlarmTime = 0;
      SPSet(4);
      LEDSet(7,4);
    }
  }
  else HeartAlarmTime = 0;
}
  
//����ʱ����///////////////////////////////////////////
void HeartTimeCheck(void)
{
  HeartTime ++;
  if(HeartTime >= sysdata.SendHeartTime&&ConfigData.ObjID>10000001)
  {
    sysdata.delaytime=0;
    HeartTime = 0;
    CommandHeart();      //����
  }
}

//�������///////////////////////////////////////////
void LongKeyCheck(void)
{
  if(sysdata.KeyOffTime != 0 && sysdata.KeyOffTime <longpass)
  {
    sysdata.KeyOffTime ++;
    if(sysdata.KeyOffTime >= longpass)
    {
      CommandLearn();      //ѧϰ
    }
  }
  if(sysdata.KeyOnTime != 0 && sysdata.KeyOnTime <longpass)
  {
    sysdata.KeyOnTime ++;
    if(sysdata.KeyOnTime >= longpass)
    {
      CommandBattery_inquiry();
    }
  }
  if(sysdata.Key4HTime != 0 && sysdata.Key4HTime < longpass)
  {
    sysdata.Key4HTime ++;
    if(sysdata.Key4HTime >= longpass)
    {  
      CommandRISS();
    }
  }  
}


//����ͻ�ȴ�///////////////////////////////////////////
void WaitTime(void)
{
  if(sysdata.WaitSendTime)    //����ͻ��ʱ����ʱ
  {
    sysdata.WaitSendTime --;
    if(sysdata.WaitSendTime == 0) sysdata.NoSleep = 1;        //RF����
  }
  if(sysdata.Electricityflag==2)
  {
    sysdata.Electricityflag=3;
  }
}

//////��������////////////////////////////////////////////

void KeyOffRespond(void)
{
  if(KEY_OFF == 0)
    
  {
     sysdata.KeyOffTime = 1;
  }
  else
  {
    if(sysdata.KeyOffTime <= (longpass-1))    
    {
      CommandOFF();      //�ػ�
    }
    sysdata.KeyOffTime = 0;
  }
}

void KeyOnRespond(void)
{
  if(KEY_ON == 0)
  {
     sysdata.KeyOnTime = 1;
  }
  else
  {
    if(sysdata.KeyOnTime <= (longpass-1))     
    {
      CommandON();      //����
    }
    sysdata.KeyOnTime = 0;
  }
}

void Key4HRespond(void)
{
  if(KEY_4H == 0)
  {
     sysdata.Key4HTime = 1;
  }
  else
  {
    if(sysdata.Key4HTime <= (longpass-1))     
    {
      CommandDELAYOFF();
    }
    sysdata.Key4HTime = 0;
  }
}

void LBT_work(void)
{
  if(sysdata.delay1s>0)
  {  
    sysdata.delay1s--;
    if(sysdata.delay1s==1)
    {
     CommandHeart();    //����     
    }
  }
  
}

//5043��ʼ��////////////////////////////////////////////
void AX5043_Init(void)
{
  if(sysdata.NoSleep)
  {
    disableInterrupts();         //�ر����ж�
    SystemClock_Init();
    Timer_Init();  //��ʱ����ʼ��
    SPI_init();         //spi��ʼ��
    if (InitAX5043() != AXRADIO_ERR_NOERROR) while (1);            //5043��ʼ�� 
    AX5043ReceiverON();
    AppLyr_QueueInit();         //��Ϣ���г�ʼ��
    enableInterrupts();         //�������ж�
    SET_TEST_STT_TIMEOUT(100);    //��ʱ�������� 
  }
  else
  {  
    if (InitAX5043() != AXRADIO_ERR_NOERROR) while (1);            //5043��ʼ��
    AX5043_OFF();  
  }
  
}


void DataRecord(void)
{
//  I2C_VCC_STATE=1;
//  DATA[0]=day*16+success;
//  DATA[1]=sysdata.SendNumer*16+sysdata.delaytime;
//  if(rssi==0)rssi=0xff;
//  DATA[2]=rssi;
//  if(sysdata.LBT==0)sysdata.LBT=0xff;
//  DATA[3]=sysdata.LBT;  
//  RssiWrite();
////  printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n\r",adderss,day,success,sysdata.SendNumer,sysdata.delaytime,DATA[2]-255,DATA[3]-255);
//  adderss++;
//  if(adderss >=max)
//  {
//    adderss=0;
//  }      
//  RssiAddressWrite();
//  I2C_VCC_STATE=0;
//  sysdata.LBT=0;
}
