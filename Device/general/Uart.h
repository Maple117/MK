#ifndef _UART_H
#define _UART_H

#include "Typedef.h"
#include "stm8l15x_usart.h"

void uart0_init(unsigned char param,USART_Parity_TypeDef Parity);
void UartRxISR(void);
void UartSendAsync(void);

enum {SERIAL_RX_QUEUE=0,SERIAL_TX_QUEUE};
#define SERIAL_RX_QUEUE 0
#define SERIAL_TX_QUEUE 0

enum {SERIAL0=0, SERIAL1};
enum {UART_IDLE = 0,UART_SENDING};

enum
{
    UART_CONFIG_WAIT = 0,
    UART_CONFIG_START,
    UART_CONFIG_RECEIVED,
    UART_CONFIG_READ
};

//#define UART_BUFFER_LEN  100
#define MAX_RX_SERIAL_QUEUE_SIZE      200
#define MAX_TX_SERIAL_QUEUE_SIZE      200

#define UART_DATA_MAX_LEN 	100
//#define MAX_SERIAL_QUEUE_SIZE      250
//#define UART_DATA_MAX_LEN 	230

void vSerialQueueInit(int iSerial, int iQueue);
void vSerialQueueFlush(int iSerial, int iQueue);
bool_t bSerialRXQueueEmtpy(int iSerial);
bool_t bSerialTXQueueFull(int iSerial);
uint16 u16GetSerialRXQueueUsedSize(int iSerial);
void vAddToSerialTXQueue(int iSerial, uint8 u8Value);
uint8 u8GetFromSerialRXQueue(int iSerial);
void vWriteToSerialPortAsync(uint8* pData, uint8 nLength);

extern U8 UartReceived;
extern U8 UartTimerOut;

enum
{
    NONE = 0,
    EVEN,
    ODD
};

#define UART_STOP_BIT 2

enum
{
    UART_DATA_7BIT = 7,
    UART_DATA_8BIT
};

#endif
