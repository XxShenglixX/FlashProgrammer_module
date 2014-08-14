#include "p18f4520.h"
#include "UART.h"

#define Baudrate 25

void main(void)
{
	uartSetup(Baudrate);
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;

	while(1);
}