#include "18c.h"
#include <spi.h>
#include <delays.h>
#include "spiMaster.h"
#include "FlashBuffer.h"
#include "DelayedWrite.h"
#include "Utils.h"
#include "SerialInterrupt.h"
#define configureTarget() TRISDbits.TRISD3 = 0 
#define runTargetPin PORTDbits.RD3
#define stopTarget() runTargetPin=0
#define runTarget() runTargetPin=1
#define BaudratePrescaler 25

#define enableInterrupt() do{					\
						INTCONbits.PEIE = 1;	\
						INTCONbits.GIE = 1;	\
					}while(0)
					
#pragma config OSC = INTIO67 , PWRT = ON , WDT = OFF , DEBUG = ON , LVP =OFF


void main()
{
	FlashBuffer fb;
	configureTarget();
	stopTarget();
	
	uartSetup(BaudratePrescaler);
	enableInterrupt();

	if(isFlashBufferNull(&fb));
		while(!flashBufferRead(&fb));



	runTarget();
}


