#include "Utils.h"
#include "DelayedWrite.h"
#include "FlashBuffer.h"

void bufferHandler(uint32 address,uint8* data,uint8 dataCount,uint8 dataStartPoint,FlashBuffer *fb)
{
	uint8 shift ;
	uint32 newAdd;
	if ( address/64 == fb->segment)
		{
			if ((address + dataCount)/64 == fb->segment )
				dataToBuffer(address,data,dataCount,dataStartPoint,fb);
			else 
				{
					shift = 63 - (fb->offset) ;
					dataToBuffer(address,data,shift,dataStartPoint,fb);
			
					flashBufferFlush(fb);
					fb->segment ++ ;
					newAdd = fb->segment * 64 ;
					dataToBuffer(newAdd,data,dataCount-shift,dataStartPoint+shift,fb);
				}
		}
	else
		{
			flashBufferFlush(fb);
			dataToBuffer(address,data,dataCount,dataStartPoint,fb);
		}

}


void dataToBuffer(uint32 address,uint8* data,uint8 dataCount,uint8 dataStartPoint,FlashBuffer *fb)
{
	uint8 i ,j = dataStartPoint ;

	fb->segment = (address/64)  ;
	fb->offset = (address - (fb->segment * 64)) ;
	
	for (i = 0 ; i < dataCount ; i ++ )
	{
		fb->buffer[fb->offset] = data[j] ;
		(fb->offset) ++ ;
		j ++ ;
	}
	

}