#ifndef SerialInterrupt_H
#define SerialInterrupt_H
#include "UART.h"
#include "Utils.h"

#define NACK 0x05
#define ACK 0x04

#define NOT_AVAILABLE 0x00
#define BUFFER0_AVAILABLE 0x01
#define BUFFER1_AVAILABLE 0x10
#define BOTH_AVAILABLE 0x11

typedef enum {WAIT_FOR_TYPE,
			  WAIT_FOR_LENGTH,
			  WAIT_FOR_VALUE} State;

typedef struct
{
	State state;
	uint32 i;
	uint32 length;	
} TLV_FSM;

//void interruptMain(void);
void chk_SerialISR(void);
void SerialISR(void);
void tlvReceiveFSM(TLV_FSM *fsm);

#endif // SerialInterrupt_H
