#include "AX5043.h"
#include "Typedef.h"
#include "Radio.h"

#define lposckfiltmax 0xaec
//#define FXTAL 26000000

static void wait_n_lposccycles(uint8_t n)
{
    uint8_t cnt = 0;
    disableInterrupts();
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1,SpiReadSingleAddressRegister(REG_AX5043_IRQMASK1)|0x04); // LPOSC irq
    for (;;)
    {
        if (SpiReadSingleAddressRegister(REG_AX5043_IRQREQUEST1) & 0x04 )
        {
            cnt++;
            SpiReadLongAddressRegister(REG_AX5043_LPOSCSTATUS); // clear irq request
        }
        if (cnt > n)
            break;
    }
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, SpiReadSingleAddressRegister(REG_AX5043_IRQMASK1)&~0x04);
    enableInterrupts();
}

void calibrate_lposc(void)
{
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCFREQ1,0x00);
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCFREQ0,0x00);
#if !(XTAL_FREQ >= 33000000 && XTAL_FREQ <= 47000000)
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCREF1,((XTAL_FREQ/640)>>8) & 0xFF);
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCREF0,((XTAL_FREQ/640)>>0) & 0xFF);
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE , AX5043_PWRSTATE_SYNTH_RX);
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCKFILT1 ,(lposckfiltmax >> (8 + 1)) & 0xFF); // kfiltmax >> 1
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCKFILT0 , (lposckfiltmax >> 1) & 0xFF);
    RdioXtalON();
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCCONFIG ,  0x25); // LPOSC ENA, slow mode; calibrate on rising edge, irq on rising edge
    wait_n_lposccycles(6);
#if XTAL_FREQ > 30000000

    if (SpiReadLongAddressRegister(REG_AX5043_LPOSCFREQ1) == 0x7F)
    {
        // freq tuning went to max pos. start 25% lower
        SpiWriteLongAddressRegister(REG_AX5043_LPOSCFREQ1, 0xBC);
        wait_n_lposccycles(6);
    }

    if ( SpiReadLongAddressRegister(REG_AX5043_LPOSCFREQ1) == 0x80)
    {
        // freq tuning went to max neg. start 25% higher
        SpiWriteLongAddressRegister(REG_AX5043_LPOSCFREQ1, 0xA8);
        wait_n_lposccycles(6);
    }
#endif

    SpiWriteLongAddressRegister(REG_AX5043_LPOSCKFILT1, (lposckfiltmax >> (8 + 2)) & 0xFF); // kfiltmax >> 2
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCKFILT0, (lposckfiltmax >> 2) & 0xFF);
    wait_n_lposccycles(5);

    SpiWriteLongAddressRegister(REG_AX5043_LPOSCCONFIG, 0x00);
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN);

    {
        uint8_t x = SpiReadLongAddressRegister(REG_AX5043_LPOSCFREQ1);
        if ( x == 0x7f || x == 0x80 )
        {
            SpiWriteLongAddressRegister(REG_AX5043_LPOSCFREQ1, 0);
            SpiWriteLongAddressRegister(REG_AX5043_LPOSCFREQ0, 0);
        }
    }
#endif
}


