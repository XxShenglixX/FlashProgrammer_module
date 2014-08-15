#ifndef FlashBuffer_H
#define FlashBuffer_H
#include "Utils.h"

typedef struct 
{
	uint8 *buffer ;
	uint8 offset ;
	int16 segment ;
}FlashBuffer ;

uint32 flashBufferGetLastAddress(FlashBuffer *fb);
uint8 flashBufferFlush(FlashBuffer *fb);
#endif // FlashBuffer_H
