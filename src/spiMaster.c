#include "p18f4520.h"
#include "spi.h"
#include "delays.h"

#include "spiMaster.h"
#include "FlashBuffer.h"
#include "Utils.h"
#define ACK 	0xA5
#define Write	0x11
#define Read	0x22
#define CONFIG 	0x33
#define DevID	0x44
#define Done 	0xFF

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

/**
 * Send a byte of data through spi and delay for 20 instruction cycles
 * just in case the slave is not quick enough to react
 *
 * Input : data is the data going to be sent out
 *
 */
void spiSendByte(uint8 data)
{
	WriteSPI(data) ;
	Delay10TCYx(2);
}

/**
 * Send any amount of data through spi and always check for the slave ACK
 * Delay extra 10 instruction cycles at the end of sending.
 *
 * Input :	*data		is the pointer to the data array
 *		  	count		is the amount of data in the array
 *		 	startPoint 	is the starting location of the data array to access
 *
 */
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


/**
 * Send command to the slave and delay extra 10 instruction cycles at the end
 *
 * Examples of command
 * Write 	for sending data to the slave and the slave will perform flash erase and write
 * Read 	for reading the desired segment from the slave
 * ID 		for reading the device ID of the slave
 * CONFIG 	for sending configuration data to the slave
 *
 * Input 	: command	is the instruction for the slave
 *
 * Output	: 1 for OK
 *			  0 for failure as the master did not receive ACK from the slave
 *
 */
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

/**
 * Send starting address of a segment to the slave
 *
 * Input : fb	contains the segment number
 *
 * Ouput : 1 for OK
 *	       0 for failure as the master did not receive ACK from the slave
 */
uint8 spiSendAddressSegment(FlashBuffer *fb)
{
	uint8 addressArray[3] ;

	uint32 address = fb->segment * 0x40;

	addressArray[0] = address ;
	addressArray[1] = address >> 8 ;
	addressArray[2] = address >> 16 ;

	if(!spiSendData(addressArray,3,0))
		return 0 ;
	else
		return 1 ;
}


/**
 * Send specific address to the slave
 * Note : For sending configuration words and reading device ID purpose only
 *
 * Input : address 	is the specific address to be sent to the slave
 *
 * Ouput : 1 for OK
 *		   0 for failure as the master did not receive ACK from the slave
 */
uint8 spiSendAddressOnly(uint32 address)
{
	uint8 addressArray[3] ;
	addressArray[0] = address ;
	addressArray[1] = address >> 8 ;
	addressArray[2] = address >> 16 ;

	if(!spiSendData(addressArray,3,0))
		return 0 ;
	else
		return 1 ;

}

/**
 * Send configuration data to the slave
 *
 *
 * Input : 	address	is the address of the configuration register
 *			data	is the data going to be written into the configuration register
 *
 * Ouput : 1 for OK
 *		   0 for failure as the master did not receive ACK from the slave
 */
uint8 spiSendConfig(uint32 address,uint8 *data)
{

	if(!spiSendCommand(CONFIG))
		return 0;
	if(!spiSendAddressOnly(address))
		return 0;
	if(!spiSendData(data,1,0))
		return 0;

	return 1 ;
}


/**
 * Receive data from the slave through spi
 *
 * Input : data is the data going to be stored
 *		   count is the amount of data going to be stored
 *
 * Ouput : 1 for OK
 */
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


/**
 * Receive status of flash writing operation by the slave
 *
 *
 * Ouput : 1 for done
 *		   0 for not done
 */
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


/**
 * Read device ID from the slave
 *
 * Ouput :	1 for target available and match
 *			0 for unknown target / target unavailable
 */
uint8 readID(void)
{
	uint8 ID;
	spiSendCommand(DevID);
	spiReceiveData(&ID,1);

	if (ID != 0x12)
		return 0 ;

	return 1;
}
/**
 * Check ACK from the input
 *
 *
 * Input : data contain the reply read from the Slave
 *
 *
 * Ouput : 1 for ACK
 *		   0 for NACK or unknown reply
 */
uint8 checkACK(uint8 *data)
{
	if (*data == ACK)
		return 1 ;
	else
		return 0 ;
}


