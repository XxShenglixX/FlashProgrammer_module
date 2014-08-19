#ifndef DelayedWrite_H
#define DelayedWrite_H
#include "Utils.h"
#include "FlashBuffer.h"

void bufferHandler(uint32 address,uint8* data,uint8 length,FlashBuffer *fb);
void dataToBuffer(uint32 address,uint8* data,uint8 length,uint8 dataStartPoint,FlashBuffer *fb);

void crossSegmentHandler(uint32 address,uint8* data,uint8 length,uint8 dataStartPoint,FlashBuffer *fb);
void differentSegmentHandler(uint32 address,uint8* data,uint8 length,uint8 dataStartPoint,FlashBuffer *fb);
void configurationDataHandler(uint32 address,uint8* data,FlashBuffer *fb);

uint8 isAddressInRange(uint32 address,uint8 length,FlashBuffer *fb);
uint8 isSameSegment(uint32 address,FlashBuffer *fb);
uint8 isConfigData(uint32 address);

#endif // DelayedWrite_H
