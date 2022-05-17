#include "Typedef.h"
#include "led.h"
#include "sysdata.h"
#include "GPIO.h"
#include "clock.h"

void ledshow(u8 number,u8 state)
{
//  if(state)
//  {
//    switch(number)
//    {
//     case 1:LED1=open ;break;
//     case 2:LED2=open ;break;
//     case 3:LED3=open ;break;
//     case 4:LED4=open ;break;
//     case 5:LED5=open ;break;
//     case 6:LED6=open ;break;
//     case 7:LED7=open ;break;
//     case 8:LED8=open ;break;
//     case 9:LED9=open ;break;
//     case 10:LED10=open ;break;
//     default: break;
//    }
//  }
//  else
//  {
//   switch(number)
//   {
//     case 1:LED1=close ;break;
//     case 2:LED2=close ;break;
//     case 3:LED3=close ;break;
//     case 4:LED4=close ;break;
//     case 5:LED5=close ;break;
//     case 6:LED6=close ;break;
//     case 7:LED7=close ;break;
//     case 8:LED8=close ;break;
//     case 9:LED9=close ;break;
//     case 10:LED10=close ;break;
//     default: break;
//   }
//  } 
  
     switch(number)
    {
     case 1:LED1=!state ;break;
     case 2:LED2=!state ;break;
     case 3:LED3=!state ;break;
     case 4:LED4=!state ;break;
     case 5:LED5=!state ;break;
     case 6:LED6=!state ;break;
     case 7:LED7=!state ;break;
     case 8:LED8=!state;break;
     case 9:LED9=!state;break;
     case 10:LED10=!state;break;
     default: break;
    } 
  
}


//u8 LEDtime_cntount(void)
//{
//  u16 sum=0;
//  for(u8 i=0;i<11;i++)
//  {
//    sum=sum+sysdata.LEDCyc[i];
//  }
//  if(sum>0)
//  return 1;
//  else
//  return 0;
//}


void LEDSet(u8 number,u8 da)
{
  if(da)
  {
    sysdata.LEDTime[number] = 20;
    sysdata.LEDCyc[number] = da - 1;
    sysdata.Showtime=1;
  }
}














