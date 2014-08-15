#include "p18f4520.h"
#include "spi.h"
#include "delays.h"

#include "spiMaster.h"
#include "FlashBuffer.h"
#include "Utils.h"
#define ACK 0xA5
#define Write 0x11
#define Read 0x22
#define Done 0xFF


uint8 tlvFrameReady = 0x11;
/**
 * Configure SPI in PIC18 as master
 *
 * clock = FOSC_64 
 * Mode (0,1)
 * Input data sampled at middle of data out
 */
void spiConfigureMaster(void)
{
	CloseSPI();
	OpenSPI(SPI_FOSC_64,MODE_01,SMPMID);
}

void spiSendByte(uint8 data)
{
	WriteSPI(data) ;
	Delay10TCYx(2);
}

uint8 spiSendData(uint8 *data,uint8 count,uint8 startPoint)
{
	uint8 acknack , i , j = startPoint;
	
	spiConfigureMaster();

	for ( i = 0 ; i < count  ; i ++ )
	{
		spiSendByte(data[j]);
		acknack = ReadSPI();
		j++;
	if(!checkACK(&acknack))
			return 0 ; //return 0 for error
	}

	CloseSPI();
	Delay10TCYx(1);
	return 1 ; //return 1 for OK
}


uint8 spiSendCommand(uint8 command)
{
	uint8 acknack,dummy;
	spiConfigureMaster();

	spiSendByte(command);
	acknack = ReadSPI();
	
	if(!checkACK(&acknack))
		return 0 ; //return 0 for error

	CloseSPI();
	Delay10TCYx(1);
	return 1 ;
}

uint8 spiSendAddress(FlashBuffer *fb)
{
	uint8 addressArray[3] ;	
	
	uint32 address = fb->segment * 0x40 ;

	addressArray[0] = address ;
	addressArray[1] = address >> 8 ;
	addressArray[2] = address >> 16 ;	

	if(!spiSendData(addressArray,3,0))
		return 0 ;
	else
		return 1 ;
}


uint8 spiReceiveData(uint8 *data,uint8 count)
{
	uint8 i ;
	spiConfigureMaster();

	for ( i = 0 ; i < count  ; i ++ )
	{
		spiSendByte(0x00);
		data[i] = ReadSPI();
	}

	CloseSPI();
	Delay10TCYx(1);
	return 1 ; //return 1 for OK
}



uint8 spiReceiveStatus(void)
{
	uint8 status ;

	spiConfigureMaster();

	spiSendByte(0x00);
	status = ReadSPI();
	
	CloseSPI();
	Delay10TCYx(1);

	if (status == Done)
		return 1 ;
	else 
		return 0 ;
}

uint8 readID(void)
{
	uint8 ID;
	
	spiReceiveData(&ID,1);
	if (ID != 0x12)
		return 0 ;

	return 1;
}

uint8 checkACK(uint8 *data)
{
	if (*data == ACK)
		return 1 ;
	else
		return 0 ;
}

