#include "18c.h"
#include <spi.h>
#include <delays.h>
#include "masterMain.h"
#include "spiMaster.h"
#include "FlashBuffer.h"
#include "DelayedWrite.h"
#include "Utils.h"
#include "SerialInterrupt.h"
#include "TLV.h"

#pragma config OSC = INTIO67 , PWRT = ON , WDT = OFF , DEBUG = ON , LVP =OFF

extern TLV_Buffer tlvBuf;

#ifdef __GNUC__
void master_main()
{
	main();
}
#else
void main()
{
    FlashBuffer fb;
    fb.buffer = 0 ;
    
	configurePorts();
    uartSetup(BaudratePrescaler);
    initTlvBuffer(&tlvBuf);
    enableInterrupt();

    // while(stopInterrupt !=0 && isAnyFrameReady(&tlvBuf) !=1)
	while(1)
        writeProgram(&fb,&tlvBuf);
}
#endif

