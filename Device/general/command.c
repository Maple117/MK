#include "command.h"
#include "stdio.h"
#include "string.h"
#include "applyr.h"
#include "sysdata.h"
#include "radio.h"
#include "Configure.h"
#include "stdlib.h"
#include "Radio_Config.h"
#include "myuart.h"
#include "stdlib.h"
#include "GPIO.h"
#include "led.h"
#include "effectshow.h"
#include "general.h"
#include "clock.h"


#define dataLBT 174
u8 mun=0;
struct
{
  u8 IdleNumber;//空闲指令计数
  u8 SendCommand;       //当前发送的指令
  u8 SendTimes;         //当前指令重发次数
  u8 SendPackID;        //发包的id
  u8 RecivePackID;      //接包的id
  u8 Len;               //数据包长度
  u8 SendBuf[100];      //数据缓存
}CommandData;


ConfigTypeDef ConfigData = 
{
  30000001,
  10000001,
  2,
};

u8 HighPowerFlag;

void CommandStopCF(void)
{
  if(HighPowerFlag)
  {
    HighPowerFlag = 0;
    sysdata.WaitSendTime = 0;
    CommandData.SendTimes = 0;
    CommandData.SendCommand = 0;    //发送一个指令结束
    sysdata.SendHeart = 0;
  }
}

void CommandSend(u32 ObjID,u8 Command,u8 Data)
{
  u8 len;
  u8 check = 0;
  
  
  CommandData.SendPackID += 2;
  if(HighPowerFlag)
  {
    CommandData.SendPackID=0;
  }
  sprintf((char *)(CommandData.SendBuf),"{%08ld,%08ld,%03d,%02d,%d}",\
    ObjID,\
      ConfigData.DeviceID,\
        CommandData.SendPackID,\
          Command,\
            Data);
  
  len = strlen((char const *)(CommandData.SendBuf));
  for(u8 i = 0 ; i < len ; i ++)
  {
    check += CommandData.SendBuf[i];
  }
  CommandData.SendBuf[len ++] = ((check>>4) < 10)?  (check>>4) + '0' : (check>>4) - 10 + 'A';
  CommandData.SendBuf[len ++] = ((check&0xf) < 10)?  (check&0xf) + '0' : (check&0xf) - 10 + 'A';
  CommandData.SendBuf[len ++] = '\r';
  CommandData.SendBuf[len ++] = '\n';
  CommandData.Len = len;
  
  AppLyr_AddTask(CommandData.Len,CommandData.SendBuf);
  
  
}

void CommandUnpack(uint8* pData, uint8 nLength)
{
  u8 check = 0;
  
  if(pData[nLength - 1] == '\n')
  {
    pData[nLength - 1] = '\0';
    nLength --;
  }
  if(pData[nLength - 1] == '\r')
  {
    pData[nLength - 1] = '\0';
    nLength --;
  }
  
  for(u8 i = 0 ; i < nLength - 2 ; i ++)
  {
    check += pData[i];
  }
  if(pData[nLength - 2] != (((check>>4) < 10)?  (check>>4) + '0' : (check>>4) - 10 + 'A')) return;
  if(pData[nLength - 1] != (((check&0xf) < 10)?  (check&0xf) + '0' : (check&0xf) - 10 + 'A')) return;
  
  nLength -= 2;
  
  if(pData[0] != '{' || pData[nLength - 1] != '}') return;
  
  CommandHandle(pData);
  
  pData = pData;
}

void CommandHandle(uint8* pData)
{
  u32 ObjID = 0;
  u32 DeviceID = 0;
  u16 SendID = 0;
  u16 Command = 0;
  u16 Data = 0;
  if(sscanf((const char *)pData,"{%ld,%ld,%d,%d,%d}",&DeviceID,&ObjID,&SendID,&Command,&Data) != 5) return;
  
  if(DeviceID != ConfigData.DeviceID) return;
  
//  if(SendID != CommandData.SendPackID + 1) return;  //包id错误
  
  if(ObjID == 0) return;
  if(ObjID != ConfigData.ObjID && Command != COMMAND_LEARN) return;        //主机id错误
  
  
  sysdata.HeartTimes = 0;       //清心跳报警计数
  sysdata.HeartAlarm = 0;
  if(Command != COMMAND_HEART) 
  {
    CommandData.SendTimes = 0;
  }
  if(HighPowerFlag ==1)
  {
   HighPowerFlag = 0;
   WirelessTxPowerConfigure(configData[POWER_LOCAT]);
  } 
  if(Command == COMMAND_LEARN)
  {
    if(sysdata.SendLearn == 2)  //返回学习状态：查询，主机id未知
    {
      sysdata.SendLearn = 3;
      sysdata.ObjID = ObjID;    //主机id（临时）
    }
    else if(sysdata.SendLearn == 4 && ObjID == sysdata.ObjID)   //返回学习状态：注册
    {
      ConfigData.ObjID = ObjID;         //主机id（确认）
      ConfigData.GroupID = Data;       //组内id
      
      sysdata.SendLearn = 0;            //学习结束
      
      CommandData.SendCommand = 0;    //发送一个指令结束
      
      FlashWrite();             //存储数据
      SPSet(5);
      LEDSet(6,5);
    }
    return;
  }
  else if(Command == COMMAND_TEST)      //测试返回
  {
    if(sysdata.SendTest == 2)
    {
      sysdata.SendTest = 0;
      if(sysdata.RSSIFLAG==1)
      {
        sysdata.RSSIFLAG=2;
      }       
      CommandData.SendCommand = 0;    //发送一个指令结束
    }
  }
  else if(Command == COMMAND_HEART)       //心跳返回
  {
    if(sysdata.SendHeart == 2)
    {
      sysdata.SendHeart = 0;
      CommandData.SendCommand = 0;    //发送一个指令结束
      CommandData.SendTimes = 0;      //发送一个指令结束
    }
  }
  else if(Command == COMMAND_ON)       //开机返回
  {
    if(sysdata.SendOn == 2)
    {
      sysdata.SendOn = 0;
      
      if(Data == 1) SPSet(1);
      else if(Data == 2)SPSet(3);   //失败
      CommandData.SendCommand = 0;    //发送一个指令结束
    }
  }
  else if(Command == COMMAND_OFF)       //关机返回
  {

    if(sysdata.SendOff == 2)
    {
      sysdata.SendOff = 0;     
      SPSet(1);
      CommandData.SendCommand = 0;    //发送一个指令结束
    }
  }
  else if(Command == COMMAND_DELAYOFF)       //关机返回
  {
    if(sysdata.SendDelayoff == 2)
    {
      sysdata.SendDelayoff = 0;
      SPSet(1);  
      sysdata.RSSIDATA=0;
      CommandData.SendCommand = 0;    //发送一个指令结束
    }
  }  
}

void CommandTask(void)
{
  if(sysdata.ReciveTime)
  {
    sysdata.ReciveTime --;
    if(sysdata.ReciveTime == 0)         //接收等待超时
    {
      if(CommandData.SendCommand == COMMAND_TEST)       //测试模式不重发
      {
        CommandData.SendCommand = 0;    //发送一个指令结束
        sysdata.SendTest = 0;
        if(sysdata.RSSIFLAG==1)
        {
          sysdata.RSSIFLAG=2;
          sysdata.RSSIDATA=0;
        }  
      }
      else if(CommandData.SendCommand == COMMAND_ON)       //开机指令不重发
      {
        CommandData.SendCommand = 0;    //发送一个指令结束
        sysdata.SendOn = 0;
      }
      else if(CommandData.SendCommand == COMMAND_DELAYOFF)       //延时关机指令不重发
      {

        CommandData.SendCommand = 0;    //发送一个指令结束
        sysdata.SendDelayoff  = 0;
      }
      else if(CommandData.SendCommand == COMMAND_OFF)       //关机指令不重发
      {
        CommandData.SendCommand = 0;    //发送一个指令结束
        sysdata.SendOff = 0;
      }
      else if(CommandData.SendCommand == COMMAND_LEARN)  //停止学习
      {
        if(CommandData.SendTimes == 3)    //最大重发次数
        {
          CommandData.SendTimes = 0;
          
          CommandData.SendCommand = 0;    //发送一个指令结束
          sysdata.SendLearn = 0;
        }
        else
        {
          CommandData.SendTimes ++;     //重发计数
          
          sysdata.SendLearn -= 1;
        }
      }
      else if(CommandData.SendCommand == COMMAND_HEART)  //心跳重发延时，待机
      {
        if(CommandData.SendTimes == SEND_AGAIN_TIMES)    //最大重发次数
        {
          CommandData.SendTimes = 0;
          CommandData.SendCommand = 0;    //发送一个指令结束
          sysdata.SendHeart = 0;       
          sysdata.HeartAlarm = 1;
        }
        else
        {
          CommandData.SendTimes ++;     //重发计数
          sysdata.WaitSendTime = SEND_AGAIN_DELAY_TIME;        //防冲突延时
          sysdata.SendHeart = 1;
          HighPowerFlag = 1;       
        }
      }
      
      //需要延时重发就待机
      if(sysdata.WaitSendTime)
      {
        AX5043_OFF();     //RF关机
        if(sysdata.NoSleep == 2) sysdata.NoSleep = 0;      //待机
      }
      
      return;   //返回等待
    }
  }
  
  //没有事项需要发送的
  if(sysdata.SendTest  + sysdata.SendDelayoff+ sysdata.SendLearn  + sysdata.SendHeart + sysdata.SendOn + sysdata.SendOff == 0)
  {
    AX5043_OFF();     //RF关机
    if(sysdata.NoSleep == 2) sysdata.NoSleep = 0;      //待机
  }
  
  if(CommandData.SendCommand)   //重发
  {
    if(CommandData.SendCommand == COMMAND_TEST)  //发送测试
    {
      if(sysdata.SendTest == 1)
      {
        sysdata.SendTest = 2;
        CommandSend(ConfigData.ObjID,COMMAND_TEST,1);
      }
    }
    else if(CommandData.SendCommand == COMMAND_HEART)  //发送心跳
    {
      if(sysdata.SendHeart == 1)
      {
        sysdata.SendHeart = 2;
        CommandSend(ConfigData.ObjID,COMMAND_HEART,sysdata.BattryAlarm);
      }
    }
    else if(CommandData.SendCommand == COMMAND_ON)  //发送开机
    {
      if(sysdata.SendOn == 1)
      {
        sysdata.SendOn = 2;
        CommandSend(ConfigData.ObjID,COMMAND_ON,0);
        
        LEDSet(8,1);
      }
    }
    
    else if(CommandData.SendCommand == COMMAND_OFF)  //发送关机
    {
      if(sysdata.SendOff == 1)
      {
        sysdata.SendOff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_OFF,0);
        LEDSet(5,1);
      }
    }
    else if(CommandData.SendCommand == COMMAND_LEARN)    //发送学习
    {
      if(sysdata.SendLearn == 1)
      {
        sysdata.SendLearn = 2;
        CommandSend(99999999,COMMAND_LEARN,1);    //学习状态：查询
      }
      else if(sysdata.SendLearn == 3)
      {
        sysdata.SendLearn = 4;
        CommandSend(sysdata.ObjID,COMMAND_LEARN,2);       //学习状态：注册
      }
      else if(sysdata.SendLearn == 5)
      {
        sysdata.SendLearn = 6;
        CommandSend(ConfigData.ObjID,COMMAND_LEARN,3);       //学习状态：注册
      }
    }
    else if(CommandData.SendCommand == COMMAND_DELAYOFF)  //发送延时关机
    {
      if(sysdata.SendDelayoff == 1)
      {
        sysdata.SendDelayoff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_DELAYOFF,0);        

      }
    } 
  }
  else  //首发
  {
    if(sysdata.SendTest)  //发送测试指令
    {
      if(sysdata.SendTest == 1)
      {
        sysdata.SendTest = 2;
        CommandSend(ConfigData.ObjID,COMMAND_TEST,1);
        
        CommandData.SendCommand = COMMAND_TEST;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
      }
    }
    else if(sysdata.SendHeart)  //发送心跳指令
    { 
      if(sysdata.SendHeart == 1)
      {
        sysdata.SendHeart = 2;
        CommandSend(ConfigData.ObjID,COMMAND_HEART,sysdata.BattryAlarm); 
        CommandData.SendCommand = COMMAND_HEART;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
      }
    }
    else if(sysdata.SendOn)  //发送开机
    {
      if(sysdata.SendOn == 1)
      {
        sysdata.SendOn = 2;
        CommandSend(ConfigData.ObjID,COMMAND_ON,0);
        CommandData.SendCommand = COMMAND_ON;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
        LEDSet(8,1);
      }
    }
    else if(sysdata.SendDelayoff)  //发送延时关机
    {
      if(sysdata.SendDelayoff == 1)
      {
        sysdata.SendDelayoff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_DELAYOFF,0);
        
        CommandData.SendCommand = COMMAND_DELAYOFF;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
        LEDSet(3,1);
      }
    }        
    else if(sysdata.SendOff)  //发送关机
    {
      if(sysdata.SendOff == 1)
      {
        sysdata.SendOff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_OFF,0);
        
        CommandData.SendCommand = COMMAND_OFF;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
        
        //sysdata.LEDOffTime = 20;
        LEDSet(5,1);
      }
    }
    else if(sysdata.SendLearn)    //发送学习
    {
      if(sysdata.SendLearn == 1)
      {
        sysdata.SendLearn = 2;
        CommandSend(99999999,COMMAND_LEARN,1);    //学习状态：查询
        
        CommandData.SendCommand = COMMAND_LEARN;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
      }
      else if(sysdata.SendLearn == 2)
      {
        
      }
      else if(sysdata.SendLearn == 3)
      {
        sysdata.SendLearn = 4;
        CommandSend(sysdata.ObjID,COMMAND_LEARN,2);       //学习状态：注册        
        CommandData.SendCommand = COMMAND_LEARN;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
      }
      else if(sysdata.SendLearn == 4)
      {
        
      }
      else if(sysdata.SendLearn == 5)
      {
        sysdata.SendLearn = 6;
        CommandSend(ConfigData.ObjID,COMMAND_LEARN,3);       //学习状态：注册
        CommandData.SendCommand = COMMAND_LEARN;  //更改发送指令
        CommandData.SendTimes = 0;        //重发技术清零
      }
    }
  }
}

void CommandSendDone(void)
{
  sysdata.ReciveTime = 100;      //接收等待时间
}



void CommandHeart(void)
{
  if(ConfigData.ObjID>10000001)
  {
    if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
    if(sysdata.SendHeart == 0) sysdata.SendHeart = 1;
  }
}

void CommandLearn(void)
{
    CommandStopCF();
  if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
  if(sysdata.SendLearn == 0)
  {
    sysdata.SendOff=0;
    sysdata.SendOn=0;    
    sysdata.SendLearn = 1;
  }
}

void CommandOpenLearn(void)
{
    CommandStopCF();
  if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
  if(sysdata.SendLearn == 0)
  {
    sysdata.SendOff=0;
    sysdata.SendOn=0;
    sysdata.SendLearn = 5;
  }
}

void CommandTest(void)
{
  if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
  if(sysdata.SendTest == 0) sysdata.SendTest = 1;
}

void CommandON(void)
{
    CommandStopCF();
    if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
    if((sysdata.SendOff+sysdata.SendOn +sysdata.SendLearn) == 0) sysdata.SendOn = 1;

}

void CommandOFF(void)
{ 
  
  CommandStopCF();
  if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
  if((sysdata.SendOff+sysdata.SendOn +sysdata.SendLearn) == 0) sysdata.SendOff = 1;

}

void CommandDELAYOFF(void)
{
  
  if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
  if(sysdata.SendDelayoff==0) sysdata.SendDelayoff = 1;
}

void CommandRISS(void)
{
  if(ConfigData.ObjID>10000001)
  {
    sysdata.RSSIFLAG=1; 
    if(sysdata.NoSleep == 0) sysdata.NoSleep = 1;
    if(sysdata.SendTest == 0) sysdata.SendTest = 1;
    sysdata.RSSIDATA=0;
  }
  else
  {
    sysdata.RSSIFLAG=2;
    sysdata.RSSIDATA=0;
  }
  
}

void CommandBattery_inquiry(void)
{
  sysdata.Electricityflag=1;
}
