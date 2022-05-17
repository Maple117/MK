#ifndef _TYPE_DEFS_H_
#define _TYPE_DEFS_H_

#include "stm8l15x.h"
#include  <iostm8l151k4.h>

//#define NEW_ENABLE

#define XTAL_FREQ   26000000
#define XTAL_FREQ_1     26
#define CHANNEL_BW      100

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned long
#define bool_t unsigned char

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

#ifndef NULL
#define NULL 0x00
#endif

#ifdef NEW_ENABLE

#else


static U8 TestStt;
static U16 TestCount;
static U16 TestCountEnd;
#define CLR_TEST_STT_TIMEOUT()       { TestCount = 0;TestStt = TIMER_IDLE;}
#define SET_TEST_STT_TIMEOUT(itvl)   { CLR_TEST_STT_TIMEOUT(); TestCountEnd = itvl;TestStt = WAIT_TIMER_OUT;}

#endif

#define max 256
extern u8 day,success,frist,mdata,rssi;
extern u8 DATA[4];

typedef enum
{
    TIMER_IDLE,
    WAIT_TIMER_OUT,
    TIMER_OUT
} TimerOutStaus;

#define open   0
#define close  1
#define on    1
#define off   0

#define longpass   4


void TEST_TIMER_INT(void);
void INT0_init(void);
void TestTask(void);
void rssiget(void);

#endif

