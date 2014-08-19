#ifndef SerialInterrupt_H
#define SerialInterrupt_H
#include "UART.h"
#include "Utils.h"
#include "FlashBuffer.h"

#define stopInterrupt INTCONbits.GIE

typedef enum {WAIT_FOR_TYPE,
			  WAIT_FOR_LENGTH,
			  WAIT_FOR_VALUE} State;

typedef struct
{
	State state;
	uint32 i;
	uint32 length;	
} TLV_FSM;

typedef struct
{
	uint8 *bufferPointers[2];
	uint32 readyFrame;
} TLV_Buffer;

//void interruptMain(void);
void chk_SerialISR(void);
void SerialISR(void);
void tlvReceiveFSM(TLV_FSM *fsm, TLV_Buffer *tlv, uint8 *ptr);
uint8 *getReadyTLVframe(TLV_Buffer *tlv);
void setTLVframe(TLV_Buffer *tlv, uint8 *ptrTLV);
uint8 *getNonReadyTLVframe(TLV_Buffer *tlv);

uint32 getAddress(uint8 *ptrTLV);
uint8 *getData(uint8 *ptrTLV);
uint8 getLength(uint8 *ptrTLV);

void decodeCommand(FlashBuffer *fb, uint8* ptrTLV);
uint8 isReadyFrameAvailable(TLV_Buffer *tlv);
// void tlvDecode(uint32 *address, uint8 *length, uint8 *data, uint8 *dataOnly);

#endif // SerialInterrupt_H
