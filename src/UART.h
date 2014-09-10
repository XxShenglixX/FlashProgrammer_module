#ifndef  UART_H
#define  UART_H

#include "Utils.h"

#define  EIGHT_BIT              0x00
#define  TX_ENABLE              0x20
#define  ASYNC_MODE             0x00
#define  HIGH_SPEED_BAUDRATE    0x04
#define  SERIAL_ENABLE          0x80
#define  RX_ENABLE              0x10
#define  TRISC_CONFIG           0xbf
#define  TX_HIGH_LEVEL          0x00
#define  EIGHT_BIT_BAUDRATE     0x00
#define Busy1USART( )  (!TXSTAbits.TRMT)

void uartSendByte(char byteToSend);
uint32 uartGetByte(void);
void uartSetup(int baudrate);

#endif // UART_H