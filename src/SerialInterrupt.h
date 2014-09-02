#ifndef  SerialInterrupt_H
#define  SerialInterrupt_H

#include "UART.h"
#include "Utils.h"
#include "FlashBuffer.h"

#define  stopInterrupt INTCONbits.GIE
#define  FRAME0_NOT_READY 1
#define  FRAME1_NOT_READY 2

typedef enum {  WAIT_FOR_TYPE,
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
void initTlvBuffer(TLV_Buffer *tlvBuf);
void tlvReceiveFSM(TLV_FSM *fsm, TLV_Buffer *tlv, uint8 *ptr);
uint8 *getReadyTLVframe(TLV_Buffer *tlv);
void setTLVframe(TLV_Buffer *tlv, uint8 *ptrTLV);
uint8 *getNonReadyTLVframe(TLV_Buffer *tlv);
void releaseTLVframe(TLV_Buffer *tlvBuf, uint8 *ptrTLV);
uint8 isFrame0Ready(uint8 readyFlag);
uint8 isFrame1Ready(uint8 readyFlag);

uint32 getAddress(uint8 *ptrTLV);
uint8 *getData(uint8 *ptrTLV);
uint8 getLength(uint8 *ptrTLV);

void decodeCommand(FlashBuffer *fb, uint8* ptrTLV);
uint8 isAnyFrameReady(TLV_Buffer *tlvBuf);

#endif // SerialInterrupt_H
