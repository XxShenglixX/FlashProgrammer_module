#include "FlashProgrammer_module.h"
#include "p18f4520.h"
#include "UART.h"
#include "SerialInterrupt.h"
#include "Utils.h"
#include <stdio.h>

#define Baudrate 25

extern uint8 tlvBuffer[160];
extern uint8 tlvFrameReady;

void programMain(void)
{
	uint32 address;
	uint8 length;
	uint8 *data;

	uartSetup(Baudrate);
	INTCONbits.GIE = 1;
	INTCONbits.PEIE = 1;

	//tlvDecode(&address, &length, data);

	// while(1);
}

void tlvDecode(uint32 *address, uint8 *length, uint8 *data, uint8 *dataOnly)
{
	uint8 i;

	// if(tlvFrameReady == 0x10)
	// {
		*address = (uint32)(data[5]) << 24 | (uint32)(data[4]) << 16 | (uint32)(data[3]) << 8 | (uint32)(data[2]);
		*length = data[1] - 5;

		for(i = 0; i < *length; i+=2)
		{
			if(*length == 1)
				dataOnly[i] = data[i+6];
			else
			{
				dataOnly[i] = data[i+6];
				dataOnly[i+1] = data[i+7];
			}
			// temp = data[i+1];
			// data[i+1] = data[i];
			// data[i] = temp;
		}
		
		if(*length == 1)
			dataOnly[i-1] = 0;
		else
			dataOnly[i] = 0;
	// }
	/* else if(tlvFrameReady == 0x01)
	{
		*address = (uint32)(data[85]) << 24 | (uint32)(data[84]) << 16 | (uint32)(data[83]) << 8 | (uint32)(data[82]);
		*length = tlvbuffer[81];

		for(i = 86; i < (*length-5) + 6; i+=2)
		{
			dataOnly[i-86] = data[i+1];
			dataOnly[i-85] = data[i];
		}
	} */
}