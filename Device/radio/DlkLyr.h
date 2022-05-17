#ifndef _DLK_LYR_H_
#define _DLK_LYR_H_

typedef enum
{
    CBTE_LBT_TIMEOUT,
    CBTE_PACKET_SENT,
    CBTE_NO_ACK,
    CBTE_ERROR
} Phy_CallBackTxEvent;


U8 DlkDtaSap(void * sap);
void intEventLowerTxCallBack(Phy_CallBackTxEvent event);
void DLK_StatusInit(void);

typedef enum
{
    DATA_PRIMITIVE_REQUEST,
    DATA_PRIMITIVE_CONFIRM,
    DATA_PRIMITIVE_INDICATION,
    DATA_PRIMITIVE_RESPONSE
}DlkPmtvType;

typedef enum
{
    DTA_PRM_CFM_TYP_PROCEEDING,
    DTA_PRM_CFM_TYP_COMPLETED,
    DTA_PRM_CFM_TYP_FAILURE,
    DTA_PRM_CFM_TYP_UNKNOWN
} eDlkStateType;

enum DLK_STATUS
{
    DLK_PKT_IDLE,
    DLK_PKT_SENING,
    DLK_PKT_SENT,
    DLK_SEND_ERROR,
    DLK_STAT_RF_ERROR
};

typedef struct
{
    DlkPmtvType DlkType;
    U8 DlkSeq;
    U8 u8AckCtl;
    U8 len;
    U8 *buf;

} Dlk_ReqNInd;

typedef struct
{
    DlkPmtvType type;

} DlkDta_Cfm;

void DlkTask(void);
void Send_TIMER_INT(void);

#endif