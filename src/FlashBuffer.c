#include "FlashBuffer.h"
#include "spiMaster.h"
#include "Utils.h"
#include <Delays.h>
#define Write 0x11

uint32 flashBufferGetLastAddress(FlashBuffer *fb)
{
	if (fb->segment == -1 )
		return -1 ;
	
	return (fb->offset + (fb->segment * 0x40) ) ;
}

void flashBufferFlush(FlashBuffer *fb)
{
	spiSendCommand(Write);
	spiSendAddress(fb);
	spiSendData(fb->buffer,64,0);
	while(!(spiReceiveStatus))
	{
		Delay10TCYx(2);
	}
}