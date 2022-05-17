#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#define FREQ_LEN    3
#define CHANNEL_LEN	1
#define DATARATE_LEN 1
#define POWER_LEN 1
#define BAUDRATE_LEN  1
#define BAUD_PARITY_LEN 1
#define WAKEUP_TIMER  1

#define FREQ_LOCAT    0
#define CHANNEL_LOCAT	(FREQ_LOCAT+FREQ_LEN)
#define DATARATE_LOCAT  (CHANNEL_LOCAT+CHANNEL_LEN)
#define POWER_LOCAT (DATARATE_LOCAT+DATARATE_LEN)
#define BAUDRATE_LOCAT  (POWER_LOCAT+POWER_LEN)
//#define BAUD_PARITY_LOCAT (BAUDRATE_LOCAT+BAUDRATE_LEN)
#define WAKEUP_TIMER_LOCAT  (BAUDRATE_LOCAT+BAUDRATE_LEN)//(BAUD_PARITY_LOCAT+BAUD_PARITY_LEN)

#define CONFIG_LEN  (WAKEUP_TIMER_LOCAT+WAKEUP_TIMER)
 
#define NOW_CONFIG_LEN  (WAKEUP_TIMER_LOCAT+WAKEUP_TIMER)

#define CONFIG_HEADER_LEN 6

U8 ConfigDataJudge(U8 *pConfigData);
void FLib_MemCpy(void *pDst,void *pSrc,U8 n);
bool_t FLib_MemCmp(void *pCmp1,  void *pCmp2, U8 n);
void FLib_MemSet(void *pDst,uint8_t value,U8 cnt);
extern U8 configData[CONFIG_LEN];
extern U8 reConfigFlag;
void RssiClear(void);
void ConfigTask(void);
void FlashRead(void);
void FlashWrite(void);
void RssiAddressWrite(void);
void RssiWrite(void);
void RssiAddressRead(void);
void RssiRead(void);
void EEPROM_Init(void);
#endif