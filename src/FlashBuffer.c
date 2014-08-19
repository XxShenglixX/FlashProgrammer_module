#include "Utils.h"
#include "Delays.h"
#include "FlashBuffer.h"
#include "spiMaster.h"

#define Write 0x11
#define Read 0x22

#ifdef __GNUC__
uint8 slaveBuffer[64];
#else
#endif

/**
 * Get last address of the flash buffer based on the segment and offset
 *
 *
 * Input : fb contains the segment address and offset
 *
 * Ouput : return the address calculated
 */
uint32 flashBufferGetLastAddress(FlashBuffer *fb)
{
	if (fb->segment == -1 )
		return -1 ;

	return (fb->offset + (fb->segment * 0x40) ) ;
}

/**
 * Send the completed segment buffer to the slave to be written
 * Will loop until receive done from the slave
 *
 * Input : fb contains the segment address and the pointer to the flash buffer
 *
 * Ouput : 1 for OK
 *		   0 for failure
 */
uint8 flashBufferFlush(FlashBuffer *fb)
{
	if(!spiSendCommand(Write))
		return 0 ;

 	 if(!spiSendAddressSegment(fb))
		return 0 ;

	 if(!spiSendData(fb->buffer,64,0))
		return 0;

	while(!spiReceiveStatus())
	{
		Delay10TCYx(2);
	}


		return 1 ;
}

/**
 * Read the segment from the slave as flash buffer
 *
 * Input : fb contains the segment address and the pointer to the flash buffer
 *
 * Ouput : 1 for OK
 *		   0 for failure
 */
uint8 flashBufferRead(FlashBuffer *fb)
{
	#ifndef __GNUC__
	uint8 slaveBuffer[64];
	#endif
	fb->buffer = slaveBuffer ;


	if(!spiSendCommand(Read))
		return 0 ;

 	 if(!spiSendAddressSegment(fb))
		return 0 ;

	 if(!spiReceiveData(slaveBuffer,64))
		return 0;


	return 1 ;
}


/**
 * Check if FlashBuffer buffer points to nothing
 *
 * Input : fb contains the segment address and the pointer to the flash buffer
 *
 * Ouput : 1 for NULL
 *		   0 for not true
 */
uint8 isFlashBufferNull(FlashBuffer *fb)
{
	if(fb->buffer == 0)
		return 1;

	return 0;
}