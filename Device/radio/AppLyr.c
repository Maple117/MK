#include "stm8l15x.h"
#include "Radio.h"
#include "list.h"
#include "Uart.h"
#include <string.h>
#include "DlkLyr.h"
#include "AppLyr.h"
#include "command.h"

#define gMaxBufNum  2
#define RetryCount 2

data_buf_t  DATA_BUF[gMaxBufNum];
LIST(AppLyr_QueueList);
LIST(AppLyr_FreeQueueList);
void *AppLyr_GetFreeQueueItem(void);
static void AppLyr_RomoveFromQueue(void *pPkt);
static void AppLyr_AddToQueue(void *pPkt);
static U8 AppLyr_DlkLyrPkting(data_buf_t *pkt, Dlk_ReqNInd *SendStruct);
static void AppLyr_SndToDlkLyr(void);
extern u16 count12;
void AppTask(void)
{
    AppLyr_SndToDlkLyr();
    if (ubReceiveFlag == 0x01)
    {
        ubReceiveFlag = 0x0;
        //vWriteToSerialPortAsync(RXBuff+1,RxLen-1);
        CommandUnpack(RXBuff+1,RxLen-1);
        if(RXBuff[29]=='0'&&RXBuff[30]=='9')
        {
          count12++;
        }
//        LED_RX = 1;
        //SetReceiveMode();
       // AX5043ReceiverON();
    }
    
}

void AppLyr_QueueInit(void)
{
    U8 i;
    list_init(AppLyr_QueueList);
    list_init(AppLyr_FreeQueueList);
    for (i = 0; i < gMaxBufNum; i++)
    {
        list_remove(AppLyr_FreeQueueList,&DATA_BUF[i]);
        list_add(AppLyr_FreeQueueList,&DATA_BUF[i]);
    }
    DLK_StatusInit();
}

void *AppLyr_GetFreeQueueItem(void)
{
    data_buf_t *item;

    item = list_chop(AppLyr_FreeQueueList);

    if (item == NULL)
    {
        item = list_tail(AppLyr_QueueList);
        AppLyr_RomoveFromQueue(item);
    }
    return item;
}

static void AppLyr_RomoveFromQueue(void *pPkt)
{
    list_remove(AppLyr_QueueList, pPkt);
    list_remove(AppLyr_FreeQueueList, pPkt);
    list_add(AppLyr_FreeQueueList, pPkt);
}

static void AppLyr_AddToQueue(void *pPkt)
{
    list_remove(AppLyr_FreeQueueList, pPkt);
    list_remove(AppLyr_QueueList, pPkt);
    list_add(AppLyr_QueueList, pPkt);
}

static U8 AppLyr_DlkLyrPkting(data_buf_t *pkt, Dlk_ReqNInd *SendStruct)
{
    SendStruct->DlkType = DATA_PRIMITIVE_REQUEST;

    if (pkt->DataLth  > UART_DATA_MAX_LEN)
    {
        return FALSE;
    }
    SendStruct->DlkSeq = pkt->seq;
    SendStruct->len = pkt->DataLth;
    SendStruct->buf = pkt->Data;
    return TRUE;
}

void AppLyr_AddTask(U8 u8len ,U8 *pData)
{
    static U8 seq = 0;
    data_buf_t *pAddPkt;
    if (u8len > UART_DATA_MAX_LEN)
    {
        return;
    }
    pAddPkt = (data_buf_t *)AppLyr_GetFreeQueueItem();
    pAddPkt->DataLth = u8len;
    pAddPkt->seq = seq++;
    pAddPkt->RtryCnt = 0;
    pAddPkt->status = APP_PKT_UNSEND;
    memcpy(pAddPkt->Data, pData, u8len);
    AppLyr_AddToQueue(pAddPkt);
}

static void AppLyr_SndToDlkLyr(void)
{
    data_buf_t *pSendPkt;
    Dlk_ReqNInd SendStruct;
    for (pSendPkt = list_head(AppLyr_QueueList); pSendPkt != NULL; pSendPkt = pSendPkt->next)
    {
        if (
            (pSendPkt->status != APP_PKT_UNSEND)
            && (pSendPkt->status != APP_PKT_SEND_ERROR)
        )
        {
          SendStruct = SendStruct;
            continue;
        }
        if (AppLyr_DlkLyrPkting(pSendPkt, &SendStruct) != TRUE)
        {
            AppLyr_RomoveFromQueue(pSendPkt);
            continue;
        }
        if (DlkDtaSap(&SendStruct) == TRUE)
        {
//            LED_TX = 0;
            AppLyr_RomoveFromQueue(pSendPkt);
            //pSendPkt->RtryCnt++;
            //pSendPkt->status = APP_PKT_SENDING;
        }
        break;
    }
}
/*
static void AppLyr_IsTaskDone(void)
{
    data_buf_t *pPollSendPkt;
    for (pPollSendPkt = list_head(AppLyr_QueueList); pPollSendPkt != NULL; pPollSendPkt = pPollSendPkt->next)
    {
        if (pPollSendPkt->status == APP_PKT_SENDING)
        {
            DlkDta_Cfm AskDlyStatus;
            AskDlyStatus.type = DATA_PRIMITIVE_CONFIRM;
            switch (DlkDtaSap(&AskDlyStatus))
            {
                case DTA_PRM_CFM_TYP_PROCEEDING:
                    ;
                    break;

                case DTA_PRM_CFM_TYP_COMPLETED:
                    AppLyr_RomoveFromQueue(pPollSendPkt);
                    break;

                case DTA_PRM_CFM_TYP_FAILURE:
                    if (pPollSendPkt->RtryCnt <= RetryCount)
                    {
                        pPollSendPkt->RtryCnt++;
                        pPollSendPkt->status = APP_PKT_SEND_ERROR;
                    }
                    else
                    {
                        AppLyr_RomoveFromQueue(pPollSendPkt);
                    }
                    break;

                default:
                    AppLyr_QueueInit();
                    break;
            }
        }
    }
}
*/
