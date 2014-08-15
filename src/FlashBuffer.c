#include "FlashBuffer.h"
#include "spiMaster.h"
#include "Utils.h"
#include "delays.h"
#define Write 0x11

uint32 flashBufferGetLastAddress(FlashBuffer *fb)
{
	if (fb->segment == -1 )
		return -1 ;
	
	return (fb->offset + (fb->segment * 0x40) ) ;
}

uint8 flashBufferFlush(FlashBuffer *fb)
{
	if(!spiSendCommand(Write))
		return 0 ;
	
 	 if(!spiSendAddress(fb))
		return 0 ;
		
	 if(!spiSendData(fb->buffer,64,0))
		return 0; 
		
	while(!spiReceiveStatus())
	{
		Delay10TCYx(2);
	}
		

		return 1 ; 
}