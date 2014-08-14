#include "p18f4520.h"
#include "spiMaster.h"
#include "DelayedWrite.h"
#include "Utils.h"
#define Read 0x22

#pragma config OSC = INTIO67 , PWRT = ON , WDT = OFF , DEBUG = ON , LVP =OFF

uint8 tlvFrameReady = 0x11 ;

uint8 readInputBuffer(uint8 *data,FlashBuffer *fb);
void main()
{
		
}

uint8 readInputBuffer(uint8 *data,FlashBuffer *fb)
{
	uint32 address ;
	uint8 *data1 = data+1 , *data2 = data +2 , *data3 = data+3 , *data4 = data+4 ;
	uint8 dataStartPoint ;

	if (tlvFrameReady = 0x01)
		dataStartPoint = 6;
	else (if tvlFrameReady = 0x10)
		dataStartPoint =86;

	address = (uint32)(*data4) << 16 |(uint16)(*data3) << 8 | (*data2) ;
	bufferHandler(address,data,(*data1),dataStartPoint,fb);
	return 1 ;
}

