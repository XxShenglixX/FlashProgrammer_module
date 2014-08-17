#include "Utils.h"
#include "FlashBuffer.h"
#include "spiMaster.h"
#include "DelayedWrite.h"

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
void bufferHandler(uint32 address,uint8* data,uint8 dataCount,FlashBuffer *fb,uint8 *previousSegment,uint8 *memory)
{
	uint8 shift,dataStartPoint = 0;
	uint32 newAddress;
	fb->segment = address / 64 ;

	if (address >= 0x300000) //Address 0x300000 contains configuration register
		{
			spiSendConfig(address,data);// Different method of sending data and programming required
			return ; // quit function
		}

	if (*memory == 0 ) //To be used to store the previous segment accessed
	{
		*previousSegment = fb->segment ;
		while(!flashBufferRead(fb)); //Read the segment from the slave
		*memory = 1; //And do not update previousSegment
	}

	if ( fb->segment == *previousSegment) //Check if writing in the same segment
		{
			if ((address + dataCount)/64 == fb->segment )//Check if cross segment occurs
				dataToBuffer(address,data,dataCount,dataStartPoint,fb);
			else
				{
					shift = 64 - (fb->offset) ; //To calculate how many available bytes left in the flash buffer
					dataToBuffer(address,data,shift,dataStartPoint,fb); //Only copy exact amount of data to fit the flash buffer first
					while(!flashBufferFlush(fb)); //Flush the buffer until success

					fb->offset = 0 ; //Reset flash buffer offset to 0
					fb->segment ++ ; //Increment the segment
					newAddress = fb->segment * 64 ; // Store the new address since segment has incremented
					*memory = 0 ; //Enable to store the new previous segment

					while(!flashBufferRead(fb)); // Read the segment from the slave
					dataToBuffer(newAddress,data,dataCount-shift,dataStartPoint+shift,fb); // Copy remaining data to the flash buffer
																						   // dataCount - shift ->some of the data was copied previously
																						   // dataStartPoint + shift -> prevent copying copied data
				}
		}
	else
		{
			while(!flashBufferFlush(fb)); 
			while(!flashBufferRead(fb));
			fb->offset = 0 ; 
			*previousSegment = address/64 ;
			dataToBuffer(address,data,dataCount,dataStartPoint,fb);
		}

}



/**
 * Copy data to the slave flash buffer only
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

