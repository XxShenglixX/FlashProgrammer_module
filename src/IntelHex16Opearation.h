#ifndef IntelHex16Opearation_H
#define IntelHex16Opearation_H
#include <stdio.h>
#include <stdlib.h>

typedef enum {ERR_NO_ERROR, ERR_NO_COLON, ERR_WRONG_CHECKSUM} ErrorLine;
typedef enum {NOTHING, PROGRAM_MSG, START_RUNNING, TARGET_AVAILABLITY, ACK, NACK, STOP} TLV_Message;

typedef struct
{
	char type;
	char length;
	char value[0];
} TLV;

typedef struct
{
	char *line;
	int index;
} IntelHex16Data;

//TLV *createMessageFromIntelHex16(char *line);
void *readLine(FILE *hexFile, char *buffer);
IntelHex16Data *createIntelHex16Data(char *line);
IntelHex16Data *deleteIntelHex16Data(IntelHex16Data *data);
int getByteFromIntelHex16Data(IntelHex16Data *data, int index);
int getLengthFromIntelHex16Data(IntelHex16Data *data);
int getTypeFromIntelHex16Data(IntelHex16Data *data);
int getAddressFromIntelHex16Data(IntelHex16Data *data);
int verifyCheckSumOfIntelHex16Data(IntelHex16Data *data);

int getLSByteAndShiftRight(int long *address);

TLV *createProgramMessage(IntelHex16Data *data, int addressHigh);
TLV *createStartRunningMessage();
TLV *createInquireTargetAvailability();
TLV *createProgrammingMode();
TLV *deleteTLV(TLV *tlv);

void sendTLV(TLV *tlv);

// void tlvProtocol();

#endif // IntelHex16Opearation_H
