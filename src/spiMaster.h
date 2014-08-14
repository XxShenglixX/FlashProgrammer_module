#ifndef spiMaster_H
#define spiMaster_H
#include "Utils.h"
#include "FlashBuffer.h"

void spiConfigureMaster(void) ;
void spiSendByte(uint8 data);

uint8 spiSendCommand(uint8 command);
uint8 spiSendAddress(FlashBuffer *fb);
uint8 spiSendData(uint8 *data,uint8 count,uint8 startPoint);
uint8 spiCheckACK(uint8 *data);

uint8 spiReceiveData(uint8 *data,uint8 count);
uint8 spiReceiveStatus(void);

uint8 readID(void);
void readSegment(FlashBuffer *fb);

#endif //