#include "TLV.h"
#include "SerialInterrupt.h"
#include "IntelHex16Operation.h"
#include "18c.h"
#include "UART.h"
#include "Utils.h"
#include "DelayedWrite.h"
#include "FlashBuffer.h"
#include "spiMaster.h"
#include <stdio.h>

extern TLV_Buffer tlv;
extern uint8 tlvBuffer[160];

void writeProgram(FlashBuffer *fb, TLV_Buffer *tlv, uint8 **ptrPtrTLV)
{
	if(isReadyFrameAvailable(tlv))
	{
		*ptrPtrTLV = getReadyTLVframe(tlv);
		decodeCommand(fb, *ptrPtrTLV);
		releaseTLVframe(tlv, *ptrPtrTLV);
	}
}
