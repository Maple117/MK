#ifndef _APP_LYR_H_
#define _APP_LYR_H_
#include "Uart.h"

void AppTask(void);

void AppLyr_QueueInit(void);

void AppLyr_AddTask(U8 u8len ,U8 *pData);

typedef struct data_buf_t
{
    struct data_buf_t *next;
    U8 status;
    U8 RtryCnt;
    U8 seq;
    U8 Data[UART_DATA_MAX_LEN];
    U8 DataLth;
}data_buf_t;


enum APP_SEND_PKT_STATE
{
    APP_PKT_UNSEND = 0,
    APP_PKT_SENDING,
    APP_PKT_SEND_SUCCESS,
    APP_PKT_SEND_ERROR
};

#endif
