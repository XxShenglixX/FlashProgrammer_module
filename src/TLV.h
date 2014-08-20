#ifndef TLV_H
#define TLV_H

#include "SerialInterrupt.h"
#include "FlashBuffer.h"

void writeProgram(FlashBuffer *fb, TLV_Buffer *tlv);

#endif // TLV_H
