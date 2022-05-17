#include "stm8l15x.h"
#include "Typedef.h"
#include "Uart.h"
#include "stm8l15x_flash.h"
#include "Configure.h"
#include "command.h"
#include "sysdata.h"
#include "24c08.h"
#include "GPIO.h"       
#define READ_CMD  0xF0
#define WRITE_CMD 0x90

void WriteDefaultConfigParm(void);
U8 configData[CONFIG_LEN];
U8 reConfigFlag;

void RssiAddressWrite(void);
void RssiWrite(void);
void FlashWrite(void);
void RssiAddressRead(void);
void RssiRead(void);
void FlashRead(void);

U8 ConfigModuleParm(U8 *pConfigData);

void ConfigTask(void)
{
    if (reConfigFlag == 1)
    {
        reConfigFlag = 0;
        uart0_init(configData[BAUDRATE_LOCAT],(USART_Parity_TypeDef)0x00);
    }
}

U8 ConfigDataJudge(U8 *pConfigData)
{
    U8 CommandHead[CONFIG_HEADER_LEN] = {0xFF,0x56,0xAE,0x35,0xA9,0x55};
    if (FLib_MemCmp(CommandHead, pConfigData, CONFIG_HEADER_LEN) == 0)
    {
        return ConfigModuleParm(pConfigData+6);
    }
    return FALSE;
}
/*
U8 ConfigModuleParm(U8 *pConfigData)
{
    U8 Tempbuf[NOW_CONFIG_LEN+3];
    Tempbuf[0] = 0x24;
    Tempbuf[1] = 0x16;
    Tempbuf[2] = 0x01;

    if (*(pConfigData + 0) == READ_CMD)//0:Control Byte
    {
        //FlashRead();
        *(pConfigData + 0) = 0x24;
        //FLib_MemCpy(pConfigData+1, configData,  NOW_CONFIG_LEN);
        FLib_MemCpy(Tempbuf+3, configData,  NOW_CONFIG_LEN);
        vWriteToSerialPortAsync(Tempbuf, NOW_CONFIG_LEN+3);
        //vWriteToSerialPortAsync(pConfigData, NOW_CONFIG_LEN);
        return TRUE;
    }
    else if (*(pConfigData + 0) == WRITE_CMD)
    {
        FLib_MemCpy(configData, pConfigData+1, NOW_CONFIG_LEN);
        FlashWrite();
        *(pConfigData + 0) = 0x24;
        //vWriteToSerialPortAsync(pConfigData, NOW_CONFIG_LEN);
        FLib_MemCpy(Tempbuf+3, pConfigData+1, NOW_CONFIG_LEN);
        vWriteToSerialPortAsync(Tempbuf, NOW_CONFIG_LEN+3);
        //reConfigFlag = 1;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
*/
U8 ConfigModuleParm(U8 *pConfigData)
{
    if (*(pConfigData + 0) == READ_CMD)//0:Control Byte
    {
        //FlashRead();
        *(pConfigData + 0) = 0x24;
        FLib_MemCpy(pConfigData+1, configData,  NOW_CONFIG_LEN);
        //FLib_MemCpy(Tempbuf+3, configData,  NOW_CONFIG_LEN);
        //vWriteToSerialPortAsync(Tempbuf, NOW_CONFIG_LEN+1);
        vWriteToSerialPortAsync(pConfigData, NOW_CONFIG_LEN+1);
        return TRUE;
    }
    else if (*(pConfigData + 0) == WRITE_CMD)
    {
        FLib_MemCpy(configData, pConfigData+1, NOW_CONFIG_LEN);
        //FlashWrite();
        *(pConfigData + 0) = 0x24;
        vWriteToSerialPortAsync(pConfigData, NOW_CONFIG_LEN+1);
       // FLib_MemCpy(Tempbuf+3, pConfigData+1, NOW_CONFIG_LEN);
       // vWriteToSerialPortAsync(Tempbuf, NOW_CONFIG_LEN+3);
        //reConfigFlag = 1;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

u8 write_eeprom(uint32_t address,uint8_t * data,u8 size)
{
    u8 check = 0;
    u8 i;
    if(IS_FLASH_DATA_EEPROM_ADDRESS(address))
    {
        FLASH_Unlock(FLASH_MemType_Data);
        for(i = 0; i < size ; i ++)
        {
                FLASH_ProgramByte(address + i,data[i]);
                check += data[i];
                while(!(FLASH->IAPSR & 0x04));
        }
        FLASH_ProgramByte(address + i,check);
        while(!(FLASH->IAPSR & 0x04));
        FLASH_Lock(FLASH_MemType_Data);
    }
    return 0;
}

u8 read_eeprom(uint32_t address,uint8_t * data,u8 size)
{
    u8 check = 0;
    u8 i;
    if(IS_FLASH_DATA_EEPROM_ADDRESS(address))
    {
        //FLASH_Unlock(FLASH_MemType_Data);
        for(i = 0; i < size ; i ++)
        {
            //data[i] = *(__IO uint8_t*)(address + i);
            data[i] = FLASH_ReadByte(address + i);
            check += data[i];
        }
        //FLASH_Lock(FLASH_MemType_Data);
        
        if(check == FLASH_ReadByte(address + i) && data[0] == 0xaa) return 0;
        else return 1;
    }
    return 1;
}

void EEPROM_Init(void)
{
//  I2C_VCC_STATE=1;
//  RssiAddressRead();
//  I2C_VCC_STATE=0;
}

#define  address_eeprom  ((uint32_t)0x0000)
void RssiAddressWrite(void)
{
  write_eeprom(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+0x20, (u8 *)&adderss,4);//sizeof(ADDRESS)
  return;
}

void RssiWrite(void)
{
   WriteEEP(address_eeprom+adderss*sizeof(DATA), (u8 *)&DATA,sizeof(DATA));
  return;
}

void RssiClear(void)
{
    for(adderss=0;adderss<max;adderss++)
    {
       IWDG_ReloadCounter(); 
//      sysdata.SendNumer=0xff;
//      sysdata.RSSIDATA=0xff;
       DATA[0]=0XFF;
       DATA[1]=0XFF;
       DATA[2]=0XFF;
       DATA[3]=0XFF;        
//      RssiWrite();
    }
    adderss=0;
//    RssiAddressWrite();
    return;
}

void RssiAddressRead(void)
{
 read_eeprom(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS+0x20, (u8 *)&adderss,4);//sizeof(ADDRESS)
  if(adderss>=(max-1))//
  {
    adderss=0;
//    RssiAddressWrite();
  }
  return;
}

void RssiRead(void)
{
  ReadEEP(address_eeprom+adderss*sizeof(DATA), (u8 *)&DATA,sizeof(DATA));
  return;
}


void FlashWrite(void)
{
  write_eeprom(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS, (u8 *)&ConfigData,sizeof(ConfigData));
  return;
}

void FlashRead(void)
{
#if 1
    U8 Flag = 0;
    disableInterrupts();
    read_eeprom(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS, (u8 *)&ConfigData,sizeof(ConfigData));
    if (ConfigData.State != 0xaa)
    {
        ConfigData.State = 0xaa;
       ConfigData.DeviceID = 30000001;
       //ConfigData.ObjID = 10000001;
       ConfigData.ObjID = 0;
        ConfigData.GroupID = 0;
        
        FlashWrite();
    }
    
    if (Flag == 0)
    {
        WriteDefaultConfigParm();
    }
    else
    {
        WriteDefaultConfigParm();
        /*
          U8 CrcCheck = 0;
          for ( i = 0; i < CONFIG_LEN - 2; i++ )
          {
              CrcCheck += configData[i];
          }
          if (CrcCheck != configData[CONFIG_LEN - 1])
          {
              WriteDefaultConfigParm();
          }
        */
        //reConfigFlag = 1;
    }
    enableInterrupts();
#endif
}

void WriteDefaultConfigParm(void)
{
    configData[FREQ_LOCAT] = 0x06;//434.7
    configData[FREQ_LOCAT+1] = 0xA2;
    configData[FREQ_LOCAT+2] = 0x0c;
    
//    configData[FREQ_LOCAT] = 0x0D;//868M
//    configData[FREQ_LOCAT+1] = 0x3E;
//    configData[FREQ_LOCAT+2] = 0xA0;
    
//    configData[FREQ_LOCAT] = 0x00;//40.68M
//    configData[FREQ_LOCAT+1] = 0x9E;
//    configData[FREQ_LOCAT+2] = 0xE8;
    /*
    configData[FREQ_LOCAT] = 0x07;//433M
    configData[FREQ_LOCAT+1] = 0x2B;
    configData[FREQ_LOCAT+2] = 0xf0;
   */ 
    configData[CHANNEL_LOCAT] = 0x00;
	 
    configData[DATARATE_LOCAT] = 0x02;

    configData[POWER_LOCAT] = 3;//15dbm
    configData[BAUDRATE_LOCAT] = 0x03;//9600
    //configData[BAUD_PARITY_LOCAT] = NONE; 
    configData[WAKEUP_TIMER_LOCAT] = 0x05;//1S
    //FlashWrite();
    //reConfigFlag = 1;
}
 
void FLib_MemCpy 
(
    void *pDst, // Destination buffer 目标地址
    void *pSrc, // Source buffer      源地址
    U8 n   // uint8_t count      字节数量
)
{
    // Code looks weird, but is quite optimal on a HCS08 CPU
    while (n)
    {
        *((U8 *)pDst) = *((U8 *)pSrc);
        pDst=((U8 *)pDst)+1;
        pSrc=((U8 *)pSrc)+1;
        n--;
    }
}

bool_t FLib_MemCmp
(
    void *pCmp1,  // Buffer with to be compared with pCmp2
    void *pCmp2,  // Buffer with to be compared with pCmp1
    U8 n     // uint8_t count
)
{
    while (n)
    {
        if ( *((U8 *)pCmp1) != *((U8 *)pCmp2) )
            return 1;

        pCmp2=(U8 *)pCmp2+1;
        pCmp1=(U8 *)pCmp1+1;
        n--;
    }
    return 0;
}

void FLib_MemSet
(
    void *pDst,    // Buffer to be reset
    U8 value, // uint8_t value
    U8 cnt    // uint8_t count
)
{
    while (cnt)
    {
        ((U8 *)pDst)[--cnt] = value;
    }
}