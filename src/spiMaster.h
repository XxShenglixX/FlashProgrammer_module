#ifndef spiMaster_H
#define spiMaster_H
#include "Utils.h"
#include "FlashBuffer.h"
void spiConfigureMaster(void) ;
void spiSendByte(uint8 data);

uint8 spiSendData(uint8 *data,uint8 count,uint8 startPoint);
uint8 spiSendCommand(uint8 command);
uint8 spiSendAddress(FlashBuffer *fb);

uint8 spiReceiveData(uint8 *data,uint8 count);
uint8 spiReceiveStatus(void);

uint8 readID(void);
uint8 checkACK(uint8 *data);

#endif //