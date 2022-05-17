#ifndef __SYSDATA_H__
#define __SYSDATA_H__
#include "Typedef.h"

#ifdef __cpluplus
extern "C" {
#endif
  


typedef struct
{
  u32 ObjID;            //学习用临时id
  
  u16 WaitSendTime;      //防冲突延时
  u16 Wait5MTime;        //5分钟等待
  u8  CountDownFlag;     //倒计时标志  
  u32 SendHeartTime;     //心跳间隔
  u8 ReciveTime;        //接收等待时间，接收超时
  
  u8 Alarm;             //报警
  u8 AlarmFlag;         //报警标志
  u8 HeartTimes;        //心跳掉线计数
  u8 HeartAlarm;        //心跳报警标志
  
  u8 c10;
  u8 f10ms;
  u8 f10ms_SP;
  
  u8 KeyOnTime;
  u8 KeyOffTime;
  u8 Key4HTime;
  u8 KeyOffTimecount;
    
  u8 Showtime;
  u8 LEDTime[11];   //LED点亮时间(10ms)
  u8 LEDCyc[11];     //循环次数
  u8 LEDFlag[11];    //翻转标志
  
  u8 BuzzerTime;    //LED点亮时间(10ms)
  u8 BuzzerCyc;     //循环次数
  u8 BuzzerFlag;    //翻转标志
  
  u8 SendOn;            //需要发送开机
  u8 SendOff;           //需要发送关机
  u8 SendTest;          //需要发送测试
  u8 SendLearn;         //需要发送学习
  u8 SendNoLearn;       //需要发送解绑
//  u8 SendAlarm;         //需要发送报警
  u8 SendHeart;         //需要发送心跳
  u8 SendDelayoff;      //需要发送延时关机
    
  
  u8 NoSleep;   //是否睡眠
  
  u8 PVDENFlag;  //电量检测标志
  u8 PVDBatteryFlag;
  
  u8 BattryAlarm;       //电池报警
  u8 BattryAlarmFlag;       //电池报警计数标志
  u32  BattryAlarmcleartime;       //电池报警计数标志清空计时
  
  u8 Battry;
  u8 Electricity_quantity;     //电量
  u8 Electricityflag;          //电量检测标识   
  
  u8 RSSIFLAG;                //RSSI的标志
  u8 RSSIDATA;                //RSSI的数据    
  u8 SendNumer;    
  u8 usarten;  

  
  u8 delaytime;
  u8 delay1s;
  u8 LBT;
  
}SYSDATA_STRUCT;


extern SYSDATA_STRUCT sysdata;
extern u16 adderss;
#ifdef __cplusplus
}
#endif
#endif /* __SYSDATA_H__ */

/** @} */
/** @} */
