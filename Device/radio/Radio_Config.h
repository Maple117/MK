#ifndef _RADIO_CONFIG_H_
#define _RADIO_CONFIG_H_

//void WirelessBitRateConfigure(U8 BitRate0, U8 BitRate1, U8 BitRate2);
void WirelessBitRateConfigure(U8 BitRate0);
U32 WirelessFreqConfigure(U8 Freq2, U8 Freq1, U8 Freq0, U8 Channel_Num);
void WirelessTxPowerConfigure(U8 TxPower);

#endif