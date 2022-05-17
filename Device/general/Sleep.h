#ifndef _SLEEP_H_
#define _SLEEP_H_

void SleepTask(void);
extern U8 SleepFlag;

void WAKED_TIMER_INT(void);

#endif