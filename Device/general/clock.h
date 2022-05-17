#ifndef __CLOCK_H__
#define __CLOCK_H__
#include "Typedef.h"

#ifdef __cpluplus
extern "C" {
#endif


//typedef unsigned long clock_time_t;
extern U16 count;
extern U16 count1;
extern u32 t;
void Timer_Init(void);
void Delayms(u16 time);
void Delay5us(__IO uint16_t nCount);
#ifdef __cplusplus
}
#endif
#endif /* __CLOCK_H__ */

/** @} */
/** @} */
