#ifndef SerialInterrupt_H
#define SerialInterrupt_H
#include "UART.h"
#include "Utils.h"

//void interruptMain(void);
void chk_SerialISR(void);
void SerialISR(void);
void tlvReceiveFSM(TLV_FSM *fsm);

#endif // SerialInterrupt_H
