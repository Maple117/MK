#include "AX5043.h"
#include "string.h"
#include "Radio.h"
#include "WakeUp.h"

U8 RTX_State;
U8 nTXFIFOAEMFlag;

static U8 WakeUpStt;
static U16 WakeUpCount;
static U16 WakeUpCountEnd;
#define CLR_WAKE_STT_TIMEOUT()       { WakeUpCount = 0;WakeUpStt = TIMER_IDLE;}
#define SET_WAKE_STT_TIMEOUT(itvl)   { CLR_WAKE_STT_TIMEOUT(); WakeUpCountEnd = itvl;WakeUpStt = WAIT_TIMER_OUT;}

#define axradio_wor_period 3200

void Ax5043_Receiver_On_Wor(void)
{
    Ax5043SetRegisters_RX();
    //SpiWriteSingleAddressRegister(REG_AX5043_IRQINVERSION1,0x01);
    SpiWriteLongAddressRegister(REG_AX5043_BGNDRSSIGAIN, 0x02);
    SpiWriteSingleAddressRegister(REG_AX5043_RADIOEVENTMASK0, 0x04);
    SpiWriteSingleAddressRegister(REG_AX5043_FIFOSTAT, 0x03);// clear FIFO data & flags
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCCONFIG, 0x01);// start LPOSC, slow mode
    SpiWriteLongAddressRegister(REG_AX5043_TMGRXPREAMBLE1, 0x32);
    SpiWriteLongAddressRegister(REG_AX5043_RSSIREFERENCE, 0x19);
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_WOR_RX);
    //SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x01);//  enable FIFO not empty
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x41);//  enable FIFO not empty / radio controller irq
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x01);// xtal ready
    //ubRFState = trxstate_wait_xtal;
    {
        uint16_t wp = axradio_wor_period;
        U16 TempData = 0;
        SpiWriteSingleAddressRegister(REG_AX5043_WAKEUPFREQ1, (wp >> 8) & 0xFF);// actually wakeup period measured in LP OSC cycles
        SpiWriteSingleAddressRegister(REG_AX5043_WAKEUPFREQ0, (wp >> 0) & 0xFF);
        TempData = SpiReadSingleAddressRegister(REG_AX5043_WAKEUPTIMER1);
        TempData = TempData << 8;
        TempData += SpiReadSingleAddressRegister(REG_AX5043_WAKEUPTIMER0);
        
        wp += TempData;
        SpiWriteSingleAddressRegister(REG_AX5043_WAKEUP1, (wp >> 8) & 0xFF);
        SpiWriteSingleAddressRegister(REG_AX5043_WAKEUP0, (wp >> 0) & 0xFF);
    }
	SpiWriteSingleAddressRegister(REG_AX5043_IRQINVERSION1, SpiReadSingleAddressRegister(REG_AX5043_IRQINVERSION1)&~0x01);
	SpiWriteLongAddressRegister(REG_AX5043_0xF30, 0x3f);
	SpiWriteLongAddressRegister(REG_AX5043_0xF31, 0xf0);
	SpiWriteLongAddressRegister(REG_AX5043_0xF32, 0x3f);
	SpiWriteLongAddressRegister(REG_AX5043_0xF33, 0xf0);
}

void WAKEUP_TIMER_INT(void)
{
    if (WakeUpStt == WAIT_TIMER_OUT)
    {
        WakeUpCount++;
        if (WakeUpCountEnd == WakeUpCount)
        {
            CLR_WAKE_STT_TIMEOUT();
            nTXFIFOAEMFlag = FIFO_AEM_INACTIVE;
            RTX_State = RX_STATE_BIT|RX_STATE_WAIT_FOR_PACKET;
        }
    }
}

U8 WakeUpNode(void)
{
    U8 tempBuff[WAKEUP_FRAME_COUNT];
    RTX_State = TX_STATE_BIT|TX_STATE_WAKEUP;
    nTXFIFOAEMFlag = FIFO_AEM_ACTIVE;
    SET_WAKE_STT_TIMEOUT(4000L);
    memset(tempBuff, 0xFF, WAKEUP_FRAME_COUNT);
    do
    {
        while ((nTXFIFOAEMFlag == FIFO_AEM_INACTIVE) && (RTX_State == (TX_STATE_BIT|TX_STATE_WAKEUP)))
        {
#ifdef WDG_ENABLE
            Refresh_WWDG_Window();
#endif
        };
        nTXFIFOAEMFlag = FIFO_AEM_INACTIVE;
        LED_TX = 0;
        if (RTX_State != (TX_STATE_BIT|TX_STATE_WAKEUP))
            break;
        transmit_packet_task(tempBuff, WAKEUP_FRAME_COUNT);
#ifdef WDG_ENABLE
        Refresh_WWDG_Window();
#endif
    }
    while (RTX_State == (TX_STATE_BIT|TX_STATE_WAKEUP)); //while(rtEvent == ENTER_WAKE_UP);
    LED_TX = 1;
    SetReceiveMode();
    AX5043ReceiverON();
    return TRUE;
}

