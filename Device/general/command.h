#ifndef __COMMAND_H_
#define __COMMAND_H_

#ifdef __cpluplus
extern "C" {
#endif
  
#include "typedef.h"
  
typedef struct
{
  u32 DeviceID;         //设备id
  u32 ObjID;            //主机id
  u8 GroupID;           //组内id
  u8 State;             //存储状态
  u8 S1;             //
  u8 S2;             //
}ConfigTypeDef;

extern ConfigTypeDef ConfigData;


//#define SEND_GROUP_DELAY 30     //每组重发间隔时间(S)
//#define SENDHEARTTIME  ((u32)24*(u32)3600)     //心跳间隔时间(S)
#define SENDHEARTTIME  ((u32)24*(u32)3600)      //心跳间隔

#define SENDHEARTTIME_INIT (SENDHEARTTIME - 60)
#define SEND_AGAIN_TIMES 11     //重发次数
#define SEND_AGAIN_DELAY_TIME (300-1)    //重发间隔 300s
#define SEND_AGAIN_DELAY_TIME_ALARM  (15-1)    //重发间隔 15s
#define MORE_TX_POWER 10
extern u8 HighPowerFlag;
void CommandStopCF(void);

#define COMMAND_TEST    1
#define COMMAND_HEART   2
#define COMMAND_LEARN   3
//#define COMMAND_ALARM   4
#define COMMAND_ON      5
#define COMMAND_OFF     6
#define COMMAND_BATTRY  7
#define COMMAND_DELAYOFF 8
  
void CommandOn(void);
void CommandOff(void);
void CommandHeart(void);
void CommandLearn(void);
void CommandOpenLearn(void);
void CommandTest(void);
void CommandON(void);
void CommandOFF(void);
void CommandCHILDLOCK(void);
void CommandDELAYOFF(void);


void SetCommandDataFromRF(u8 Command,u8 data);


void CommandTask(void);
void CommandSend(u32 ObjID,u8 Command,u8 Data);
void CommandAskSend(u32 ObjID,u8 SendId,u8 Command,u8 Data);
void CommandSendDone(void);
void CommandUnpack(uint8* pData, uint8 nLength);
void CommandHandle(uint8* pData);
void CommandBattery_inquiry(void);
void CommandRISS(void);    
#ifdef __cplusplus
}
#endif
#endif /* __COMMAND_H_ */



