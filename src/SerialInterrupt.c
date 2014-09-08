#include "SerialInterrupt.h"
#include "18c.h"
#include "UART.h"
#include "Utils.h"
#include "DelayedWrite.h"
#include "FlashBuffer.h"
#include "masterMain.h"
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
    uint8 *ptr;

    ptr = getNonReadyTLVframe(&tlvBuf);

    if(ptr == 0)
        uartSendByte(NACK);
    else
        tlvReceiveFSM(&fsm, &tlvBuf, ptr);
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
			if(ptr[fsm->i] == PROGRAMMING_MODE)
			{
				setProgrammingMode();
				fsm->state = WAIT_FOR_TYPE;
			}
			if(ptr[fsm->i] == START_RUNNING)
			{
				setStartRunningMode();
				fsm->state = WAIT_FOR_TYPE;
			}
            fsm->state = WAIT_FOR_LENGTH;
            break;
        case WAIT_FOR_LENGTH:
            ptr[fsm->i] = uartGetByte();
            fsm->length = ptr[fsm->i];
            fsm->state = WAIT_FOR_VALUE;
            break;
        case WAIT_FOR_VALUE:
            if(fsm->i < fsm->length + 1)
            {
                ptr[fsm->i] = uartGetByte();
                fsm->state = WAIT_FOR_VALUE;
            }
            else
            {
                ptr[fsm->i] = uartGetByte();
                setTLVframe(tlvBuf, ptr);
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
	uint8 *data, i;

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
	bufferHandler(getAddress(ptrTLV), getData(ptrTLV), getLength(ptrTLV), fb);
}

uint8 isAnyFrameReady(TLV_Buffer *tlvBuf)
{
	if(isFrame0Ready(tlvBuf->readyFrame) || isFrame1Ready(tlvBuf->readyFrame))
		return 1;

	return 0;
}

void setProgrammingMode(void)
{
	stopTarget();
	resetTarget = 0;
	uartSendByte(ACK);
}

void setStartRunningMode(void)
{
	runTarget();
	resetTarget = 0;
	stopInterrupt = 0;
}