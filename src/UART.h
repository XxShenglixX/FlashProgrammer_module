#ifndef UART_H
#define UART_H
#include "Utils.h"

#define EIGHT_BIT 0x00
#define TX_ENABLE 0x20
#define ASYNC_MODE 0x00
#define HIGH_SPEED_BAUDRATE 0x04
#define SERIAL_ENABLE 0x80
#define RX_ENABLE 0x10
#define TRISC_CONFIG 0xbf
#define TX_HIGH_LEVEL 0x00
#define EIGHT_BIT_BAUDRATE 0x00

// #define NOT_AVAILABLE 0x00
// #define BUFFER0_AVAILABLE 0x01
// #define BUFFER1_AVAILABLE 0x10
// #define BOTH_AVAILABLE 0x11

void uartSendByte(char byteToSend);
uint32 uartGetByte(void);
void uartSetup(int baudrate);
//void uartClose();

// typedef struct
// {
	// State state;
	// uint32 i;
	// uint32 length;
// } TLV_FSM;

#endif // UART_H