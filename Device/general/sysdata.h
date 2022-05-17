#ifndef __SYSDATA_H__
#define __SYSDATA_H__
#include "Typedef.h"

#ifdef __cpluplus
extern "C" {
#endif
  


typedef struct
{
  u32 ObjID;            //ѧϰ����ʱid
  
  u16 WaitSendTime;      //����ͻ��ʱ
  u16 Wait5MTime;        //5���ӵȴ�
  u8  CountDownFlag;     //����ʱ��־  
  u32 SendHeartTime;     //�������
  u8 ReciveTime;        //���յȴ�ʱ�䣬���ճ�ʱ
  
  u8 Alarm;             //����
  u8 AlarmFlag;         //������־
  u8 HeartTimes;        //�������߼���
  u8 HeartAlarm;        //����������־
  
  u8 c10;
  u8 f10ms;
  u8 f10ms_SP;
  
  u8 KeyOnTime;
  u8 KeyOffTime;
  u8 Key4HTime;
  u8 KeyOffTimecount;
    
  u8 Showtime;
  u8 LEDTime[11];   //LED����ʱ��(10ms)
  u8 LEDCyc[11];     //ѭ������
  u8 LEDFlag[11];    //��ת��־
  
  u8 BuzzerTime;    //LED����ʱ��(10ms)
  u8 BuzzerCyc;     //ѭ������
  u8 BuzzerFlag;    //��ת��־
  
  u8 SendOn;            //��Ҫ���Ϳ���
  u8 SendOff;           //��Ҫ���͹ػ�
  u8 SendTest;          //��Ҫ���Ͳ���
  u8 SendLearn;         //��Ҫ����ѧϰ
  u8 SendNoLearn;       //��Ҫ���ͽ��
//  u8 SendAlarm;         //��Ҫ���ͱ���
  u8 SendHeart;         //��Ҫ��������
  u8 SendDelayoff;      //��Ҫ������ʱ�ػ�
    
  
  u8 NoSleep;   //�Ƿ�˯��
  
  u8 PVDENFlag;  //��������־
  u8 PVDBatteryFlag;
  
  u8 BattryAlarm;       //��ر���
  u8 BattryAlarmFlag;       //��ر���������־
  u32  BattryAlarmcleartime;       //��ر���������־��ռ�ʱ
  
  u8 Battry;
  u8 Electricity_quantity;     //����
  u8 Electricityflag;          //��������ʶ   
  
  u8 RSSIFLAG;                //RSSI�ı�־
  u8 RSSIDATA;                //RSSI������    
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
