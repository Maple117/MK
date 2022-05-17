#include "stm8l15x.h"
#include "battery.h"
#include "sysdata.h"
#include "command.h"

/*------
标志位 :电量百分比
0  :100%
1  :40%
2  :20%
3  :80%
4  :60%
sysdata.BattryAlarm=0  2.65V  sysdata.BattryAlarm=3      
sysdata.BattryAlarm=3  2.45V  sysdata.BattryAlarm=4 
sysdata.BattryAlarm=4  2.26V  sysdata.BattryAlarm=1
sysdata.BattryAlarm=1  2.05V  sysdata.BattryAlarm=2
-------*/
void PWRDeInit(void)
{
  PWR_PVDCmd(DISABLE);                                     //PVD使能
  PWR_PVDITConfig(DISABLE);                                //PVD中断使能
  PWR_PVDClearITPendingBit();
}

void PWR_Init(void)
{
  PWRDeInit();
  sysdata.PVDBatteryFlag = 0;
  PWR_PVDLevelConfig(PWR_PVDLevel_2V65);                  //PVD设置2.65V
  PWR_PVDCmd(ENABLE);                                     //PVD使能
  PWR_PVDClearITPendingBit();
  PWR_PVDITConfig(ENABLE);                                //PVD中断使能
}

void PWR_Init1(void)
{ 
  PWRDeInit();
  PWR_PVDLevelConfig(PWR_PVDLevel_2V45);                  //PVD设置2.45V
  PWR_PVDCmd(ENABLE);                                     //PVD使能
  PWR_PVDClearITPendingBit();
  PWR_PVDITConfig(ENABLE);                                //PVD中断使能
}

void PWR_Init2(void)
{
  PWRDeInit();
  PWR_PVDLevelConfig(PWR_PVDLevel_2V26);                  //PVD设置2.26V
  PWR_PVDCmd(ENABLE);                                     //PVD使能
  PWR_PVDClearITPendingBit();
  PWR_PVDITConfig(ENABLE);                                //PVD中断使能
}

void PWR_Init3(void)
{
  PWRDeInit();
  PWR_PVDLevelConfig(PWR_PVDLevel_2V05);                  //PVD设置2.05V
  PWR_PVDCmd(ENABLE);                                     //PVD使能
  PWR_PVDClearITPendingBit();
  PWR_PVDITConfig(ENABLE);                                //PVD中断使能
}

void BatteryCheckOpen(void)
{
  if(sysdata.PVDENFlag == 0)
  {
    sysdata.PVDENFlag = 1;
    switch(sysdata.BattryAlarm)
    {
    case 0:PWR_Init();break;
    case 1:PWR_Init3();break;
    case 2: break;
    case 3:PWR_Init1();break;
    case 4:PWR_Init2();break;
    default: break;       
    }
  }
}
  
void BatteryCheckClose(void)
{
  if(sysdata.PVDENFlag == 1)
  {
    sysdata.PVDENFlag = 0;
    PWRDeInit();      //关闭电量检测
  }
}

void PVDRespond(void)
{
  if(sysdata.PVDBatteryFlag == 0)
  {
    sysdata.PVDBatteryFlag = 15;
  }
  if(sysdata.BattryAlarm == 0)
  {
    sysdata.BattryAlarm = 3;
    PWRDeInit();
    CommandHeart();      //心跳
  }
  else if(sysdata.BattryAlarm == 3)
  {
    sysdata.BattryAlarm = 4;
    PWRDeInit();
    CommandHeart();      //心跳
  }
  else if(sysdata.BattryAlarm == 4)
  { 
    PWRDeInit();
    if(sysdata.BattryAlarmFlag<=3)
    {
      sysdata.BattryAlarmFlag++;
      sysdata.BattryAlarmcleartime=1;
    }
    if(sysdata.BattryAlarmFlag>=3)
    {
      sysdata.BattryAlarmFlag=0;
      sysdata.BattryAlarm = 1;
      CommandHeart();      //心跳        
    }
  }    
  else if(sysdata.BattryAlarm == 1)
  {
    sysdata.BattryAlarm = 2;
    PWRDeInit();
    CommandHeart();      //心跳
  }    
}

//低电压多次检测///////////////////////////////////////////
void LowVoltageConfirmation(void)
{
  if(sysdata.BattryAlarmcleartime>0)
  {
    sysdata.BattryAlarmcleartime++;
    if(sysdata.BattryAlarmcleartime>=(SENDHEARTTIME+120))
    {
      sysdata.BattryAlarmFlag=0;
      sysdata.BattryAlarmcleartime=0;
    }
  }
}
