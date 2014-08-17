#ifndef FlashProgrammer_module_H
#define FlashProgrammer_module_H
#include "Utils.h"

void programMain(void);
void tlvDecode(uint32 *address, uint8 *length, uint8 *data, uint8 *dataOnly);

#endif // FlashProgrammer_module_H
