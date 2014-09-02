#include "SerialInterrupt.h"
#include "IntelHex16Operation.h"
#include "18c.h"
#include "UART.h"
#include "Utils.h"
#include "DelayedWrite.h"
#include "FlashBuffer.h"
#include <stdio.h>

uint8 tlvBuffer[160];

uint8 byteReceive;
uint8 bufferIndex;

TLV_Buffer tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}};

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

void SerialISR(void)
{
    static TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
	// TLV tlvBuffer = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
    uint8 *ptr;

    ptr = getNonReadyTLVframe(&tlvBuf);
	//byteReceive = uartGetByte();

    if(ptr == 0)
        uartSendByte(NACK);
    else
        tlvReceiveFSM(&fsm, &tlvBuf, ptr);

	/*if(tlvFrameReady == BUFFER0_AVAILABLE || tlvFrameReady == BOTH_AVAILABLE)
	{
		bufferIndex = 0;
		//uartSendByte(ACK);
		tlvReceiveFSM(&fsm);
	}
	else if(tlvFrameReady == BUFFER1_AVAILABLE)
	{
		bufferIndex = 1;
		//uartSendByte(ACK);
		tlvReceiveFSM(&fsm);
	}
	else
		uartSendByte(NACK);*/
}

void initTlvBuffer(TLV_Buffer *tlvBuf)
{
    tlvBuf->bufferPointers[0] = &tlvBuffer[0];
    tlvBuf->bufferPointers[1] = &tlvBuffer[80];
    tlvBuf->readyFrame = 3;
}

void tlvReceiveFSM(TLV_FSM *fsm, TLV_Buffer *tlvBuf, uint8 *ptr)
{
    switch(fsm->state)
    {
        case WAIT_FOR_TYPE:
            ptr[fsm->i] = uartGetByte();
            //bufferPointers[bufferIndex][fsm->i] = uartGetByte();
            //uartSendByte(bufferPointers[bufferIndex][fsm->i]);
            //uartSendByte(ACK);
            if(ptr[fsm->i] == 6)
                stopInterrupt = 0;
            fsm->state = WAIT_FOR_LENGTH;
            //printf("bufferPointers[bufferIndex][fsm->i]: %d\n", bufferPointers[bufferIndex][fsm->i]);
            break;
        case WAIT_FOR_LENGTH:
            ptr[fsm->i] = uartGetByte();
            //bufferPointers[bufferIndex][fsm->i] = uartGetByte();
            //uartSendByte(bufferPointers[bufferIndex][fsm->i]);
            //uartSendByte(ACK);
            fsm->length = ptr[fsm->i];
            fsm->state = WAIT_FOR_VALUE;
            //printf("bufferPointers[bufferIndex][fsm->i]: %d\n", bufferPointers[bufferIndex][fsm->i]);
            break;
        case WAIT_FOR_VALUE:
            if(fsm->i < fsm->length + 1)
            {
                ptr[fsm->i] = uartGetByte();
                //bufferPointers[bufferIndex][fsm->i] = uartGetByte();
                //uartSendByte(bufferPointers[bufferIndex][fsm->i]);
                //uartSendByte(ACK);
                fsm->state = WAIT_FOR_VALUE;
                //printf("bufferPointers[bufferIndex][fsm->i]: %d\n", bufferPointers[bufferIndex][fsm->i]);
            }
            else
            {
                ptr[fsm->i] = uartGetByte();
                //bufferPointers[bufferIndex][fsm->i] = uartGetByte();
                setTLVframe(tlvBuf, ptr);
                //if(bufferIndex == 0)
                    //tlvFrameReady = tlvFrameReady & 0xf0;
                //else
                    //tlvFrameReady = tlvFrameReady & 0x0f;
                //uartSendByte(bufferPointers[bufferIndex][fsm->i]);
                //uartSendByte(ACK);
                fsm->state = WAIT_FOR_TYPE;
                //printf("bufferPointers[bufferIndex][fsm->i]: %d\n", bufferPointers[bufferIndex][fsm->i]);
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

uint8 *getNonReadyTLVframe(TLV_Buffer *tlvBuf)
{
    if(!isFrame0Ready(tlvBuf->readyFrame))
        return tlvBuf->bufferPointers[0];
    else if(!isFrame1Ready(tlvBuf->readyFrame))
        return tlvBuf->bufferPointers[1];
    else
        return NULL;
}

uint8 isFrame0Ready(uint8 readyFlag)
{
    if((readyFlag & 0x1) == 0)
        return 1;

    return 0;
}

uint8 isFrame1Ready(uint8 readyFlag)
{
	if((readyFlag & 0x2) == 0)
		return 1;

	return 0;
}

void setTLVframe(TLV_Buffer *tlvBuf, uint8 *ptrTLV)
{

	if(ptrTLV == &tlvBuffer[0])
	{
		tlvBuf->readyFrame = tlvBuf->readyFrame & 0x2;
	}
	else
	{
		tlvBuf->readyFrame = tlvBuf->readyFrame & 0x1;
	}
}

uint8 *getReadyTLVframe(TLV_Buffer *tlvBuf)
{
	if(isFrame0Ready(tlvBuf->readyFrame))
		return tlvBuf->bufferPointers[0];
	else if(isFrame1Ready(tlvBuf->readyFrame))
		return tlvBuf->bufferPointers[1];
	else
		return NULL;
}

void releaseTLVframe(TLV_Buffer *tlvBuf, uint8 *ptrTLV)
{
	if(ptrTLV == &tlvBuffer[0])
	{
		tlvBuf->readyFrame = tlvBuf->readyFrame | 0x1;
	}
	else
	{
		tlvBuf->readyFrame = tlvBuf->readyFrame | 0x2;
	}
}

uint32 getAddress(uint8 *ptrTLV)
{
	uint32 address;

	address = (uint32)(ptrTLV[5]) << 24 | (uint32)(ptrTLV[4]) << 16 | (uint32)(ptrTLV[3]) << 8 | (uint32)(ptrTLV[2]);

	return address;
}

uint8 getLength(uint8 *ptrTLV)
{
	uint8 length;

	length = ptrTLV[1] - 5;

	return length;
}

uint8 *getData(uint8 *ptrTLV)
{
	uint8 dataSize, *data, i;

	dataSize = getLength(ptrTLV);

	if(((TLV *)ptrTLV)->type == PROGRAM_MSG)
	{
		data = &ptrTLV[6];
		return data;
	}
	else
		return NULL;
}

void decodeCommand(FlashBuffer *fb, uint8 *ptrTLV)
{
	// printf("ptrTLV: %d\n", ptrTLV[0]);
	bufferHandler(getAddress(ptrTLV), getData(ptrTLV), getLength(ptrTLV), fb);
}

uint8 isAnyFrameReady(TLV_Buffer *tlvBuf)
{
	if(isFrame0Ready(tlvBuf->readyFrame) || isFrame1Ready(tlvBuf->readyFrame))
		return 1;

	return 0;
}