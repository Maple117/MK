#ifndef _WAKE_UP_H_
#define _WAKE_UP_H_

#define TX_STATE_BIT        0x10
#define RX_STATE_BIT        0x20
enum
{
    TX_STATE_WAIT_FOR_TX,
    TX_STATE_WAKEUP
};
enum
{
    RX_STATE_WAIT_FOR_PACKET,
    RX_STATE_WAKEUP
};


#define FIFO_AEM_ACTIVE 1
#define FIFO_AEM_INACTIVE 0

#define WAKE_UP_TIMER_5S   5000L

#define WAKEUP_FRAME_COUNT    7
#define MAX_WAKEUP_LEN     7

extern U8 nTXFIFOAEMFlag;
extern U8 RTX_State;

void Ax5043_Receiver_On_Wor(void);
void WAKEUP_TIMER_INT(void);
U8 WakeUpNode(void);



#define axradio_phy_preamble_wor_len  40
#define axradio_phy_preamble_wor_longlen 60
#define axradio_phy_preamble_len  32
#define axradio_framing_synclen   32

#define axradio_phy_preamble_flags 0x38
#define  axradio_phy_preamble_byte 0x55


#endif
