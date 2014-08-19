#include "18c.h"
#include <spi.h>
#include <delays.h>
#include "spiMaster.h"
#include "FlashBuffer.h"
#include "DelayedWrite.h"
#include "Utils.h"
#include "SerialInterrupt.h"
#define JumpPin PORTDbits.RD3
#define Baudrate 25


#pragma config OSC = INTIO67 , PWRT = ON , WDT = OFF , DEBUG = ON , LVP =OFF


void main()
{
	FlashBuffer fb;
	TRISDbits.TRISD3 = 0 ;
	JumpPin = 0;


}

