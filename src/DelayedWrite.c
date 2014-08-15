#include "Utils.h"
#include "DelayedWrite.h"
#include "FlashBuffer.h"

/**
 * Copy data from the UART input buffer to the slave flush buffer
 * and perform flush if cross segment occurs or different segment between the slave buffer and UART input buffer
 *
 *
 * Input : address		is the starting address of the data going to be written
 *		   data			contain the pointer to data array going to be written
 *		   dataCount	is the amount of data in the data array
 *		   fb			contain the segment,offset and pointer to the slave flash buffer
 *
 *
 */
void bufferHandler(uint32 address,uint8* data,uint8 dataCount,FlashBuffer *fb)
{
	uint8 shift ;
	uint8 dataStartPoint = 0;
	uint32 newAdd;
	if ( address/64 == fb->segment)
		{
			if ((address + dataCount)/64 == fb->segment )
				dataToBuffer(address,data,dataCount,dataStartPoint,fb);
			else
				{
					shift = 64 - (fb->offset) ;
					dataToBuffer(address,data,shift,dataStartPoint,fb);
					while(!flashBufferFlush(fb));
					fb->offset = 0 ;
					fb->segment ++ ;
					newAdd = fb->segment * 64 ;
					dataToBuffer(newAdd,data,dataCount-shift,dataStartPoint+shift,fb);
				}
		}
	else
		{
			while(!flashBufferFlush(fb));
			dataToBuffer(address,data,dataCount,dataStartPoint,fb);
		}

}



/**
 * Copy data from the UART input buffer to the slave flush buffer only
 *
 * Input : address		is the starting address of the data going to be written
 *		   data			contain the pointer to data array going to be written
 *		   dataCount	is the amount of data in the data array
 *		   fb			contain the segment,offset and pointer to the slave flash buffer
 *
 *
 */
void dataToBuffer(uint32 address,uint8* data,uint8 dataCount,uint8 dataStartPoint,FlashBuffer *fb)
{
	uint8 i ,j= dataStartPoint;

	fb->segment = (address/64)  ;
	fb->offset = (address - (fb->segment * 64)) ;

	for (i = 0 ; i < dataCount ; i ++ )
	{
		fb->buffer[fb->offset] = data[j] ;
		(fb->offset) ++ ;
		j++;
	}


}

