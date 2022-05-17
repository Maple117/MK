/*********************************************************************                                                                   **
**  Device:     AX5043
**  File:  Radio.c
**  Author:  Strong
**  Target:  R8C/1B
**  Tools:  ICE
**  Version:    V00
**  Date:     2014-01-20
**  Update :    2014/01/20
**  Description:
**        This file is a sample code for your reference.
**  Copyright (C) 2014 Karv Technology Co.,Ltd
*********************************************************************/
#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include  <iostm8l151k4.h>
#include <string.h>
#include "Uart.h"
#include "Typedef.h"
#include "DlkLyr.h"
#include "AX5043.h"
#include "Configure.h"
#include "WakeUp.h"
#include "Radio_Config.h"
#include "Radio.h"
#include "spi.h"
#include "sysdata.h"
#include "command.h"
#include "GPIO.h"

// TX: fcarrier=433.000MHz dev=  3.333kHz br= 10.000kBit/s pwr= 15.0dBm
// RX: fcarrier=433.000MHz bw= 15.000kHz br= 10.000kBit/s
const unsigned int RegisterVaule[][2]=
{
#if(XTAL_FREQ == 48000000)
    {
        REG_AX5043_MODULATION ,   0x08
    },
    { REG_AX5043_ENCODING  ,   0x00   },
    { REG_AX5043_FRAMING  ,   0x26   },
    { REG_AX5043_PINFUNCSYSCLK,  0x01},
    { REG_AX5043_PINFUNCDCLK , 0x01  },
    { REG_AX5043_PINFUNCDATA , 0x01  },
    { REG_AX5043_PINFUNCANTSEL , 0x00},
    { REG_AX5043_PINFUNCPWRAMP , 0x07},
    { REG_AX5043_WAKEUPXOEARLY , 0x01},
    { REG_AX5043_IFFREQ1  , 0x01   },
    { REG_AX5043_IFFREQ0  , 0x11   },
    { REG_AX5043_DECIMATION , 0x3c },
    { REG_AX5043_RXDATARATE2 , 0x00  },
    { REG_AX5043_RXDATARATE1 , 0xa6  },
    { REG_AX5043_RXDATARATE0 , 0xaa  },
    { REG_AX5043_MAXDROFFSET2 , 0x00 },
    { REG_AX5043_MAXDROFFSET1 , 0x00 },
    { REG_AX5043_MAXDROFFSET0 , 0x00 },
    { REG_AX5043_MAXRFOFFSET2 , 0x80 },
    { REG_AX5043_MAXRFOFFSET1 , 0x02 },
    { REG_AX5043_MAXRFOFFSET0 , 0x5d },
    { REG_AX5043_FSKDMAX1  , 0x03   },
    { REG_AX5043_FSKDMAX0  , 0x95   },
    { REG_AX5043_FSKDMIN1  , 0xfc   },
    { REG_AX5043_FSKDMIN0  , 0x6b   },
    { REG_AX5043_AMPLFILTER , 0x00   },
    { REG_AX5043_RXPARAMSETS , 0xF4  },
    { REG_AX5043_AGCGAIN0  , 0x86   },
    { REG_AX5043_AGCTARGET0 , 0x76   },
    { REG_AX5043_TIMEGAIN0  , 0xf6   },
    { REG_AX5043_DRGAIN0  , 0xa4   },
    { REG_AX5043_PHASEGAIN0 ,  0xc3  },
    { REG_AX5043_FREQUENCYGAINA0, 0x0f},
    { REG_AX5043_FREQUENCYGAINB0, 0x1f},
    { REG_AX5043_FREQUENCYGAINC0, 0x0b},
    { REG_AX5043_FREQUENCYGAIND0, 0x0b},
    { REG_AX5043_AMPLITUDEGAIN0 , 0x46 },
    { REG_AX5043_FREQDEV10   , 0x00   },
    { REG_AX5043_FREQDEV00   , 0x00   },
    { REG_AX5043_FOURFSK0   , 0x00   },
    { REG_AX5043_BBOFFSRES0  , 0x00  },
    { REG_AX5043_AGCGAIN1   , 0x86   },
    { REG_AX5043_AGCTARGET1  , 0x76  },
    { REG_AX5043_AGCAHYST1   , 0x00   },
    { REG_AX5043_AGCMINMAX1  , 0x00  },
    { REG_AX5043_TIMEGAIN1   , 0xa8   },
    { REG_AX5043_DRGAIN1   , 0xa3   },
    { REG_AX5043_PHASEGAIN1  , 0xc3  },
    { REG_AX5043_FREQUENCYGAINA1, 0x0f},
    { REG_AX5043_FREQUENCYGAINB1, 0x1f},
    { REG_AX5043_FREQUENCYGAINC1, 0x0b},
    { REG_AX5043_FREQUENCYGAIND1, 0x0b},
    { REG_AX5043_AMPLITUDEGAIN1 , 0x46 },
    { REG_AX5043_FREQDEV11   , 0x00   },
    { REG_AX5043_FREQDEV01   , 0x38   },
    { REG_AX5043_FOURFSK1   , 0x00   },
    { REG_AX5043_BBOFFSRES1  , 0x00  },
    { REG_AX5043_AGCGAIN3   , 0xff   },
    { REG_AX5043_AGCTARGET3  , 0x76  },
    { REG_AX5043_AGCAHYST3   , 0x00   },
    { REG_AX5043_AGCMINMAX3  , 0x00  },
    { REG_AX5043_TIMEGAIN3   , 0xa7   },
    { REG_AX5043_DRGAIN3   , 0xa2 },
    { REG_AX5043_PHASEGAIN3  , 0xc3  },
    { REG_AX5043_FREQUENCYGAINA3, 0x0f},
    { REG_AX5043_FREQUENCYGAINB3, 0x1f},
    { REG_AX5043_FREQUENCYGAINC3, 0x0d},
    { REG_AX5043_FREQUENCYGAIND3, 0x0d},
    { REG_AX5043_AMPLITUDEGAIN3 , 0x46 },
    { REG_AX5043_FREQDEV13   , 0x00  },
    { REG_AX5043_FREQDEV03   , 0x38  },
    { REG_AX5043_FOURFSK3   , 0x00  },
    { REG_AX5043_BBOFFSRES3  , 0x00 },
    { REG_AX5043_MODCFGF   , 0x03  },
    { REG_AX5043_FSKDEV2   , 0x00  },
    { REG_AX5043_FSKDEV1   , 0x00  },
    { REG_AX5043_FSKDEV0   , 0xaf  },
    { REG_AX5043_MODCFGA   , 0x05  },
    { REG_AX5043_TXRATE2   , 0x00  },
    { REG_AX5043_TXRATE1   , 0x01  },
    { REG_AX5043_TXRATE0   , 0xa3  },
    { REG_AX5043_TXPWRCOEFFB1  , 0x0f },
    { REG_AX5043_TXPWRCOEFFB0  , 0xff },
    { REG_AX5043_PLLRNGCLK   , 0x05  },
    { REG_AX5043_BBTUNE   , 0x0f  },
    { REG_AX5043_BBOFFSCAP   , 0x77  },
    { REG_AX5043_PKTADDRCFG  , 0x00 },
    { REG_AX5043_PKTLENCFG   , 0x80  },
    { REG_AX5043_PKTLENOFFSET  , 0x00 },
    { REG_AX5043_PKTMAXLEN   , 0xf0  },
    { REG_AX5043_MATCH0PAT3  , 0xaa },
    { REG_AX5043_MATCH0PAT2  , 0xcc },
    { REG_AX5043_MATCH0PAT1  , 0xaa },
    { REG_AX5043_MATCH0PAT0  , 0xcc },
    { REG_AX5043_MATCH0LEN   , 0x1f  },
    { REG_AX5043_MATCH0MAX   , 0x1f  },
    { REG_AX5043_MATCH1PAT1  , 0x55 },
    { REG_AX5043_MATCH1PAT0  , 0x55 },
    { REG_AX5043_MATCH1LEN   , 0x8a  },
    { REG_AX5043_MATCH1MAX   , 0x0a  },
    { REG_AX5043_TMGRXBOOST    , 0x5b   },
    { REG_AX5043_TMGRXSETTLE  , 0x3E },
    { REG_AX5043_TMGRXOFFSACQ  , 0x9c },
    { REG_AX5043_TMGRXCOARSEAGC , 0x72},
    { REG_AX5043_TMGRXAGC   , 0x00  },
    { REG_AX5043_TMGRXRSSI   , 0x08  },
    { REG_AX5043_TMGRXPREAMBLE2 , 0x35},
//    { REG_AX5043_RSSIREFERENCE  , 0x19 },
    { REG_AX5043_RSSIABSTHR  , 0xc6 },
    { REG_AX5043_BGNDRSSIGAIN  , 0x02 },
    { REG_AX5043_BGNDRSSITHR  , 0x00 },
    { REG_AX5043_PKTCHUNKSIZE  , 0x0d },
    { REG_AX5043_PKTMISCFLAGS  , 0x03 },
    { REG_AX5043_PKTACCEPTFLAGS , 0x20},
    { REG_AX5043_DACVALUE1   , 0x00  },
    { REG_AX5043_DACVALUE0   , 0x00  },
    { REG_AX5043_DACCONFIG   , 0x00  },
    { REG_AX5043_0xF10    , 0x04   },
    { REG_AX5043_0xF11    , 0x00   },
    { REG_AX5043_0xF1C    , 0x07   },
    { REG_AX5043_0xF21    , 0x5c   },
    { REG_AX5043_0xF22    , 0x53   },
    { REG_AX5043_0xF23    , 0x76   },
    { REG_AX5043_0xF26    , 0x92   },
    { REG_AX5043_0xF34    , 0x28   },
    { REG_AX5043_0xF35    , 0x11 },
#elif(XTAL_FREQ == 26000000)
    {REG_AX5043_MODULATION ,   0x08},
    { REG_AX5043_ENCODING  ,   0x00   },
    { REG_AX5043_FRAMING  ,   0x26   },
    { REG_AX5043_PINFUNCSYSCLK,  0x01},
    { REG_AX5043_PINFUNCDCLK , 0x01  },
    { REG_AX5043_PINFUNCDATA , 0x01  },
    { REG_AX5043_PINFUNCANTSEL , 0x04},
    { REG_AX5043_PINFUNCPWRAMP , 0x01},
    { REG_AX5043_WAKEUPXOEARLY , 0x01},
    { REG_AX5043_IFFREQ1  , 0x00   },
    { REG_AX5043_IFFREQ0  , 0xFC   },
    { REG_AX5043_DECIMATION , 0x5A  },
    { REG_AX5043_RXDATARATE2 , 0x00  },
    { REG_AX5043_RXDATARATE1 , 0x3c  },
    { REG_AX5043_RXDATARATE0 , 0x2f  },
    { REG_AX5043_MAXDROFFSET2 , 0x00 },
    { REG_AX5043_MAXDROFFSET1 , 0x00 },
    { REG_AX5043_MAXDROFFSET0 , 0x00 },
    { REG_AX5043_MAXRFOFFSET2 , 0x80 },
    { REG_AX5043_MAXRFOFFSET1 , 0x02 },
    { REG_AX5043_MAXRFOFFSET0 , 0x31 },
    { REG_AX5043_FSKDMAX1  , 0x00   },
    { REG_AX5043_FSKDMAX0  , 0xA6   },
    { REG_AX5043_FSKDMIN1  , 0xFF   },
    { REG_AX5043_FSKDMIN0  , 0x5a   },
    { REG_AX5043_AMPLFILTER , 0x00   },
    { REG_AX5043_RXPARAMSETS , 0xF4  },
    { REG_AX5043_AGCGAIN0  , 0x97   },
    { REG_AX5043_AGCTARGET0 , 0x76   },
    { REG_AX5043_TIMEGAIN0  , 0xf8   },
    { REG_AX5043_DRGAIN0  , 0xf2   },
    { REG_AX5043_PHASEGAIN0 ,  0xc3  },
    { REG_AX5043_FREQUENCYGAINA0, 0x0f},
    { REG_AX5043_FREQUENCYGAINB0, 0x1f},
    { REG_AX5043_FREQUENCYGAINC0, 0x0a},
    { REG_AX5043_FREQUENCYGAIND0, 0x0a},
    { REG_AX5043_AMPLITUDEGAIN0 , 0x46 },
    { REG_AX5043_FREQDEV10   , 0x00   },
    { REG_AX5043_FREQDEV00   , 0x00   },
    { REG_AX5043_FOURFSK0   , 0x00   },
    { REG_AX5043_BBOFFSRES0  , 0x00  },
    { REG_AX5043_AGCGAIN1   , 0x97   },
    { REG_AX5043_AGCTARGET1  , 0x76  },
    { REG_AX5043_AGCAHYST1   , 0x00   },
    { REG_AX5043_AGCMINMAX1  , 0x00  },
    { REG_AX5043_TIMEGAIN1   , 0xf6   },
    { REG_AX5043_DRGAIN1   , 0xf1   },
    { REG_AX5043_PHASEGAIN1  , 0xc3  },
    { REG_AX5043_FREQUENCYGAINA1, 0x0f},
    { REG_AX5043_FREQUENCYGAINB1, 0x1f},
    { REG_AX5043_FREQUENCYGAINC1, 0x0a},
    { REG_AX5043_FREQUENCYGAIND1, 0x0a},
    { REG_AX5043_AMPLITUDEGAIN1 , 0x46 },
    { REG_AX5043_FREQDEV11   , 0x00   },
    { REG_AX5043_FREQDEV01   , 0x38   },
    { REG_AX5043_FOURFSK1   , 0x00   },
    { REG_AX5043_BBOFFSRES1  , 0x00  },
    { REG_AX5043_AGCGAIN3   , 0xff   },
    { REG_AX5043_AGCTARGET3  , 0x76  },
    { REG_AX5043_AGCAHYST3   , 0x00   },
    { REG_AX5043_AGCMINMAX3  , 0x00  },
    { REG_AX5043_TIMEGAIN3   , 0xf5   },
    { REG_AX5043_DRGAIN3   , 0xf0   },
    { REG_AX5043_PHASEGAIN3  , 0xc3  },
    { REG_AX5043_FREQUENCYGAINA3, 0x0f},
    { REG_AX5043_FREQUENCYGAINB3, 0x1f},
    { REG_AX5043_FREQUENCYGAINC3, 0x0d},
    { REG_AX5043_FREQUENCYGAIND3, 0x0d},
    { REG_AX5043_AMPLITUDEGAIN3 , 0x46 },
    { REG_AX5043_FREQDEV13   , 0x00  },
    { REG_AX5043_FREQDEV03   , 0x38  },
    { REG_AX5043_FOURFSK3   , 0x00  },
    { REG_AX5043_BBOFFSRES3  , 0x00 },
    { REG_AX5043_MODCFGF   , 0x03  },
    { REG_AX5043_FSKDEV2   , 0x00  },
    { REG_AX5043_FSKDEV1   , 0x01  },
    { REG_AX5043_FSKDEV0   , 0x43  },
    { REG_AX5043_MODCFGA   , 0x05  },
    { REG_AX5043_TXRATE2   , 0x00  },
    { REG_AX5043_TXRATE1   , 0x03  },
    { REG_AX5043_TXRATE0   , 0x06  },
    { REG_AX5043_TXPWRCOEFFB1  , 0x0f },
    { REG_AX5043_TXPWRCOEFFB0  , 0xff },
    { REG_AX5043_PLLRNGCLK   , 0x04  },
    { REG_AX5043_BBTUNE   , 0x0f  },
    { REG_AX5043_BBOFFSCAP   , 0x77  },
    { REG_AX5043_PKTADDRCFG  , 0x00 },
    { REG_AX5043_PKTLENCFG   , 0x80  },
    { REG_AX5043_PKTLENOFFSET  , 0x00 },
    { REG_AX5043_PKTMAXLEN   , 0xc8  },
    { REG_AX5043_MATCH0PAT3  , 0xaa },
    { REG_AX5043_MATCH0PAT2  , 0xcc },
    { REG_AX5043_MATCH0PAT1  , 0xaa },
    { REG_AX5043_MATCH0PAT0  , 0xcc },
    { REG_AX5043_MATCH0LEN   , 0x1f  },
    { REG_AX5043_MATCH0MAX   , 0x1f  },
    { REG_AX5043_MATCH1PAT1  , 0x55 },
    { REG_AX5043_MATCH1PAT0  , 0x55 },
    { REG_AX5043_MATCH1LEN   , 0x8a  },
    { REG_AX5043_MATCH1MAX   , 0x0a  },
    { REG_AX5043_TMGRXBOOST    , 0x3e   },
    { REG_AX5043_TMGRXSETTLE  , 0x31 },
    { REG_AX5043_TMGRXOFFSACQ  , 0x7f },
    { REG_AX5043_TMGRXCOARSEAGC , 0x14},
    { REG_AX5043_TMGRXAGC   , 0x00  },
    { REG_AX5043_TMGRXRSSI   , 0x03 },
    { REG_AX5043_TMGRXPREAMBLE2 , 0x35},
//    { REG_AX5043_RSSIREFERENCE  , 0x19 },
    { REG_AX5043_RSSIABSTHR  , 0xc6 },
    { REG_AX5043_BGNDRSSIGAIN  , 0x02 },
    { REG_AX5043_BGNDRSSITHR  , 0x00 },
    { REG_AX5043_PKTCHUNKSIZE  , 0x0d },
    { REG_AX5043_PKTMISCFLAGS  , 0x03 },
    { REG_AX5043_PKTACCEPTFLAGS , 0x20},
    { REG_AX5043_DACVALUE1   , 0x00  },
    { REG_AX5043_DACVALUE0   , 0x00  },
    { REG_AX5043_DACCONFIG   , 0x00  },
    { REG_AX5043_0xF10    , 0x04   },
    { REG_AX5043_0xF11    , 0x00   },
    { REG_AX5043_0xF1C    , 0x07   },
    { REG_AX5043_0xF21    , 0x5c   },
    { REG_AX5043_0xF22    , 0x53   },
    { REG_AX5043_0xF23    , 0x76   },
    { REG_AX5043_0xF26    , 0x92   },
    { REG_AX5043_0xF34    , 0x28   },
    { REG_AX5043_0xF35    , 0x11 },
#elif(XTAL_FREQ == 16000000)
    {
        REG_AX5043_MODULATION ,   0x08
    },
    { REG_AX5043_ENCODING  ,   0x00   },
    { REG_AX5043_FRAMING  ,   0x26 },
    { REG_AX5043_PINFUNCSYSCLK,  0x01},
    { REG_AX5043_PINFUNCDCLK , 0x01  },
    { REG_AX5043_PINFUNCDATA , 0x01  },
    { REG_AX5043_PINFUNCANTSEL , 0x04},
    { REG_AX5043_PINFUNCPWRAMP , 0x01},
    { REG_AX5043_WAKEUPXOEARLY , 0x01},
    { REG_AX5043_MAXDROFFSET2 , 0x00 },
    { REG_AX5043_MAXDROFFSET1 , 0x00 },
    { REG_AX5043_MAXDROFFSET0 , 0x00 },
    { REG_AX5043_MAXRFOFFSET2 , 0x80 },
    { REG_AX5043_AMPLFILTER , 0x00 },
    { REG_AX5043_RXPARAMSETS , 0xF4  },
    { REG_AX5043_FREQUENCYGAINA0, 0x0f},
    { REG_AX5043_FREQUENCYGAINB0, 0x1f},
    { REG_AX5043_AMPLITUDEGAIN0 , 0x46 },
    { REG_AX5043_FREQDEV10 , 0x00  },
    { REG_AX5043_FREQDEV00 , 0x00  },
    { REG_AX5043_FOURFSK0 , 0x00  },
    { REG_AX5043_BBOFFSRES0  , 0x00  },
    { REG_AX5043_AGCTARGET1  , 0x76  },
    { REG_AX5043_AGCAHYST1 , 0x00  },
    { REG_AX5043_AGCMINMAX1  , 0x00  },
    { REG_AX5043_FREQUENCYGAINA1, 0x0f},
    { REG_AX5043_FREQUENCYGAINB1, 0x1f},
    { REG_AX5043_AMPLITUDEGAIN1 , 0x46 },
    { REG_AX5043_FREQDEV11 , 0x00  },
    { REG_AX5043_FREQDEV01 , 0x38  },
    { REG_AX5043_FOURFSK1 , 0x00  },
    { REG_AX5043_BBOFFSRES1  , 0x00  },
    { REG_AX5043_AGCGAIN3 , 0xff  },
    { REG_AX5043_AGCTARGET3  , 0x76  },
    { REG_AX5043_AGCAHYST3 , 0x00  },
    { REG_AX5043_AGCMINMAX3  , 0x00  },
    { REG_AX5043_FREQUENCYGAINA3, 0x0f},
    { REG_AX5043_FREQUENCYGAINB3, 0x1f},
    { REG_AX5043_AMPLITUDEGAIN3 , 0x46 },
    { REG_AX5043_FREQDEV13 , 0x00 },
    { REG_AX5043_FREQDEV03 , 0x38 },
    { REG_AX5043_FOURFSK3 , 0x00 },
    { REG_AX5043_BBOFFSRES3  , 0x00 },
    { REG_AX5043_MODCFGF   , 0x03  },
    { REG_AX5043_TXPWRCOEFFB1  , 0x02 },
    { REG_AX5043_TXPWRCOEFFB0  , 0x07 },
    { REG_AX5043_PLLRNGCLK , 0x03 },
    { REG_AX5043_BBOFFSCAP , 0x77 },
    { REG_AX5043_PKTADDRCFG  , 0x00 },
    { REG_AX5043_PKTLENCFG , 0x80 },
    { REG_AX5043_PKTLENOFFSET  , 0x00 },
    { REG_AX5043_PKTMAXLEN , 0xf0 },
    { REG_AX5043_MATCH0PAT3  , 0xaa },
    { REG_AX5043_MATCH0PAT2  , 0xcc },
    { REG_AX5043_MATCH0PAT1  , 0xaa },
    { REG_AX5043_MATCH0PAT0  , 0xcc },
    { REG_AX5043_MATCH0LEN , 0x1f },
    { REG_AX5043_MATCH0MAX , 0x1f },
    { REG_AX5043_MATCH1PAT1  , 0x55 },
    { REG_AX5043_MATCH1PAT0  , 0x55 },
    { REG_AX5043_MATCH1LEN , 0x8a },
    { REG_AX5043_MATCH1MAX , 0x0a },
    { REG_AX5043_TMGRXBOOST    , 0x32 },
    { REG_AX5043_TMGRXSETTLE  , 0x14 },
    { REG_AX5043_TMGRXOFFSACQ  , 0x73 },
    { REG_AX5043_TMGRXCOARSEAGC , 0x0c},
    { REG_AX5043_TMGRXAGC , 0x00 },
    { REG_AX5043_TMGRXRSSI , 0x08 },
    { REG_AX5043_TMGRXPREAMBLE2 , 0x35},
    { REG_AX5043_RSSIABSTHR  , 0xc6 },
    { REG_AX5043_BGNDRSSIGAIN  , 0x02 },
    { REG_AX5043_BGNDRSSITHR  , 0x00 },
    { REG_AX5043_PKTCHUNKSIZE  , 0x0d },
    { REG_AX5043_PKTMISCFLAGS  , 0x03 },
    { REG_AX5043_PKTACCEPTFLAGS , 0x20},
    { REG_AX5043_DACVALUE1 , 0x00 },
    { REG_AX5043_DACVALUE0 , 0x00 },
    { REG_AX5043_DACCONFIG , 0x00 },
    { REG_AX5043_0xF10  , 0x04   },
    { REG_AX5043_0xF11  , 0x00   },
    { REG_AX5043_0xF1C  , 0x07   },
    { REG_AX5043_0xF21  , 0x5c   },
    { REG_AX5043_0xF22  , 0x53   },
    { REG_AX5043_0xF23  , 0x76   },
    { REG_AX5043_0xF26  , 0x92   },
    { REG_AX5043_0xF34  , 0x28   },
    { REG_AX5043_0xF35  , 0x10 },
#endif
    { REG_AX5043_0xF44    , 0x24   },
    { AX5043_Addr_END    , AX5043_Vule_END  }
};

const unsigned int TXMODE_REG[][2]=
{
#if(XTAL_FREQ == 48000000)
    {
        REG_AX5043_PLLLOOP        , 0x09
    },
    { REG_AX5043_PLLCPI         , 0x02       },
    { REG_AX5043_PLLVCODIV      , 0x24    },
    { REG_AX5043_PLLVCOI        , 0x99      },
    { REG_AX5043_XTALCAP        , 0x00      },
    { REG_AX5043_0xF00          , 0x0f        },
    { REG_AX5043_REF            , 0x03          },
    { REG_AX5043_0xF18          , 0x06        }
#endif
#if(XTAL_FREQ == 26000000)
    {
        REG_AX5043_PLLLOOP  , 0x0b
    },
    { REG_AX5043_PLLCPI   , 0x10    },
    { REG_AX5043_PLLVCODIV  , 0x24   },
    { REG_AX5043_PLLVCOI  , 0x99   },
    { REG_AX5043_XTALCAP  , 0x00   },
    { REG_AX5043_0xF00   , 0x0f    },
    { REG_AX5043_REF   , 0x03    },
    { REG_AX5043_0xF18   , 0x06    }
#endif
#if (XTAL_FREQ == 16000000)
    {
        REG_AX5043_PLLLOOP , 0x09
    },
    { REG_AX5043_PLLCPI   , 0x02 },
    { REG_AX5043_PLLVCODIV , 0x24  },
    { REG_AX5043_PLLVCOI  , 0x99   },
    { REG_AX5043_XTALCAP  , 0x01   },
    { REG_AX5043_0xF00  , 0x0f    },
    { REG_AX5043_REF   , 0x03  },
    { REG_AX5043_0xF18  , 0x06    }
#endif
};

const unsigned int RXMODE_REG[][2]=
{
#if(XTAL_FREQ == 48000000)
    {
        REG_AX5043_PLLLOOP        , 0x0b
    },
    { REG_AX5043_PLLCPI         , 0x10       },
    { REG_AX5043_PLLVCODIV      , 0x25    },
    { REG_AX5043_PLLVCOI        , 0x99      },
    { REG_AX5043_XTALCAP        , 0x00      },
    { REG_AX5043_0xF00          , 0x0f        },
    { REG_AX5043_REF            , 0x03          },
    { REG_AX5043_0xF18          , 0x02        }

#endif
#if(XTAL_FREQ == 26000000)
    {
        REG_AX5043_PLLLOOP  , 0x0b
    },
    { REG_AX5043_PLLCPI   , 0x10  },
    { REG_AX5043_PLLVCODIV  , 0x24 },
    { REG_AX5043_PLLVCOI  , 0x99  },
    { REG_AX5043_XTALCAP  , 0x0c  },
    { REG_AX5043_0xF00   , 0x0f  },
    { REG_AX5043_REF   , 0x03   },
    { REG_AX5043_0xF18   , 0x02  }
#endif
#if(XTAL_FREQ == 16000000)
    {
        REG_AX5043_PLLLOOP  , 0x09
    },
    { REG_AX5043_PLLCPI   , 0x01  },
    { REG_AX5043_PLLVCODIV  , 0x24 },
    { REG_AX5043_PLLVCOI  , 0x99  },
    { REG_AX5043_XTALCAP  , 0x01  },
    { REG_AX5043_0xF00   , 0x0f  },
    { REG_AX5043_REF   , 0x03   },
    { REG_AX5043_0xF18   , 0x02  }
#endif
};


unsigned char AxRadioPHYChanPllRng_TX[CHANNEL_NUM];   //频率补偿
unsigned char AxRadioPHYChanPllRng_RX[CHANNEL_NUM];


unsigned char ChannelPtr ;
unsigned char ubRFState ;

unsigned char TXBuff[UART_DATA_MAX_LEN+1];
unsigned char TxLen;
unsigned char RXBuff[UART_DATA_MAX_LEN+1];
unsigned char RxLen;

unsigned char ubReceiveFlag;


void SpiPowerDown(void)
{
  SPI_OFF();
}

/***********************************************************************
* Funcation ： void SpiWriteByte(unsigned char ubByte)
* Description： write one byte
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SpiWriteByte(unsigned char ubByte)
{
    unsigned char ubi;

    for (ubi = 0;ubi < 8;ubi++)
    {
        SCK = 0;
        if ((ubByte&0x80) == 0x80)
            SDO = 1;
        else
            SDO = 0;
        SCK = 1;
        ubByte = ubByte << 1;
    }
    SCK = 0;
    asm("NOP");
    SDO = 0;
}

/***********************************************************************
* Funcation ： void SpiWriteWord(unsigned int ubByte)
* Description： write one word
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SpiWriteWord(unsigned int ubByte)
{
    unsigned char ubi;

    for (ubi = 0;ubi < 16;ubi++)
    {
        SCK = 0;
        if ((ubByte & 0x8000) == 0x8000)
            SDO = 1;
        else
            SDO = 0;
        SCK = 1;
        ubByte = ubByte << 1;
    }
    SCK = 0;
    asm("NOP");
    SDO = 0;
}

/***********************************************************************
* Funcation ：unsigned char SpiReadByte( void )
* Description： read one byte
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
unsigned char SpiReadByte( void )
{
    unsigned char ubi;
    unsigned char ubByte;

    //ubByte = 0x00;
    for (ubi = 0;ubi < 8;ubi++)
    {
        ubByte <<= 1;
        SCK = 1;
        if (SDI)
            ubByte |= 0x01;

        SCK = 0;
    }
    SDO = 0;
    return  ubByte;
}
unsigned char SpiReadAutoAddressRegister(unsigned int Addr)
{
  if(Addr < 0x70) return SpiReadSingleAddressRegister(Addr);
  else return SpiReadLongAddressRegister(Addr);
}

void SpiWriteAutoAddressRegister(unsigned int Addr,unsigned char Data)
{
  if(Addr < 0x70) SpiWriteSingleAddressRegister(Addr,Data);
  else SpiWriteLongAddressRegister(Addr,Data);
}
/***********************************************************************
* Funcation ： void SpiWriteSingleAddressRegister(unsigned char Addr, unsigned char Data)
* Description： write 8bit address register
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SpiWriteSingleAddressRegister(unsigned char Addr, unsigned char Data)
{
    unsigned char ubAddr ;

    ubAddr = Addr|0x80 ;  //write common  bit7=1
    //disableInterrupts();
    CSN = 0;
    SpiWriteByte(ubAddr);
    SpiWriteByte(Data);
    CSN = 1;
    // enableInterrupts();

}

/***********************************************************************
* Funcation ：void SpiWriteLongAddressRegister(unsigned int Addr, unsigned char Data)
* Description： write 11bit address register
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SpiWriteLongAddressRegister(unsigned int Addr, unsigned char Data)
{
    unsigned int ubAddr;
    ubAddr = Addr|0xF000; //long address bit[14:12]=0b111 ,write common bit15=1;
    //disableInterrupts();
    CSN = 0;
    SpiWriteWord(ubAddr);
    SpiWriteByte(Data);
    CSN = 1;
    //enableInterrupts();
}

/***********************************************************************
* Funcation ：void SpiWriteData(unsigned char *pBuf,unsigned char Length)
* Description：write data to AX5043_FIFODATA
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SpiWriteData(unsigned char *pBuf,unsigned char Length)
{
    unsigned char  ubi;
    disableInterrupts();
    CSN = 0;
    SpiWriteByte(REG_AX5043_FIFODATA| 0x80);  //write common  bit7=1
    for (ubi= 0x00;ubi < Length;ubi++)
    {
        SpiWriteByte(pBuf[ubi]);
    }
    CSN = 1;
    enableInterrupts();
}
/***********************************************************************
* Funcation ： unsigned char SpiReadSingleAddressRegister(unsigned char Addr)
* Description： form register read one byte data
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
unsigned char SpiReadSingleAddressRegister(unsigned char Addr)
{
    unsigned char ubAddr ;
    ubAddr = Addr&0x7F ;//read common bit7=0
    //disableInterrupts();
    CSN = 0;
    SpiWriteByte(ubAddr);
    ubAddr = SpiReadByte();
    CSN = 1;
    //enableInterrupts();
    return ubAddr ;
}

/***********************************************************************
* Funcation ： unsigned char SpiReadSingleAddressRegister(unsigned char Addr)
* Description： from long address register read one byte data
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
unsigned char SpiReadLongAddressRegister(unsigned int Addr)
{
    unsigned char ubData;
    unsigned int  ubAddr;
    //ubAddr = Addr&0x7FFF; // read   bit15=0
    ubAddr = Addr|0x7000; // read long addr bit[14:12]=0b111
    // disableInterrupts();
    CSN = 0;
    SpiWriteWord(ubAddr);
    ubData = SpiReadByte();
    CSN = 1;
    // enableInterrupts();
    return ubData;
}


/***********************************************************************
* Funcation ：void SpiReadData(unsigned char *pBuf,unsigned char Length)
* Description： form AX5043_FIFODATA read data
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SpiReadData(unsigned char *pBuf,unsigned char Length)
{
    unsigned char  ubi;
    disableInterrupts();
    CSN = 0;
    SpiWriteByte(REG_AX5043_FIFODATA & 0x7F);  //read common  bit7=0
    for (ubi = 0x00;ubi < Length;ubi++)
    {
        pBuf[ubi] = SpiReadByte();
    }
    CSN = 1;
    enableInterrupts();
}

/***********************************************************************
* Funcation ：
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
char SetChannel(unsigned char ubNum )
{
    unsigned char ubRang;
    unsigned char ubTemp;          //ubData;
    unsigned long RadioChannel;

    ubRang = AxRadioPHYChanPllRng_RX[ubNum];

    //RadioChannel  = ChannelTable[ubNum];
    RadioChannel  = WirelessFreqConfigure(configData[FREQ_LOCAT],configData[FREQ_LOCAT+1],configData[FREQ_LOCAT+2],ubNum);

    //RadioChannel += axradio_curfreqoffset;  //加上频率补偿
    ubTemp = SpiReadSingleAddressRegister(REG_AX5043_PLLLOOP);  //读PLLLOOP
    if (ubTemp & 0x80)
    {
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGA, ubRang&0x0F);
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA0, RadioChannel&0xFF);       //AX5043_FREQA0 = f;
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA1, (RadioChannel>>8)&0xFF);  //AX5043_FREQA1 = f >> 8;
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA2, (RadioChannel>>16)&0xFF); //AX5043_FREQA2 = f >> 16;
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA3, (RadioChannel>>24)&0xFF); //AX5043_FREQA3 = f >> 24;
    }
    else
    {
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGB, ubRang&0x0F);     //AX5043_PLLRANGINGB = rng & 0x0F;
        SpiWriteSingleAddressRegister(REG_AX5043_FREQB0, RadioChannel&0xFF);       //AX5043_FREQB0 = f;
        SpiWriteSingleAddressRegister(REG_AX5043_FREQB1, (RadioChannel>>8)&0xFF);  //AX5043_FREQB1 = f >> 8;
        SpiWriteSingleAddressRegister(REG_AX5043_FREQB2, (RadioChannel>>16)&0xFF); //AX5043_FREQB2 = f >> 16;
        SpiWriteSingleAddressRegister(REG_AX5043_FREQB3, (RadioChannel>>24)&0xFF); //AX5043_FREQB3 = f >> 24;
    }
    ubTemp^=0x80 ;
    SpiWriteSingleAddressRegister(REG_AX5043_PLLLOOP, ubTemp);                        //AX5043_PLLLOOP ^= 0x80;

    return AXRADIO_ERR_NOERROR;
}

/***********************************************************************
* Funcation ：
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void AX5043_Reset(void)
{
    unsigned char ubAddres;

    CSN = 1;
    CSN = 0;
    while (SDI == 0);
    ubAddres = REG_AX5043_PWRMODE|0x80 ;                            //write comm  bit7=1
    SpiWriteByte(ubAddres);
    SpiWriteByte(0x80);
    CSN = 1;
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, 0x00);        //RADIO_PWRMODE = PWRMODE_PWRDOWN=0x00

    SpiWriteSingleAddressRegister(REG_AX5043_SCRATCH, 0x55);
    do
    {
        //delay(1);
        asm("NOP");
        asm("NOP");
        ubAddres = SpiReadSingleAddressRegister(REG_AX5043_SCRATCH);
    }
    while (ubAddres != 0x55);

    SpiWriteSingleAddressRegister(REG_AX5043_PINFUNCIRQ, 0x01);  // IRQ Line 1   001 IRQ Output ’1’
    asm("NOP");
    asm("NOP");
    SpiWriteSingleAddressRegister(REG_AX5043_PINFUNCIRQ, 0x00);  //IRQ Line 0  000 IRQ Output ’0’
    SpiWriteSingleAddressRegister(REG_AX5043_PINFUNCIRQ, 0x03);  //011 IRQ Output Interrupt Request
}

/***********************************************************************
* Funcation ：void InitAx5043REG(void)
* Description：init all register and local pktaddr
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void InitAx5043REG(void)
{
    unsigned char ubi;

    //init all register
    for (ubi = 0; ((RegisterVaule[ubi][0] != 0xFF)&&(RegisterVaule[ubi][1] != 0xDD));ubi++)
    {
        // for(ubi = 0; ubi < 127;ubi++){
        SpiWriteLongAddressRegister(RegisterVaule[ubi][0] , RegisterVaule[ubi][1] );
    }


    
    WirelessBitRateConfigure(configData[DATARATE_LOCAT]);
    WirelessTxPowerConfigure(configData[POWER_LOCAT]);
    if(HighPowerFlag) WirelessTxPowerConfigure(MORE_TX_POWER);
   SpiWriteAutoAddressRegister(REG_AX5043_RSSIABSTHR, 0xE0);
   SpiWriteAutoAddressRegister(REG_AX5043_PKTSTOREFLAGS, (0x13)); 	
   SpiWriteAutoAddressRegister(REG_AX5043_BGNDRSSITHR, 0x00);		
   SpiWriteAutoAddressRegister(REG_AX5043_RSSIREFERENCE,(int8_t)(0xF9+0x40));    
    //SpiWriteSingleAddressRegister(REG_AX5043_PKTLENOFFSET,SpiReadSingleAddressRegister(REG_AX5043_PKTLENOFFSET)+0);
    // AX5043_PKTLENOFFSET += axradio_framing_swcrclen;             // add len offs for software CRC16 (used for both, fixed and variable length packets
    SpiWriteSingleAddressRegister(REG_AX5043_PINFUNCIRQ, 0x03);    // AX5043_PINFUNCIRQ = 0x03;  use as IRQ pin
    //SpiWriteLongAddressRegister(REG_AX5043_PKTSTOREFLAGS , 0x15); //0x15 AX5043_PKTSTOREFLAGS(0x232) = 0x15;store RF offset, RSSI and delimiter timing

}

/***********************************************************************
* Funcation ：void Ax5043SetRegisters_TX(void)
* Description：set Tx register
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void Ax5043SetRegisters_TX(void)
{
    unsigned char ubi;

    for (ubi = 0x00;ubi < 8;ubi++)
    {
        SpiWriteLongAddressRegister(TXMODE_REG[ubi][0] , TXMODE_REG[ubi][1] );
    }
#if (XTAL_FREQ == 26000000)
    switch (configData[DATARATE_LOCAT])
    {
        case 0:
        case 1:
        case 2:
            SpiWriteLongAddressRegister(REG_AX5043_PLLLOOP,0x0b);
            SpiWriteLongAddressRegister(REG_AX5043_PLLCPI,0x10);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            SpiWriteLongAddressRegister(REG_AX5043_PLLLOOP,0x09);
            SpiWriteLongAddressRegister(REG_AX5043_PLLCPI,0x02);
            break;
    }
#endif

}

/***********************************************************************
* Funcation ：void Ax5043SetRegisters_RX(void)
* Description：set Rx register
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void Ax5043SetRegisters_RX(void)
{
    unsigned char ubi;

    for (ubi = 0x00;ubi < 8;ubi++)
    {
        SpiWriteLongAddressRegister(RXMODE_REG[ubi][0] , RXMODE_REG[ubi][1] );
    }
#if (XTAL_FREQ == 26000000)
    switch (configData[DATARATE_LOCAT])
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            SpiWriteLongAddressRegister(REG_AX5043_PLLLOOP,0x0b);
            SpiWriteLongAddressRegister(REG_AX5043_PLLCPI,0x10);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            SpiWriteLongAddressRegister(REG_AX5043_PLLLOOP,0x09);
            SpiWriteLongAddressRegister(REG_AX5043_PLLCPI,0x01);
            break;
    }
#endif

}

/***********************************************************************
* Funcation ：void RdioXtalON(void)
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void RdioXtalON(void)
{
    unsigned char ubTemp;

    ubRFState = trxstate_wait_xtal;
    ubTemp = SpiReadSingleAddressRegister(REG_AX5043_IRQMASK1);
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, ubTemp|0x01);            // enable xtal ready interrupt
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON); //AX5043_PWRSTATE_XTAL_ON=0x5
    do
    {
        // delay(1);
        asm("NOP");
        asm("NOP");
    }
    while (ubRFState == trxstate_wait_xtal);     //wait for ubRFState=trxstate_xtal_ready
}

/***********************************************************************
* Funcation ：
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
unsigned char InitAX5043(void)
{
    unsigned char ubi,ubTemp;
    unsigned long uwFreq;
    U8 pllloop_save, pllcpi_save;

    SDO = 0;
    SCK = 0;
    CSN = 1;
    //asm("FCLR I");
    disableInterrupts(); //Interrupt disable
    AX5043_Reset();
    InitAx5043REG();
    // asm("FSET I");
    enableInterrupts();// Interrupt enable
    RdioXtalON();

    Ax5043SetRegisters_RX();
    pllloop_save = SpiReadSingleAddressRegister(REG_AX5043_PLLLOOP);
    pllcpi_save = SpiReadSingleAddressRegister(REG_AX5043_PLLCPI);

    SpiWriteSingleAddressRegister(REG_AX5043_PLLLOOP, 0x09);// AX5043_PLLLOOP = 0x09;default 100kHz loop BW for ranging
    SpiWriteSingleAddressRegister(REG_AX5043_PLLCPI, 0x08); //AX5043_PLLCPI = 0x08;

    for (ubi = 0x00; ubi < CHANNEL_NUM ;ubi++)
    {
        uwFreq = WirelessFreqConfigure(configData[FREQ_LOCAT],configData[FREQ_LOCAT+1],configData[FREQ_LOCAT+2],ubi);
        // uwFreq = ChannelTable[ubi];
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA0, uwFreq&0xFF);
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA1, (uwFreq>>8)&0xFF);
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA2, (uwFreq>>16)&0xFF);
        SpiWriteSingleAddressRegister(REG_AX5043_FREQA3, (uwFreq>>24)&0xFF);
        if ( !(0x0a & 0xF0) )     // start values for ranging available
        {
            ubTemp = 0x0a | 0x10;
        }
        else
        {
            ubTemp = 0x18;
            if (ubi)
            {
                ubTemp = AxRadioPHYChanPllRng_RX[ubi - 1];
                if ( ubTemp & 0x20)
                    ubTemp = 0x08;
                ubTemp &= 0x0F;
                ubTemp |= 0x10;
            }
        }
        ubRFState = trxstate_pll_ranging;
        SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x10);     // enable pll autoranging done interrupt
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGA, ubTemp); //init ranging process starting from "range"
        do
        {
            //delay(1);
            asm("NOP");
            asm("NOP");
        }
        while (ubRFState == trxstate_pll_ranging); //while(ubRFState != trxstate_pll_ranging_done); wait for trxstate_pll_ranging_done

        ubRFState = trxstate_off;
        SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x00);
        SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x00);     //AX5043_IRQMASK1 = 0x00;
        ubTemp = SpiReadSingleAddressRegister(REG_AX5043_PLLRANGINGA);
        AxRadioPHYChanPllRng_RX[ubi] = ubTemp;                        //AX5043_PLLRANGINGA;
    }

    //================================================================
    Ax5043SetRegisters_TX();
    SpiWriteSingleAddressRegister(REG_AX5043_PLLLOOP, 0x09); // AX5043_PLLLOOP = 0x09;default 100kHz loop BW for ranging
    SpiWriteSingleAddressRegister(REG_AX5043_PLLCPI, 0x08);  //AX5043_PLLCPI = 0x08;

    // for (ubi=0x00;ubi < 16 ;ubi++)
    for (ubi = 0x00;ubi < CHANNEL_NUM ;ubi++)
    {
        if ( !(0x0a & 0xF0) )   // start values for ranging available
        {
            ubTemp = 0x0a | 0x10;
        }
        else
        {
            ubTemp = AxRadioPHYChanPllRng_RX[ubi];
            if ( ubTemp & 0x20)
                ubTemp = 0x08;
            ubTemp &= 0x0F;
            ubTemp |= 0x10;
        }

        ubRFState = trxstate_pll_ranging;
        SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x10);      // enable pll autoranging done interrupt
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGA, ubTemp); //  init ranging process starting from "range"

        do
        {
            asm("NOP");
            asm("NOP");//delay(1);
        }
        while (ubRFState == trxstate_pll_ranging);//while(ubRFState != trxstate_pll_ranging_done);

        ubRFState = trxstate_off;
        SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x00);     //disable low byte interrupt
        SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x00);     //disable high byte interrupt
        ubTemp=SpiReadSingleAddressRegister(REG_AX5043_PLLRANGINGA);
        AxRadioPHYChanPllRng_TX[ubi] = ubTemp;                        //AX5043_PLLRANGINGA;
    }

    //asm("FCLR I");
    //disableInterrupts();//Interrupt all disable

    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN); // AX5043_PWRMODE = AX5043_PWRSTATE_POWERDOWN=0x00;
    ubTemp = AxRadioPHYChanPllRng_RX[0]&0x0F;                                     //AX5043_PLLRANGINGA = axradio_phy_chanpllrng_rx[0] & 0x0F;
    SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGA,ubTemp );

    uwFreq = WirelessFreqConfigure(configData[FREQ_LOCAT],configData[FREQ_LOCAT+1],configData[FREQ_LOCAT+2],0);
    //uwFreq = ChannelTable[0];

    SpiWriteSingleAddressRegister(REG_AX5043_FREQA0, uwFreq&0xFF);
    SpiWriteSingleAddressRegister(REG_AX5043_FREQA1, (uwFreq>>8)&0xFF);
    SpiWriteSingleAddressRegister(REG_AX5043_FREQA2, (uwFreq>>16)&0xFF);
    SpiWriteSingleAddressRegister(REG_AX5043_FREQA3, (uwFreq>>24)&0xFF);
    AxRadioPHYChanPllRng_TX[0] = AxRadioPHYChanPllRng_RX[0];

    //Whether all the channels in the range of Settings
    //for (ubi = 0x00;ubi < 16 ;ubi++)
    SpiWriteSingleAddressRegister(REG_AX5043_PLLLOOP, pllloop_save);
    SpiWriteSingleAddressRegister(REG_AX5043_PLLCPI,pllcpi_save);
	
	SpiWriteLongAddressRegister(REG_AX5043_POWCTRL1, 0x07);

    for (ubi = 0x00;ubi < CHANNEL_NUM ;ubi++)
    {
        if ((AxRadioPHYChanPllRng_TX[ubi] |AxRadioPHYChanPllRng_RX[ubi] ) & 0x20)
        {
            return AXRADIO_ERR_RANGING;
        }
    }
    return AXRADIO_ERR_NOERROR;
}


/***********************************************************************
* Funcation ：void SetTransmitMode(void)
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SetTransmitMode(void)
{
    unsigned char Rng;
    unsigned char ubData;

    Ax5043SetRegisters_TX();
    Rng = AxRadioPHYChanPllRng_TX[configData[CHANNEL_LOCAT]];
    ubData=SpiReadSingleAddressRegister(REG_AX5043_PLLLOOP);
    if (ubData & 0x80)
    {
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGB, Rng&0x0F);   //AX5043_PLLRANGINGB = rng & 0x0F;
    }
    else
    {
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGA , Rng&0x0F);   //  AX5043_PLLRANGINGA = rng & 0x0F;
    }
}
/***********************************************************************
* Funcation ：
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void AX5043ReceiverON(void)
{
    unsigned char ubTemp;
    // SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON); // Crystal Oscillator enabled
    //SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_FIFO_ON); //AX5043_PWRMODE = AX5043_PWRSTATE_FIFO_ON;
    //Ax5043SetRegisters_RX();
    //REG_AX5043_RSSIREFERENCE=0x22C;axradio_phy_rssireference=0x19
//    SpiWriteLongAddressRegister(REG_AX5043_RSSIREFERENCE, 0x19);
    SpiWriteLongAddressRegister(REG_AX5043_TMGRXPREAMBLE1, 0x00); //   AX5043_TMGRXPREAMBLE1(0x229) = 0x00;
    //SpiWriteLongAddressRegister(REG_AX5043_PKTSTOREFLAGS , 0x40); // AX5043_PKTSTOREFLAGS &= (uint8_t)~0x40;
    ubTemp = SpiReadLongAddressRegister(REG_AX5043_PKTSTOREFLAGS);
    ubTemp &= ~0x40;
    SpiWriteLongAddressRegister(REG_AX5043_PKTSTOREFLAGS , ubTemp ); // AX5043_PKTSTOREFLAGS &= (uint8_t)~0x40;

    //清除FIFO
    SpiWriteSingleAddressRegister(REG_AX5043_FIFOSTAT,0x03);  // AX5043_FIFOSTAT(0x28) = 3;  bit[5:0]=0b00011 Clear FIFO Data and Flags  清除FIFO page 69
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE,AX5043_PWRSTATE_FULL_RX); //AX5043_PWRSTATE_FULL_RX =0x09; bit[3:0]=1001 Receiver Running page55 启动接收
    ubRFState = trxstate_rx;
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0,0x01);//AX5043_IRQMASK0（0x07） = 0x01;  enable FIFO not empty
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1,0x00);
}


/***********************************************************************
* Funcation ：
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void SetReceiveMode(void)
{
    unsigned char ubRng;
    unsigned char ubData;

    //ChannelPtr = 0x00;
    //SetChannel(configData[CHANNEL_LOCAT]);
    Ax5043SetRegisters_RX();

    ubRng = AxRadioPHYChanPllRng_RX[configData[CHANNEL_LOCAT]]; //axradio_phy_chanpllrng_rx[axradio_curchannel];
    ubData=SpiReadSingleAddressRegister(REG_AX5043_PLLLOOP);
    if (ubData & 0x80)
    {
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGB, ubRng&0x0F);   //AX5043_PLLRANGINGB = rng & 0x0F;
    }
    else
    {
        SpiWriteSingleAddressRegister(REG_AX5043_PLLRANGINGA , ubRng&0x0F);   //  AX5043_PLLRANGINGA = rng & 0x0F;
    }
}
signed char Rssidata;
/***********************************************************************
* Funcation ：
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void ReceiveData(void)
{
    unsigned char ubDataLen,ubTepm;
    unsigned char ubTemCom;
    unsigned char ubDataFlag;
    unsigned char ubOffSet;
    signed char ubRssi;
    unsigned long uwFreqOffSet;    //RX Freq Offset
    int  uwRSSI;

    ubDataLen = SpiReadSingleAddressRegister(REG_AX5043_RADIOEVENTREQ0);  //RADIOEVENTREQ0(0x0F); bit0=1  FIFO not empty interrupt pending

    while (SpiReadSingleAddressRegister(REG_AX5043_IRQREQUEST0) & 0x01)   // bit0=1  FIFO not empty interrupt pending
    {
        ubTemCom = SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);   // 第1个字节 read command
        ubDataLen = ubTemCom >>5 ;
        if (ubDataLen == 0x07)             //如果是bit[7:5]=111  表示可变的数据长度
        {
            ubDataLen = SpiReadSingleAddressRegister(REG_AX5043_FIFODATA); // 第2个字节 7 means variable length, -> get length byte  获得 数据长度
        }
        ubTemCom &=0x1F;
        switch (ubTemCom)
        {
            case AX5043_FIFOCMD_DATA: //0x0001 1111  检测bit[4:0]的内容  ubTemCom =0x01  表示接收是数据,
                if (!ubDataLen)
                    break;
                ubDataFlag = SpiReadSingleAddressRegister(REG_AX5043_FIFODATA); //第3个字节  标志:0 /0/ UNENC/ RAW/ NOCRC/ RESIDUE /PKTEND /PKTSTART
                ubDataLen--;
                SpiReadData(RXBuff,ubDataLen);         //ax5043_readfifo(axradio_rxbuffer, len);  //读FIFO中数据
                RxLen = ubDataLen;
				
                ubTepm = SpiReadSingleAddressRegister(REG_AX5043_IRQMASK0);
                SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, ubTepm&0xFE);   // disable FIFO not empty irq
                break;

            case AX5043_FIFOCMD_RFFREQOFFS:
                if (ubDataLen != 3)
                    goto dropchunk;
                ubOffSet = SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);
                ubOffSet &= 0x0F;
                ubOffSet |= 1+(~(ubOffSet&0x08));
                uwFreqOffSet=((signed char)ubOffSet)>>8 ;
                uwFreqOffSet <<=8;
                uwFreqOffSet |= ubOffSet;
                uwFreqOffSet <<=8;
                ubOffSet = SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);
                uwFreqOffSet |= ubOffSet;
                uwFreqOffSet <<=8;
                ubOffSet =SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);
                uwFreqOffSet |= ubOffSet;
                break;

            case AX5043_FIFOCMD_RSSI:
                if (ubDataLen != 1)
                    goto dropchunk;
                ubRssi = SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);
                uwRSSI = ubRssi - 64;
                Rssidata= uwRSSI;
                sysdata.RSSIDATA= uwRSSI;
                break;

            case AX5043_FIFOCMD_TIMER:
                if (ubDataLen != 3)
                    goto dropchunk;
                SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);
                SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);
                SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);
                break;

            default:
            dropchunk:
                if (!ubDataLen)
                    break;
                ubTepm = ubDataLen;
                do
                {
                    SpiReadSingleAddressRegister(REG_AX5043_FIFODATA);  // purge FIFO
                }
                while (--ubTepm);
                break;
                // ubTemp = SpiReadSingleAddressRegister(REG_AX5043_IRQMASK0);
                // SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0,ubTemp&0xFE); // bit0=0disable FIFO not empty irq
                //SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0,0x00);
                //SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1,0x00);
        }
    }
    ubDataFlag = ubDataFlag;
    uwRSSI = uwRSSI;
}

//=====================================================================
/***********************************************************************
* Funcation ：
* Description：
* InPut    ：
* OutPut   ：
* Return   ：
* Notice   ：
***********************************************************************/
void AX5043_OFF(void)
{
    // ax5043_off_xtal()
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x00);      // IRQ off
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x00);
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON);    //AX5043_PWRMODE = AX5043_PWRSTATE_XTAL_ON=0x00;
    SpiWriteLongAddressRegister(REG_AX5043_LPOSCCONFIG, 0x00);                    //AX5043_LPOSCCONFIG(0x310) = 0x00; // LPOSC off
    ubRFState = trxstate_off;
    //SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN);//AX5043_PWRMODE = AX5043_PWRSTATE_POWERDOWN=0x00;
    
    SpiWriteSingleAddressRegister(0x01, 0x01);
    SpiWriteSingleAddressRegister(0x02, 0x00);
    SpiPowerDown();
    SpiWriteSingleAddressRegister(0x02, 0x01);
    SpiPowerDown();
}

void transmit_packet_task(U8 *Buf, U8 u8Len)
{
    SetChannel(0);
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON); //AX5043_PWRMODE = AX5043_PWRSTATE_XTAL_ON;    Crystal Oscillator enabled
    SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_FIFO_ON); //AX5043_PWRMODE = AX5043_PWRSTATE_FIFO_ON;    FIFO enabled
    SetTransmitMode();//   配置TX寄存器参数，及TX发射频道
    //SetChannel(0);
    TxLen = u8Len;
    TxLen++;
    TXBuff[0] = TxLen;
    memcpy(TXBuff+1, Buf, u8Len); 
    //upDateBuffData( TXBuff );                                     //upload FIFO data
    SpiWriteSingleAddressRegister(REG_AX5043_FIFOTHRESH1, 0x00);  //AX5043_FIFOTHRESH1(0x2E) = 0;
    SpiWriteSingleAddressRegister(REG_AX5043_FIFOTHRESH0, 0x80);  //AX5043_FIFOTHRESH0(0x2F) = 0x80;
    ubRFState = trxstate_tx_xtalwait;
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x00);     //AX5043_IRQMASK0 = 0x00;
    SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x01);     //AX5043_IRQMASK1 = 0x01; // enable xtal ready interrupt

}

static void TransmitData(void)
{
    unsigned char ubFreeCnt;
    //unsigned char ubFlag;
    unsigned char ubi;
    U16 axradio_txbuffer_cnt = 0;
    for (;;)
    {
        ubFreeCnt = SpiReadSingleAddressRegister(REG_AX5043_FIFOFREE0); //uint8_t cnt = AX5043_FIFOFREE0;
        if (SpiReadSingleAddressRegister(REG_AX5043_FIFOFREE1))
            //ubFreeCnt = 0xFF;
            ubFreeCnt = 0xff;
        switch (ubRFState)
        {
            case trxstate_tx_longpreamble: //A
                if (!axradio_txbuffer_cnt)
                {
                    ubRFState = trxstate_tx_shortpreamble;
                    axradio_txbuffer_cnt = axradio_phy_preamble_len;
                    goto shortpreamble;
                }
                ubFreeCnt = 7;
                if (axradio_txbuffer_cnt < 7)
                {
                    ubFreeCnt = axradio_txbuffer_cnt;
                }
                axradio_txbuffer_cnt -= ubFreeCnt;
                ubFreeCnt <<= 5;
                SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA, 0x02 | (3 << 5));     //AX5043_FIFODATA = AX5043_FIFOCMD_REPEATDATA | (3 << 5); //0x62  bit[7:5]= 011 Three byte payload;#define AX5043_FIFOCMD_REPEATDATA 0x02
                SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA, 0x38);     //AX5043_FIFODATA = axradio_phy_preamble_flags;  //axradio_phy_preamble_flags = 0x38; 0b0011 1000  bit5=UNENC bit4=RAW  bit3=NOCRC
                SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA, ubFreeCnt);//AX5043_FIFODATA = cnt;  //重发次数
                SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA, 0x55);      //AX5043_FIFODATA = axradio_phy_preamble_byte; //axradio_phy_preamble_byte = 0x55;  重发的数据
                break;

            case trxstate_tx_shortpreamble:   //B
            shortpreamble:
                if (!axradio_txbuffer_cnt)
                {
                    ubi = SpiReadSingleAddressRegister(REG_AX5043_FRAMING);
                    if ((ubi & 0x0E) == 0x06 && axradio_framing_synclen)                                  //axradio_framing_synclen=32
                    {
                        uint8_t len_byte = axradio_framing_synclen;
                        uint8_t i = (len_byte & 0x07) ? 0x04 : 0;
                        len_byte += 7;
                        len_byte >>= 3;
                        SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,0x01 | ((len_byte + 1) << 5));//AX5043_FIFODATA =   0xA1; //0x01 | ((len_byte + 1) << 5);  //0xA1
                        SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,0x18|i);//AX5043_FIFODATA = 0x18;//axradio_framing_syncflags | i;  //0x18   这个数表示什么?
                        SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,0xCC);//AX5043_FIFODATA = 0xcc;
                        SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,0xAA);//AX5043_FIFODATA = 0xaa;
                        SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,0xCC);//AX5043_FIFODATA = 0xcc;
                        SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,0xAA);//AX5043_FIFODATA = 0xaa;

                        ubRFState = trxstate_tx_packet;
                        break;
                    }
                }
                if (ubFreeCnt < 4)
                    goto fifocommit;
                ubFreeCnt = 255;
                if (axradio_txbuffer_cnt < (255*8))
                    ubFreeCnt = axradio_txbuffer_cnt >> 3;
                if (ubFreeCnt)
                {
                    axradio_txbuffer_cnt -= ((uint16_t)ubFreeCnt) << 3;
                    SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA , AX5043_FIFOCMD_REPEATDATA | (3 << 5));
                    SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA ,axradio_phy_preamble_flags);
                    SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA , ubFreeCnt);
                    SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA ,axradio_phy_preamble_byte);
                    break;
                }
                {
                    uint8_t byte = axradio_phy_preamble_byte;
                    ubFreeCnt = axradio_txbuffer_cnt;
                    axradio_txbuffer_cnt = 0;
                    SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA ,AX5043_FIFOCMD_DATA | (2 << 5));
                    SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA, 0x1C);
                    if (SpiReadLongAddressRegister(REG_AX5043_PKTADDRCFG) & 0x80)
                    {
                        // msb first -> stop bit below
                        byte &= 0xFF << (8-ubFreeCnt);
                        byte |= 0x80 >> ubFreeCnt;
                    }
                    else
                    {
                        // lsb first -> stop bit above
                        byte &= 0xFF >> (8-ubFreeCnt);
                        byte |= 0x01 << ubFreeCnt;
                    }
                    SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA, byte);
                }
                // ubRFState = trxstate_tx_packet;
                break;
            case trxstate_tx_packet:  //C
                //ubFlag = 0x03;
                TxLen +=1 ;
                SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA, AX5043_FIFOCMD_DATA | (7 << 5));//AX5043_FIFODATA = AX5043_FIFOCMD_DATA | (7 << 5);
                SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,TxLen);//write FIFO chunk length byte (data length+MAC_len + Flag byte=64+3+1)
                SpiWriteSingleAddressRegister(REG_AX5043_FIFODATA,0x03);//AX5043_FIFODATA = flags=0x03;
                TxLen--;
                SpiWriteData( TXBuff,TxLen);          //ax5043_writefifo(&axradio_txbuffer[axradio_txbuffer_cnt], cnt);
                ubRFState = trxstate_tx_waitdone;
                SpiWriteSingleAddressRegister(REG_AX5043_RADIOEVENTMASK0,0x01);//AX5043_RADIOEVENTMASK0 = 0x01; // enable REVRDONE event
                SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0,0x40);        //AX5043_IRQMASK0 = 0x40; // enable radio controller irq
                SpiWriteSingleAddressRegister(REG_AX5043_FIFOSTAT,0x04);        //AX5043_FIFOSTAT = 4; // commit
                //LED2 = 0;
                return;

            default:
                return;
        }
    }
fifocommit:
    SpiWriteSingleAddressRegister(REG_AX5043_FIFOSTAT, 4); // commit
}


#ifdef NEW_ENABLE
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
#else
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
#endif
{
#ifdef NEW_ENABLE
    EXTI_ClearITPendingBit(EXTI_IT_Pin3);
#else
    EXTI_ClearITPendingBit(EXTI_IT_Pin4);
#endif
    switch (ubRFState)
    {
        case trxstate_rx: //0x01
//            LED_RX = 0;
            ReceiveData();
            if (RxLen != 0)
            {
                ubReceiveFlag = 0x1;
            }
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0,0x01);
            AX5043ReceiverON();
            break;

        case trxstate_wait_xtal:     //3
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x00);
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x00);//AX5043_IRQMASK1 = 0x00 otherwise crystal ready will fire all over again
            ubRFState = trxstate_xtal_ready;
            break;

        case trxstate_pll_ranging:     //5
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x00);
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x00);//AX5043_IRQMASK1 = 0x00 otherwise autoranging done will fire all over again
            ubRFState = trxstate_pll_ranging_done;
            break;

        case trxstate_pll_settling:     //7
            SpiWriteSingleAddressRegister(REG_AX5043_RADIOEVENTMASK0, 0x00);// AX5043_RADIOEVENTMASK0 = 0x00;
            ubRFState = trxstate_pll_settled;
            break;

        case trxstate_tx_xtalwait:    //9
            SpiReadSingleAddressRegister(REG_AX5043_RADIOEVENTREQ0); //make sure REVRDONE bit is cleared, so it is a reliable indicator that the packet is out
            //AX5043_FIFOSTAT = 3; clear FIFO data & flags (prevent transmitting anything left over in the FIFO,this has no effect if the FIFO is not powerered, in this case it is reset any way)
            SpiWriteSingleAddressRegister(REG_AX5043_FIFOSTAT, 0x03);
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x00);
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x08);// enable fifo free threshold
            ubRFState = trxstate_tx_longpreamble;
            TransmitData();
            SpiWriteSingleAddressRegister(REG_AX5043_PWRMODE, AX5043_PWRSTATE_FULL_TX); //AX5043_PWRMODE = AX5043_PWRSTATE_FULL_TX;
            break;

        case trxstate_tx_waitdone:                 //D
            SpiReadSingleAddressRegister(REG_AX5043_RADIOEVENTREQ0);        //clear Interrupt flag
//            LED_TX = 1;
            if (SpiReadSingleAddressRegister(REG_AX5043_RADIOSTATE) != 0)
            {
                intEventLowerTxCallBack(CBTE_PACKET_SENT);
                //LED_TX = 1;
                break;
            }
            SpiWriteSingleAddressRegister(REG_AX5043_RADIOEVENTMASK0, 0x00);
            intEventLowerTxCallBack(CBTE_PACKET_SENT);
            //AX5043_OFF();
            break;
        default:
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK1, 0x00);
            SpiWriteSingleAddressRegister(REG_AX5043_IRQMASK0, 0x00);
            break;

    } // end switch(axradio_trxstate)
}

/*
void WirelessBitRateConfigure(U8 Param)
{
    switch (Param)
    {
        default:
            WirelessBitRate_20K();
            break;
    }
}

void WirelessBitRate_20K(void)
{
    SpiWriteLongAddressRegister(REG_AX5043_IFFREQ1,AX5043_IFFREQ1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_IFFREQ0,AX5043_IFFREQ0_20K);
    SpiWriteLongAddressRegister(REG_AX5043_DECIMATION,AX5043_DECIMATION_20K);
    SpiWriteLongAddressRegister(REG_AX5043_RXDATARATE2,AX5043_RXDATARATE2_20K);
    SpiWriteLongAddressRegister(REG_AX5043_RXDATARATE1,AX5043_RXDATARATE1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_RXDATARATE0,AX5043_RXDATARATE0_20K);

    SpiWriteLongAddressRegister(REG_AX5043_MAXDROFFSET2,AX5043_MAXDROFFSET2_20K);
    SpiWriteLongAddressRegister(REG_AX5043_MAXDROFFSET1,AX5043_MAXDROFFSET0_20K);
    SpiWriteLongAddressRegister(REG_AX5043_MAXDROFFSET0,AX5043_MAXDROFFSET1_20K);

    SpiWriteLongAddressRegister(REG_AX5043_FSKDMAX1,AX5043_FSKDMAX1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDMAX0,AX5043_FSKDMAX0_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDMIN1,AX5043_FSKDMIN1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDMIN0,AX5043_FSKDMIN0_20K);

    SpiWriteLongAddressRegister(REG_AX5043_TIMEGAIN0,AX5043_TIMEGAIN0_20K);
    SpiWriteLongAddressRegister(REG_AX5043_DRGAIN0,AX5043_DRGAIN0_20K);

    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAINC0,AX5043_FREQUENCYGAINC0_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAIND0,AX5043_FREQUENCYGAIND0_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FREQDEV10,AX5043_FREQDEV10_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FREQDEV00,AX5043_FREQDEV00_20K);

    SpiWriteLongAddressRegister(REG_AX5043_TIMEGAIN1,AX5043_TIMEGAIN1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_DRGAIN1,AX5043_DRGAIN1_20K);


    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAINC1,AX5043_FREQUENCYGAINC1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FREQUENCYGAIND1,AX5043_FREQUENCYGAIND1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDEV2,AX5043_FSKDEV2_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDEV1,AX5043_FSKDEV1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_FSKDEV0,AX5043_FSKDEV0_20K);

    SpiWriteLongAddressRegister(REG_AX5043_TXRATE2,AX5043_TXRATE2_20K);
    SpiWriteLongAddressRegister(REG_AX5043_TXRATE1,AX5043_TXRATE1_20K);
    SpiWriteLongAddressRegister(REG_AX5043_TXRATE0,AX5043_TXRATE0_20K);

}
*/
/********************************the end of file***********************/
