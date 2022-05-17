#include "Typedef.h"
#include "effectshow.h"
#include "sysdata.h"
#include "GPIO.h"
#include "clock.h"
#include "led.h"
#include "battery.h"
u16 Battrytime=0;

void SPSet(u8 da)
{
  if(da)
  {
    sysdata.BuzzerTime = 20;
    sysdata.BuzzerCyc = da - 1;
  }
  sysdata.Showtime=1;
}

void LongBeep(void)
{
  BUZZER_STATE = 1;
  Delayms(500);
  BUZZER_STATE = 0;
}


void ShowTask(void)
{
  sysdata.Showtime=0;
  
  for(u8 i=0;i<11;i++)
  {
    if(sysdata.LEDTime[i])
    {
      sysdata.LEDTime[i]--;
      if(sysdata.LEDTime[i])
      {
        if(sysdata.LEDFlag[i] == 0) ledshow(i,on);
        else ledshow(i,off);
        sysdata.Showtime++;
      }
      else
      {
        ledshow(i,off);
        if(sysdata.LEDFlag[i])
        {
          sysdata.LEDFlag[i] = 0;
          sysdata.LEDTime[i] = 20;
          sysdata.Showtime++;
        }
        else if(sysdata.LEDCyc[i])
        {
          sysdata.LEDCyc[i] --;   
          sysdata.LEDFlag[i] = 1;
          sysdata.LEDTime[i] = 20;
          sysdata.Showtime++;
        }
      }
    }
  }
  if(sysdata.BuzzerTime)
  {
    sysdata.BuzzerTime --;
    if(sysdata.BuzzerTime)
    {
      if(sysdata.BuzzerFlag == 0) BUZZER_STATE = 1;
      else BUZZER_STATE = 0; 
      sysdata.Showtime++;
    }
    else
    {
      BUZZER_STATE = 0;
      
      if(sysdata.BuzzerFlag)
      {
        sysdata.BuzzerFlag = 0;
        sysdata.BuzzerTime = 20;
        sysdata.Showtime++;
      }
      else if(sysdata.BuzzerCyc)
      {
        sysdata.BuzzerCyc --;
        
        sysdata.BuzzerFlag = 1;
        sysdata.BuzzerTime = 20;
        sysdata.Showtime++;
      }
    }
  }
}



void Battryshow(void)
{
    if(sysdata.Electricityflag>0)
    { 
      
     BatteryCheckOpen();//¿ªÆôµçÁ¿¼ì²â
     if(sysdata.Electricityflag==1)
     {
       Battrytime=1;
       sysdata.Electricityflag=2;
     }
     if(sysdata.Electricityflag==3)
     {
      sysdata.Electricityflag=0;
      BatteryCheckClose();
      switch(sysdata.BattryAlarm)
      {
       case 0:
         LEDSet(10,2);
         LEDSet(8,2);
       break;
       case 3:
         LEDSet(10,2);
         LEDSet(6,2);         
       break;
       case 4:
         LEDSet(10,2);
         LEDSet(4,2);         
       break;
       case 1:
         LEDSet(10,2);
         LEDSet(2,2);         
       break;
        case 2:
         LEDSet(10,2);
         LEDSet(3,2);          
       break;      
       default: break;            
      }
     }
    }
 }
void RSSIshow(void)
{
  if(sysdata.RSSIFLAG==2)
  {
    sysdata.RSSIFLAG=0;
    if(sysdata.RSSIDATA<149)
    {
         LEDSet(1,2);
         LEDSet(9,2);         
    }
    else if(sysdata.RSSIDATA<170)
    {
         LEDSet(1,2);
         LEDSet(6,2); 
    }
    else if(sysdata.RSSIDATA<202)
    {
         LEDSet(1,2);
         LEDSet(4,2);       
    }
    else if(sysdata.RSSIDATA>201)
    {
         LEDSet(1,2);
         LEDSet(2,2);       
    }
    sysdata.RSSIDATA=0;
  }
  
}


  















