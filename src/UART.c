#include "p18f4520.h"
#include "UART.h"
#include "Utils.h"
//#pragma config OSC = INTIO67, WDT = OFF, DEBUG = ON, LVP = OFF

uint8 byteToGet;
/*
void uartSendByte(char byteToSend);
int uartGetByte();
void uartSetup(int baudrate);
//void uartClose();

void main(void)
{
	int baudrate = 25;
	char byteToSend = 0x35;

	uartSetup(baudrate);
	////uartSendByte(byteToSend);
	uartGetByte();
	////uartClose();

	while(1);
}*/

/**
 * UART configuration
 * Port C pin 6 as output(Transmit)
 * Port C pin 7 as input(Receive)
 **/
void uartSetup(int baudrate)
{
	OSCCONbits.IRCF1 = 1;			//frequency set to 4Mhz
	while(OSCCONbits.IOFS == 0);	//wait untill stable

	TXSTA = EIGHT_BIT | TX_ENABLE | ASYNC_MODE | HIGH_SPEED_BAUDRATE;
	RCSTA = SERIAL_ENABLE | RX_ENABLE;
	TRISC = TRISC_CONFIG;
	BAUDCON = TX_HIGH_LEVEL | EIGHT_BIT_BAUDRATE;
	SPBRG = baudrate;				//9600 baud @4MHz

	PIE1bits.RCIE = 1;
	PIE1bits.TXIE = 1;
}

/**
 * The transmitter interrupt flag (TXIF) is cleared
 * by a write to TXREG and set when transmission
 * of the byte is complete
 **/
void uartSendByte(char byteToSend)
{
	while(PIR1bits.TXIF == 1)
	{
		TXREG = byteToSend;
	}
}

/**
 * The receiver interrupt Flag (RCIF) is set when an incoming
 * data byte is available in the RCREG. Automatically
 * cleared when the data is read from the RCREG
**/
uint32 uartGetByte(void)
{
	while(PIR1bits.RCIF == 0);
	{
		byteToGet = RCREG;
	}
	return byteToGet;
}