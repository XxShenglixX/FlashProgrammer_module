#include "SerialInterrupt.h"
#include "p18f4520.h"
#include "UART.h"
#include "Utils.h"

#pragma config OSC = INTIO67, WDT = OFF, DEBUG = ON, LVP = OFF

uint8 tlvBuffer[160];
uint8 *bufferPointers[] = {&tlvBuffer[0], &tlvBuffer[80]};

uint8 byteReceive;
uint8 tlvFrameReady = 0x11;
uint8 bufferIndex;

#pragma interrupt chk_SerialISR
void chk_SerialISR(void)
{
	if(PIR1bits.RCIF == 1)
		SerialISR();
}

#pragma code My_HiVect_Int = 0x008
void My_HiVect_Int(void)
{
	#ifdef __GNUC__
	#else 
	_asm GOTO chk_SerialISR _endasm
	#endif
}
#pragma code

//void interruptMain(void)
//{
	//uartSetup(Baudrate);
	//INTCONbits.GIE = 1;
	//INTCONbits.PEIE = 1;

	//while(1);
//}

void SerialISR(void)
{
	static TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
	byteReceive = uartGetByte();
	if(tlvFrameReady == BUFFER0_AVAILABLE || tlvFrameReady == BOTH_AVAILABLE)
		bufferIndex = 0;
	else if(tlvFrameReady == BUFFER1_AVAILABLE)
		bufferIndex = 1;
	tlvReceiveFSM(&fsm);
}

void tlvReceiveFSM(TLV_FSM *fsm)
{
	switch(fsm->state)
	{
		case WAIT_FOR_TYPE:
			bufferPointers[bufferIndex][fsm->i] = byteReceive;
			uartSendByte(bufferPointers[bufferIndex][fsm->i]);
			fsm->state = WAIT_FOR_LENGTH;
			break;
		case WAIT_FOR_LENGTH:
			bufferPointers[bufferIndex][fsm->i] = byteReceive;
			uartSendByte(bufferPointers[bufferIndex][fsm->i]);
			fsm->length = byteReceive;
			fsm->state = WAIT_FOR_VALUE;
			break;
		case WAIT_FOR_VALUE:
			if(fsm->i < fsm->length + 1)
			{
				bufferPointers[bufferIndex][fsm->i] = byteReceive;
				uartSendByte(bufferPointers[bufferIndex][fsm->i]);
				fsm->state = WAIT_FOR_VALUE;
			}
			else
			{
				if(tlvFrameReady == 0x01 || tlvFrameReady == 0x11)
					tlvFrameReady = BUFFER1_AVAILABLE;
				else if(tlvFrameReady = 0x10)
					tlvFrameReady = BUFFER0_AVAILABLE;
				bufferPointers[bufferIndex][fsm->i] = byteReceive;
				uartSendByte(bufferPointers[bufferIndex][fsm->i]);
				fsm->state = WAIT_FOR_TYPE;
			}
			break;
		default:
			break;
	}

	if(fsm->state == WAIT_FOR_TYPE)
		fsm->i = 0;
	else
		fsm->i++;
}