#include "Typedef.h"
#include "list.h"
#include "clock.h"
#include "Uart.h"
#include "Radio.h"
#include "AX5043.h"
#include "DlkLyr.h"
#include "command.h"

#define MaxBuf UART_DATA_MAX_LEN
U8 SendDoneFlag;
/*
static void PhySendData_Timeout(void *ptr);
static struct ctimer PhySendTimer;
#define CLR_SEND_STT_TIMEOUT()       {                     \
        ctimer_stop(&PhySendTimer);                            \
}

#define SET_SEND_STT_TIMEOUT(itvl)   {                              \
                  CLR_SEND_STT_TIMEOUT();                     \
                  ctimer_set(&PhySendTimer, (itvl), PhySendData_Timeout, NULL);    \
}
*/

static U8 SendStt;
static U16 SendCount;
static U16 SendCountEnd;
#define CLR_SEND_STT_TIMEOUT()       { SendCount = 0;SendStt = TIMER_IDLE;}
#define SET_SEND_STT_TIMEOUT(itvl)   { CLR_SEND_STT_TIMEOUT(); SendCountEnd = itvl;SendStt = WAIT_TIMER_OUT;}


U8 u8Dlk_TxStatus;
void Dlk_SetStatus(U8 DlkState);
U8 DLK_GetStatus(void);
static U8 DlkToPhy(Dlk_ReqNInd *pData);

#define SEND_TIMER  2000L

void aaa(void)
{
  Dlk_SetStatus(DLK_PKT_SENING);
  //SET_SEND_STT_TIMEOUT(SEND_TIMER);
  transmit_packet_task("qwertyuiop",10);
  while(DLK_GetStatus() != DLK_PKT_IDLE);
  Dlk_SetStatus(DLK_PKT_SENING);
  //SET_SEND_STT_TIMEOUT(SEND_TIMER);
  transmit_packet_task("9876543210",10);
  while(DLK_GetStatus() != DLK_PKT_IDLE);
}

U8 DlkDtaSap(void * sap)
{
    switch (*((U8*)sap))
    {
        case DATA_PRIMITIVE_REQUEST:
            return DlkToPhy(sap);
        default:
            return FALSE;
    }
}

static U8 DlkToPhy(Dlk_ReqNInd *pData)
{
    if (DLK_GetStatus() != DLK_PKT_IDLE)
    {
        return FALSE;
    }
    if (pData->len > MaxBuf)
    {
        return FALSE;
    }
    //LED_TX = 0;
    Dlk_SetStatus(DLK_PKT_SENING);
    SET_SEND_STT_TIMEOUT(SEND_TIMER);
    transmit_packet_task(pData->buf, pData->len);
    //SX1276LoRaSetTxPacket(pData->buf,pData->len);
    return TRUE;
}

void Dlk_SetStatus(U8 DlkState)
{
    u8Dlk_TxStatus = DlkState;
}

U8 DLK_GetStatus(void)
{
    return u8Dlk_TxStatus;
}

void intEventLowerTxCallBack(Phy_CallBackTxEvent event)
{
    switch (DLK_GetStatus())
    {
        case DLK_PKT_SENING:
            switch (event)
            {
                case CBTE_LBT_TIMEOUT:
                case CBTE_NO_ACK:
                    Dlk_SetStatus(DLK_STAT_RF_ERROR);
                    break;
                case CBTE_PACKET_SENT:
                    CLR_SEND_STT_TIMEOUT();
                    //AX5043ReceiverON();
                    Dlk_SetStatus(DLK_PKT_IDLE);
                    SendDoneFlag = 1;
                    //SetReceiveMode();
                    //AX5043ReceiverON();
                    break;
                case CBTE_ERROR:
                default:
                    break;
            }
            break;
            /*
        default:
            Dlk_SetStatus(DLK_STAT_RF_ERROR);
            break;
            */
    }
}

void DLK_StatusInit(void)
{
    u8Dlk_TxStatus = DLK_PKT_IDLE;
    SendDoneFlag = 0;
}

void Send_TIMER_INT(void)
{
    if (SendStt == WAIT_TIMER_OUT)
    {
        SendCount++;
        if (SendCountEnd == SendCount)
        {
            CLR_SEND_STT_TIMEOUT();
            Dlk_SetStatus(DLK_STAT_RF_ERROR);
        }
    }
}

void DlkTask(void)
{
    if (DLK_GetStatus() == DLK_STAT_RF_ERROR)
    {
        CLR_SEND_STT_TIMEOUT();
        if (InitAX5043() != AXRADIO_ERR_NOERROR)
        {
            while (1);
        }
        SetReceiveMode();
        AX5043ReceiverON();
        Dlk_SetStatus(DLK_PKT_IDLE);
    }
    if (SendDoneFlag == 1)
    {
        if (SpiReadSingleAddressRegister(REG_AX5043_RADIOSTATE) == 0)
        {
            SendDoneFlag = 0;
            
            SetReceiveMode();
            AX5043ReceiverON();
            
            CommandSendDone();
        }
    }
}

