#ifndef TLV_H
#define TLV_H

#include "SerialInterrupt.h"
#include "FlashBuffer.h"

void writeProgram(FlashBuffer *fb, TLV_Buffer *tlv, uint8 **ptrPtrTLV);

#endif // TLV_H
