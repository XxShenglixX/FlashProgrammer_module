#ifndef DelayedWrite_H
#define DelayedWrite_H
#include "Utils.h"
#include "FlashBuffer.h"

void bufferHandler(uint32 address,uint8* data,uint8 dataCount,FlashBuffer *fb);
void dataToBuffer(uint32 address,uint8* data,uint8 dataCount,uint8 dataStartPoint,FlashBuffer *fb);

#endif // DelayedWrite_H
