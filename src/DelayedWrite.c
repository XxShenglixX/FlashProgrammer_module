#include "Utils.h"
#include "FlashBuffer.h"
#include "spiMaster.h"
#include "DelayedWrite.h"

/**
 * Copy data from the UART input buffer to the slave flash buffer
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
void bufferHandler(uint32 address,uint8* data,uint8 length,FlashBuffer *fb)
{
	uint8 dataStartPoint = 0;

	if(isConfigData(address))
	{
		configurationDataHandler(address,data,fb);
		return ;
	}

	if(isSameSegment(address,fb))
	{
		if(isAddressInRange(address,length,fb))
			dataToBuffer(address,data,length,dataStartPoint,fb);

		else
			crossSegmentHandler(address,data,length,dataStartPoint,fb);
	}
	else
		differentSegmentHandler(address,data,length,dataStartPoint,fb);
}




/**
 * Copy data to the slave flash buffer only
 *
 * Input : address			is the starting address of the data going to be written
 *		   data				contain the pointer to data array going to be written
 *		   length			is the amount of data in the data array
 *		   dataStartPoint	is the starting location of data going to be accessed
 *		   fb				contain the segment,offset and pointer to the slave flash buffer
 *
 *
 */
void dataToBuffer(uint32 address,uint8* data,uint8 length,uint8 dataStartPoint,FlashBuffer *fb)
{
	uint8 i ,j= dataStartPoint;

	fb->segment = (address/64)  ;
	fb->offset = (address - (fb->segment * 64)) ;

	for (i = 0 ; i < length ; i ++ )
	{
		fb->buffer[fb->offset] = data[j] ;
		(fb->offset) ++ ;
		j++;
	}


}

/**
 * Handle cases when cross segment of flash buffer occurs
 *
* Input : address			is the starting address of the data going to be written
 *		  data				contain the pointer to data array going to be written
 *		  length			is the amount of data in the data array
 *		  dataStartPoint	is the starting location of data going to be accessed
 *		  fb				contain the segment,offset and pointer to the slave flash buffer
 *
 *
 */
void crossSegmentHandler(uint32 address,uint8* data,uint8 length,uint8 dataStartPoint,FlashBuffer *fb)
{
	uint8 shift;
	uint32 newAddress ;

	shift = 64 - (fb->offset) ; //Calculate how many available bytes left in the flash buffer
	dataToBuffer(address,data,shift,dataStartPoint,fb); //Only copy an amount of data to fit into the flash buffer first
	while(!flashBufferFlush(fb)); //Flush the buffer until success

	fb->offset = 0 ; //Reset flash buffer offset to 0
	fb->segment ++ ; //Increment the segment
	newAddress = fb->segment * 64 ; // Store the new address since segment has incremented


	while(!flashBufferRead(fb)); // Read the segment from the slave
	dataToBuffer(newAddress,data,length-shift,dataStartPoint+shift,fb); // Copy remaining data to the flash buffer
																		// dataCount - shift ->some of the data was copied previously
																		// dataStartPoint + shift -> prevent copying copied data

}



/**
 * Handle cases when different segment of holding flash buffer and segment to write occurs
 *
* Input : address			is the starting address of the data going to be written
 *		  data				contain the pointer to data array going to be written
 *		  length			is the amount of data in the data array
 *		  dataStartPoint	is the starting location of data going to be accessed
 *		  fb				contain the segment,offset and pointer to the slave flash buffer
 *
 *
 */
void differentSegmentHandler(uint32 address,uint8* data,uint8 length,uint8 dataStartPoint,FlashBuffer *fb)
{

		while(!flashBufferFlush(fb));
		while(!flashBufferRead(fb));
		fb->segment = address/64 ;
		fb->offset = 0 ;
		dataToBuffer(address,data,length,dataStartPoint,fb);

}


/**
 * Handle cases when the data is going to be written into configuration register
 *
* Input : address			is the starting address of the data going to be written (configuration register)
 *		  data				contain the pointer to data array going to be written
 *		  length			is the amount of data in the data array
 *		  fb				contain the segment,offset and pointer to the slave flash buffer
 *
 *
 */
void configurationDataHandler(uint32 address,uint8* data,FlashBuffer *fb)
{
	while(!flashBufferFlush(fb));//Flush the previous holding buffer
	spiSendConfig(address,data);// Different method of sending data and programming required
}





/**
 * Check if the address and the amount of data required to write is in the same segment
 *
 * Input :	address		is the starting address of the data going to be written
 *		  	length		is the amount of data to be written
 *			fb			contains the segment to be compared with
 *
 * Output : 1			for address in range
 *			0			for address not in range
 */
uint8 isAddressInRange(uint32 address,uint8 length,FlashBuffer *fb)
{
	if ((address+length)/64 == fb->segment)
		return 1;


	return 0;
}

/**
 * Check if the address desired to write is in the same segment
 *
 * Input :	address		is the starting address of the data going to be written
 *			fb			contains the segment to be compared with
 *
 * Output : 1			for same segment
 *			0			for different segment
 */
uint8 isSameSegment(uint32 address,FlashBuffer *fb)
{
	if (address/64 == fb->segment)
		return 1 ;

	return 0 ;
}

/**
 * Check if the data going to be written is configuration data
 *
 * Input :	address		is the starting address of the data going to be written
 *
 * Output : 1			for configuration data
 *			0			for not configuration data
 */
uint8 isConfigData(uint32 address)
{
	if (address >= 0x300000)
		return 1 ;

	return 0 ;
}

