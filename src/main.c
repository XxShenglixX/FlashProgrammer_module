#include "p18f4520.h"
#include "UART.h"
#include "SerialInterrupt.h"

#define Baudrate 25

extern uint8 tlvBuffer[160];
extern uint8 tlvFrameReady = 0x11;

void main(void)
{
	uartSetup(Baudrate);
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;

	while(1);
}