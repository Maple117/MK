#include "stm8l15x.h"
#include "Typedef.h"
#include "spi.h"
#include "stm8l15x_usart.h"
#include "stm8l15x_syscfg.h"
#include "Configure.h"
#include "Uart.h"
//#include "main.h"

U8 UartState;
U8 UartReceived;
U8 UartTimerOut;

typedef struct
{
    uint16      u16Head;
    uint16      u16Tail;
    uint16      u16Count;
    uint8       buf[MAX_RX_SERIAL_QUEUE_SIZE];
} tsUartRxQueue;

typedef struct
{
    uint16      u16Head;
    uint16      u16Tail;
    uint16      u16Count;
    uint8       buf[MAX_TX_SERIAL_QUEUE_SIZE];
} tsUartTxQueue;


typedef struct
{
    tsUartRxQueue queue[1];
} tsSerialRxQueue;

typedef struct
{
    tsUartTxQueue queue[1];
} tsSerialTxQueue;

tsSerialRxQueue serialRxQueue[1];
tsSerialTxQueue serialTxQueue[1];

static bool_t bSerialRXQueueFull(int iSerial);
static bool_t bSerialTXQueueEmtpy(int iSerial);
static uint8 u8GetFromSerialTXQueue(int iSerial);
void vAddToSerialRXQueue(int iSerial, uint8 u8Value);

// 0: 1200 1: 2400 2: 4800 3: 9600 4: 19200 5: 38400 6: 57600 7:115200
void uart0_init(unsigned char param,USART_Parity_TypeDef Parity)
{
    U32 BaudRate = 0;
    SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA,ENABLE);
    USART_DeInit(USART1);
    //Enable USART clock
    CLK_PeripheralClockConfig((CLK_Peripheral_TypeDef)CLK_Peripheral_USART1, ENABLE);
    // Configure USART Tx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);
    // Configure USART Rx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2, ENABLE);

    switch (param)
    {
        case 0:
            BaudRate = 1200;
            break;
        case 1:
            BaudRate = 2400;
            break;
        case 2:
            BaudRate = 4800;
            break;
        case 3:
            BaudRate = 9600;
            break;
        case 4:
            BaudRate = 19200;
            break;
        case 5:
            BaudRate = 38400;
            break;
        case 6:
            BaudRate = 57600;
            break;
        case 7:
            BaudRate = 115200;
            break;
        default:
            BaudRate = 9600;
            break;
    }
    USART_Init(USART1, BaudRate, USART_WordLength_8b, USART_StopBits_1, Parity, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

    UartTimerOut = ((10*1000/BaudRate)+1)*3;//根据波特率计算3个字节超时时间

    UartState = UART_IDLE;
    UartReceived = 0;

    vSerialQueueInit(SERIAL0, SERIAL_RX_QUEUE);
    vSerialQueueInit(SERIAL0, SERIAL_TX_QUEUE);

    USART_ClearITPendingBit(USART1, USART_IT_TC);
    USART_ClearITPendingBit(USART1, USART_IT_TXE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
}

void UartRxISR(void)
{
    if (!bSerialRXQueueFull(SERIAL0))
    {
        vAddToSerialRXQueue(SERIAL0,USART1->DR);
    }
}

void UartSendAsync(void)
{
    if (!bSerialTXQueueEmtpy(SERIAL0))
    {
        USART1->DR = u8GetFromSerialTXQueue(SERIAL0);
        UartState = UART_SENDING;
        USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    }
    else
    {
        if (reConfigFlag == 0x10)
        {
            reConfigFlag = 0x01;
        }
        USART_ITConfig(USART1, USART_IT_TC, DISABLE);
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

        UartState = UART_IDLE;
        USART_ClearITPendingBit(USART1, USART_IT_TC);
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
    }
}

static bool_t bSerialTXQueueEmtpy(int iSerial)
{
    bool_t bResult = FALSE;
    tsUartTxQueue *pQueue = &serialTxQueue[iSerial].queue[SERIAL_TX_QUEUE];

    if ((pQueue->u16Head == pQueue->u16Tail))
    {
        bResult = TRUE;
    }
    return(bResult);
}

static bool_t bSerialRXQueueFull(int iSerial)
{
    bool_t bResult = FALSE;
    tsUartRxQueue *pQueue = &serialRxQueue[iSerial].queue[SERIAL_RX_QUEUE];
    if (pQueue->u16Tail == (pQueue->u16Head+1)%MAX_RX_SERIAL_QUEUE_SIZE)
    {
        bResult = TRUE;
    }
    return(bResult);
}

void vAddToSerialRXQueue(int iSerial, uint8 u8Value)
{
    tsUartRxQueue *pQueue = &serialRxQueue[iSerial].queue[SERIAL_RX_QUEUE];
    if (!bSerialRXQueueFull(iSerial))
    {
        pQueue->buf[pQueue->u16Head] = u8Value;
        pQueue->u16Head = (pQueue->u16Head + 1) % MAX_RX_SERIAL_QUEUE_SIZE;
        pQueue->u16Count++;
    }
}

static uint8 u8GetFromSerialTXQueue(int iSerial)
{
    tsUartTxQueue *pQueue = &serialTxQueue[iSerial].queue[SERIAL_TX_QUEUE];
    uint8 u8Value = 0;
    if (!bSerialTXQueueEmtpy(iSerial))
    {
        u8Value = pQueue->buf[pQueue->u16Tail];
        pQueue->u16Tail = (pQueue->u16Tail + 1) % MAX_TX_SERIAL_QUEUE_SIZE;
        pQueue->u16Count--;
    }
    return u8Value;
}

void vSerialQueueInit(int iSerial, int iQueue)
{
    vSerialQueueFlush(iSerial, iQueue);
}

static void vSerialQueueFlush(int iSerial, int iQueue)
{
    tsUartTxQueue *pTxQueue = &serialTxQueue[iSerial].queue[iQueue];
	tsUartRxQueue *pRxQueue = &serialRxQueue[iSerial].queue[iQueue];

    pTxQueue->u16Head = 0;
    pTxQueue->u16Tail = 0;
    pTxQueue->u16Count = 0;
	
    pRxQueue->u16Head = 0;
    pRxQueue->u16Tail = 0;
    pRxQueue->u16Count = 0;
}

bool_t bSerialRXQueueEmtpy(int iSerial)
{
    bool_t bResult = FALSE;
    tsUartRxQueue *pQueue = &serialRxQueue[iSerial].queue[SERIAL_RX_QUEUE];

    if ((pQueue->u16Head == pQueue->u16Tail))
    {
        bResult = TRUE;
    }
    else
    {
        bResult = FALSE;
    }
    return(bResult);
}

static bool_t bSerialTXQueueFull(int iSerial)
{
    bool_t bResult = FALSE;
    tsUartTxQueue *pQueue = &serialTxQueue[iSerial].queue[SERIAL_TX_QUEUE];
    if (pQueue->u16Tail == (pQueue->u16Head+1)%MAX_TX_SERIAL_QUEUE_SIZE)
    {
        bResult = TRUE;
    }
    return(bResult);
}

uint16 u16GetSerialRXQueueUsedSize(int iSerial)
{
    tsUartRxQueue *pQueue = &serialRxQueue[iSerial].queue[SERIAL_RX_QUEUE];
    if ( pQueue->u16Head >= pQueue->u16Tail)
    {
        return  (pQueue->u16Head - pQueue->u16Tail);
    }
    else
    {
        return  (MAX_RX_SERIAL_QUEUE_SIZE - pQueue->u16Tail+pQueue->u16Head);
    }
}

void vAddToSerialTXQueue(int iSerial, uint8 u8Value)
{
    tsUartTxQueue *pQueue = &serialTxQueue[iSerial].queue[SERIAL_TX_QUEUE];
    if (!bSerialTXQueueFull(iSerial))
    {
        pQueue->buf[pQueue->u16Head] = u8Value;
        pQueue->u16Head = (pQueue->u16Head + 1) % MAX_TX_SERIAL_QUEUE_SIZE;
        pQueue->u16Count++;
    }
}

uint8 u8GetFromSerialRXQueue(int iSerial)
{
    tsUartRxQueue *pQueue = &serialRxQueue[iSerial].queue[SERIAL_RX_QUEUE];
    uint8 u8Value = 0;
    if (!bSerialRXQueueEmtpy(iSerial))
    {
        u8Value = pQueue->buf[pQueue->u16Tail];
        pQueue->u16Tail = (pQueue->u16Tail + 1) % MAX_RX_SERIAL_QUEUE_SIZE;
        pQueue->u16Count--;
    }
    return u8Value;
}

void vWriteToSerialPortAsync(uint8* pData, uint8 nLength)
{
    int i;
    for (i = 0; i < nLength; i++)
    {
        vAddToSerialTXQueue(SERIAL0, *(pData+i));
    }
    if (UartState != UART_SENDING) //if idle
    {
        USART1->DR =  u8GetFromSerialTXQueue(SERIAL0);
        USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    }
}
