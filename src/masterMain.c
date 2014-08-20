#include "18c.h"
#include <spi.h>
#include <delays.h>
#include "spiMaster.h"
#include "FlashBuffer.h"
#include "DelayedWrite.h"
#include "Utils.h"
#include "SerialInterrupt.h"
#include "TLV.h"
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

extern TLV_Buffer tlvBuf;

void main()
{
	uint8 *ptrTLV;

	FlashBuffer fb;
	fb.buffer = 0 ;
	configureTarget();
	stopTarget();
	
	uartSetup(BaudratePrescaler);
	enableInterrupt();

	while(1)
	{
		writeProgram(&fb,&tlvBuf,&ptrTLV);

	}


	runTarget();
}


