#include "18c.h"
#include "UART.h"

uint8 byteToGet;

/**
 * UART configuration
 * Port C pin 6 as output(Transmit)
 * Port C pin 7 as input(Receive)
 **/
void uartSetup(uint32 baudrate)
{
    OSCCONbits.IRCF1 = 1;			//frequency set to 4Mhz
    while(OSCCONbits.IOFS == 0);	//wait untill stable

    TXSTA = EIGHT_BIT | TX_ENABLE | ASYNC_MODE | HIGH_SPEED_BAUDRATE;
    RCSTA = SERIAL_ENABLE | RX_ENABLE;
    TRISC = TRISC_CONFIG;
    BAUDCON = TX_HIGH_LEVEL | EIGHT_BIT_BAUDRATE;
    SPBRG = baudrate;				//9600 baud @4MHz

    PIE1bits.RCIE = 1;
}

/**
 * The transmitter interrupt flag (TXIF) is cleared
 * by a write to TXREG and set when transmission
 * of the byte is complete
 **/
void uartSendByte(uint8 byteToSend)
{
    while(Busy1USART());
    TXREG = byteToSend;
}

/**
 * The receiver interrupt Flag (RCIF) is set when an incoming
 * data byte is available in the RCREG. Automatically
 * cleared when the data is read from the RCREG
**/
uint32 uartGetByte(void)
{
    while(PIR1bits.RCIF == 0);
    byteToGet = RCREG;
    return byteToGet;
}