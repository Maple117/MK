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
  u8 IdleNumber;//����ָ�����
  u8 SendCommand;       //��ǰ���͵�ָ��
  u8 SendTimes;         //��ǰָ���ط�����
  u8 SendPackID;        //������id
  u8 RecivePackID;      //�Ӱ���id
  u8 Len;               //���ݰ�����
  u8 SendBuf[100];      //���ݻ���
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
    CommandData.SendCommand = 0;    //����һ��ָ�����
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
  
//  if(SendID != CommandData.SendPackID + 1) return;  //��id����
  
  if(ObjID == 0) return;
  if(ObjID != ConfigData.ObjID && Command != COMMAND_LEARN) return;        //����id����
  
  
  sysdata.HeartTimes = 0;       //��������������
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
    if(sysdata.SendLearn == 2)  //����ѧϰ״̬����ѯ������idδ֪
    {
      sysdata.SendLearn = 3;
      sysdata.ObjID = ObjID;    //����id����ʱ��
    }
    else if(sysdata.SendLearn == 4 && ObjID == sysdata.ObjID)   //����ѧϰ״̬��ע��
    {
      ConfigData.ObjID = ObjID;         //����id��ȷ�ϣ�
      ConfigData.GroupID = Data;       //����id
      
      sysdata.SendLearn = 0;            //ѧϰ����
      
      CommandData.SendCommand = 0;    //����һ��ָ�����
      
      FlashWrite();             //�洢����
      SPSet(5);
      LEDSet(6,5);
    }
    return;
  }
  else if(Command == COMMAND_TEST)      //���Է���
  {
    if(sysdata.SendTest == 2)
    {
      sysdata.SendTest = 0;
      if(sysdata.RSSIFLAG==1)
      {
        sysdata.RSSIFLAG=2;
      }       
      CommandData.SendCommand = 0;    //����һ��ָ�����
    }
  }
  else if(Command == COMMAND_HEART)       //��������
  {
    if(sysdata.SendHeart == 2)
    {
      sysdata.SendHeart = 0;
      CommandData.SendCommand = 0;    //����һ��ָ�����
      CommandData.SendTimes = 0;      //����һ��ָ�����
    }
  }
  else if(Command == COMMAND_ON)       //��������
  {
    if(sysdata.SendOn == 2)
    {
      sysdata.SendOn = 0;
      
      if(Data == 1) SPSet(1);
      else if(Data == 2)SPSet(3);   //ʧ��
      CommandData.SendCommand = 0;    //����һ��ָ�����
    }
  }
  else if(Command == COMMAND_OFF)       //�ػ�����
  {

    if(sysdata.SendOff == 2)
    {
      sysdata.SendOff = 0;     
      SPSet(1);
      CommandData.SendCommand = 0;    //����һ��ָ�����
    }
  }
  else if(Command == COMMAND_DELAYOFF)       //�ػ�����
  {
    if(sysdata.SendDelayoff == 2)
    {
      sysdata.SendDelayoff = 0;
      SPSet(1);  
      sysdata.RSSIDATA=0;
      CommandData.SendCommand = 0;    //����һ��ָ�����
    }
  }  
}

void CommandTask(void)
{
  if(sysdata.ReciveTime)
  {
    sysdata.ReciveTime --;
    if(sysdata.ReciveTime == 0)         //���յȴ���ʱ
    {
      if(CommandData.SendCommand == COMMAND_TEST)       //����ģʽ���ط�
      {
        CommandData.SendCommand = 0;    //����һ��ָ�����
        sysdata.SendTest = 0;
        if(sysdata.RSSIFLAG==1)
        {
          sysdata.RSSIFLAG=2;
          sysdata.RSSIDATA=0;
        }  
      }
      else if(CommandData.SendCommand == COMMAND_ON)       //����ָ��ط�
      {
        CommandData.SendCommand = 0;    //����һ��ָ�����
        sysdata.SendOn = 0;
      }
      else if(CommandData.SendCommand == COMMAND_DELAYOFF)       //��ʱ�ػ�ָ��ط�
      {

        CommandData.SendCommand = 0;    //����һ��ָ�����
        sysdata.SendDelayoff  = 0;
      }
      else if(CommandData.SendCommand == COMMAND_OFF)       //�ػ�ָ��ط�
      {
        CommandData.SendCommand = 0;    //����һ��ָ�����
        sysdata.SendOff = 0;
      }
      else if(CommandData.SendCommand == COMMAND_LEARN)  //ֹͣѧϰ
      {
        if(CommandData.SendTimes == 3)    //����ط�����
        {
          CommandData.SendTimes = 0;
          
          CommandData.SendCommand = 0;    //����һ��ָ�����
          sysdata.SendLearn = 0;
        }
        else
        {
          CommandData.SendTimes ++;     //�ط�����
          
          sysdata.SendLearn -= 1;
        }
      }
      else if(CommandData.SendCommand == COMMAND_HEART)  //�����ط���ʱ������
      {
        if(CommandData.SendTimes == SEND_AGAIN_TIMES)    //����ط�����
        {
          CommandData.SendTimes = 0;
          CommandData.SendCommand = 0;    //����һ��ָ�����
          sysdata.SendHeart = 0;       
          sysdata.HeartAlarm = 1;
        }
        else
        {
          CommandData.SendTimes ++;     //�ط�����
          sysdata.WaitSendTime = SEND_AGAIN_DELAY_TIME;        //����ͻ��ʱ
          sysdata.SendHeart = 1;
          HighPowerFlag = 1;       
        }
      }
      
      //��Ҫ��ʱ�ط��ʹ���
      if(sysdata.WaitSendTime)
      {
        AX5043_OFF();     //RF�ػ�
        if(sysdata.NoSleep == 2) sysdata.NoSleep = 0;      //����
      }
      
      return;   //���صȴ�
    }
  }
  
  //û��������Ҫ���͵�
  if(sysdata.SendTest  + sysdata.SendDelayoff+ sysdata.SendLearn  + sysdata.SendHeart + sysdata.SendOn + sysdata.SendOff == 0)
  {
    AX5043_OFF();     //RF�ػ�
    if(sysdata.NoSleep == 2) sysdata.NoSleep = 0;      //����
  }
  
  if(CommandData.SendCommand)   //�ط�
  {
    if(CommandData.SendCommand == COMMAND_TEST)  //���Ͳ���
    {
      if(sysdata.SendTest == 1)
      {
        sysdata.SendTest = 2;
        CommandSend(ConfigData.ObjID,COMMAND_TEST,1);
      }
    }
    else if(CommandData.SendCommand == COMMAND_HEART)  //��������
    {
      if(sysdata.SendHeart == 1)
      {
        sysdata.SendHeart = 2;
        CommandSend(ConfigData.ObjID,COMMAND_HEART,sysdata.BattryAlarm);
      }
    }
    else if(CommandData.SendCommand == COMMAND_ON)  //���Ϳ���
    {
      if(sysdata.SendOn == 1)
      {
        sysdata.SendOn = 2;
        CommandSend(ConfigData.ObjID,COMMAND_ON,0);
        
        LEDSet(8,1);
      }
    }
    
    else if(CommandData.SendCommand == COMMAND_OFF)  //���͹ػ�
    {
      if(sysdata.SendOff == 1)
      {
        sysdata.SendOff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_OFF,0);
        LEDSet(5,1);
      }
    }
    else if(CommandData.SendCommand == COMMAND_LEARN)    //����ѧϰ
    {
      if(sysdata.SendLearn == 1)
      {
        sysdata.SendLearn = 2;
        CommandSend(99999999,COMMAND_LEARN,1);    //ѧϰ״̬����ѯ
      }
      else if(sysdata.SendLearn == 3)
      {
        sysdata.SendLearn = 4;
        CommandSend(sysdata.ObjID,COMMAND_LEARN,2);       //ѧϰ״̬��ע��
      }
      else if(sysdata.SendLearn == 5)
      {
        sysdata.SendLearn = 6;
        CommandSend(ConfigData.ObjID,COMMAND_LEARN,3);       //ѧϰ״̬��ע��
      }
    }
    else if(CommandData.SendCommand == COMMAND_DELAYOFF)  //������ʱ�ػ�
    {
      if(sysdata.SendDelayoff == 1)
      {
        sysdata.SendDelayoff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_DELAYOFF,0);        

      }
    } 
  }
  else  //�׷�
  {
    if(sysdata.SendTest)  //���Ͳ���ָ��
    {
      if(sysdata.SendTest == 1)
      {
        sysdata.SendTest = 2;
        CommandSend(ConfigData.ObjID,COMMAND_TEST,1);
        
        CommandData.SendCommand = COMMAND_TEST;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
      }
    }
    else if(sysdata.SendHeart)  //��������ָ��
    { 
      if(sysdata.SendHeart == 1)
      {
        sysdata.SendHeart = 2;
        CommandSend(ConfigData.ObjID,COMMAND_HEART,sysdata.BattryAlarm); 
        CommandData.SendCommand = COMMAND_HEART;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
      }
    }
    else if(sysdata.SendOn)  //���Ϳ���
    {
      if(sysdata.SendOn == 1)
      {
        sysdata.SendOn = 2;
        CommandSend(ConfigData.ObjID,COMMAND_ON,0);
        CommandData.SendCommand = COMMAND_ON;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
        LEDSet(8,1);
      }
    }
    else if(sysdata.SendDelayoff)  //������ʱ�ػ�
    {
      if(sysdata.SendDelayoff == 1)
      {
        sysdata.SendDelayoff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_DELAYOFF,0);
        
        CommandData.SendCommand = COMMAND_DELAYOFF;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
        LEDSet(3,1);
      }
    }        
    else if(sysdata.SendOff)  //���͹ػ�
    {
      if(sysdata.SendOff == 1)
      {
        sysdata.SendOff = 2;
        CommandSend(ConfigData.ObjID,COMMAND_OFF,0);
        
        CommandData.SendCommand = COMMAND_OFF;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
        
        //sysdata.LEDOffTime = 20;
        LEDSet(5,1);
      }
    }
    else if(sysdata.SendLearn)    //����ѧϰ
    {
      if(sysdata.SendLearn == 1)
      {
        sysdata.SendLearn = 2;
        CommandSend(99999999,COMMAND_LEARN,1);    //ѧϰ״̬����ѯ
        
        CommandData.SendCommand = COMMAND_LEARN;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
      }
      else if(sysdata.SendLearn == 2)
      {
        
      }
      else if(sysdata.SendLearn == 3)
      {
        sysdata.SendLearn = 4;
        CommandSend(sysdata.ObjID,COMMAND_LEARN,2);       //ѧϰ״̬��ע��        
        CommandData.SendCommand = COMMAND_LEARN;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
      }
      else if(sysdata.SendLearn == 4)
      {
        
      }
      else if(sysdata.SendLearn == 5)
      {
        sysdata.SendLearn = 6;
        CommandSend(ConfigData.ObjID,COMMAND_LEARN,3);       //ѧϰ״̬��ע��
        CommandData.SendCommand = COMMAND_LEARN;  //���ķ���ָ��
        CommandData.SendTimes = 0;        //�ط���������
      }
    }
  }
}

void CommandSendDone(void)
{
  sysdata.ReciveTime = 100;      //���յȴ�ʱ��
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
