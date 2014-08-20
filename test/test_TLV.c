#include "unity.h"
#include "TLV.h"
#include "IntelHex16Operation.h"
#include "SerialInterrupt.h"
#include "mock_UART.h"
#include "p18f4520.h"
#include "CustomTypeAssertion.h"
#include "DelayedWrite.h"
#include "mock_FlashBuffer.h"
#include "mock_spiMaster.h"
#include <stdio.h>

extern TLV_Buffer tlvBuf;
extern uint8 tlvBuffer[160];

void setUp(void)
{
	initTlvBuffer(&tlvBuf);
}

void tearDown(void)
{
}

void test_writeProgram_given_frame_2_should_write_the_program_from_frame_2(void)
{
	tlvBuf.readyFrame = 1;

	uint8 buffer[64];
	uint8 frame[] = {0x01, 0x07, 0x40, 0x00, 0x00, 0x00, 0x02, 0xe7, 0xff};
	uint8 *TLVpointer, i;

	TLVpointer = tlvBuf.bufferPointers[1];

	for(i = 0; i < 9; i++)
	{
		TLVpointer[i] = frame[i];
	}

	FlashBuffer fb;
	fb.offset = 0;
	fb.segment = 0;
	fb.buffer = buffer;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb, 1);
	flashBufferRead_ExpectAndReturn(&fb, 1);

	writeProgram(&fb, &tlvBuf);
	TEST_ASSERT_EQUAL(3, tlvBuf.readyFrame);
}

void test_writeProgram_given_frame_2_should_write_the_program_from_frame_1(void)
{
	tlvBuf.readyFrame = 2;

	uint8 buffer[64];
	uint8 frame[] = {0x01, 0x07, 0x40, 0x00, 0x00, 0x00, 0x02, 0xe7, 0xff};
	uint8 *TLVpointer, i;

	TLVpointer = tlvBuf.bufferPointers[0];

	for(i = 0; i < 9; i++)
	{
		TLVpointer[i] = frame[i];
	}

	// printf("&tlv.bufferPointers[0]: %d\n", tlvBuf.bufferPointers[1]);
	// printf("&tlvBuffer[80]: %d\n", &tlvBuffer[80]);
	// printf("ptrTLV: %d\n", ptrTLV);

	FlashBuffer fb;
	fb.offset = 0;
	fb.segment = 0;
	fb.buffer = buffer;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb, 1);
	flashBufferRead_ExpectAndReturn(&fb, 1);

	writeProgram(&fb, &tlvBuf);

	TEST_ASSERT_EQUAL(3, tlvBuf.readyFrame);
}

void test_writeProgram_given_frame_1_and_2_should_write_the_program_from_frame_1_and_2(void)
{
	tlvBuf.readyFrame = 0;

	uint8 buffer[64];
	uint8 frame1[] = {0x01, 0x07, 0x40, 0x00, 0x00, 0x00, 0x02, 0xe7, 0xff};
	uint8 frame2[] = {0x01, 0x0b, 0x80, 0x00, 0x00, 0x00, 0x02, 0xe7, 0x45, 0x18, 0x75, 0x27, 0xff};
	uint8 *TLVpointer, i;

	TLVpointer = tlvBuf.bufferPointers[0];

	for(i = 0; i < 9; i++)
	{
		TLVpointer[i] = frame1[i];
	}

	TLVpointer = tlvBuf.bufferPointers[1];

	for(i = 0; i < 13; i++)
	{
		TLVpointer[i] = frame2[i];
	}

	// printf("&tlv.bufferPointers[0]: %d\n", tlvBuf.bufferPointers[1]);
	// printf("&tlvBuffer[80]: %d\n", &tlvBuffer[80]);
	// printf("ptrTLV: %d\n", ptrTLV);

	FlashBuffer fb;
	fb.offset = 0;
	fb.segment = 0;
	fb.buffer = buffer;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb, 1);
	flashBufferRead_ExpectAndReturn(&fb, 1);

	writeProgram(&fb, &tlvBuf);

	TEST_ASSERT_EQUAL(1, tlvBuf.readyFrame);

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb, 1);
	flashBufferRead_ExpectAndReturn(&fb, 1);

	writeProgram(&fb, &tlvBuf);

	TEST_ASSERT_EQUAL(3, tlvBuf.readyFrame);
}

void test_writeProgram_given_frame_1_as_configure_data_should_write_the_program_from_frame_1(void)
{
	tlvBuf.readyFrame = 2;

	uint8 buffer[64];
	uint8 frame[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xff};
	uint8 *TLVpointer, i;

	TLVpointer = tlvBuf.bufferPointers[0];

	for(i = 0; i < 9; i++)
	{
		TLVpointer[i] = frame[i];
	}

	// printf("&tlv.bufferPointers[0]: %d\n", tlvBuf.bufferPointers[1]);
	// printf("&tlvBuffer[80]: %d\n", &tlvBuffer[80]);
	// printf("ptrTLV: %d\n", ptrTLV);

	FlashBuffer fb;
	fb.offset = 0;
	fb.segment = 0;
	fb.buffer = buffer;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb, 1);
	spiSendConfig_ExpectAndReturn(0x00300001, &TLVpointer[6], 1);

	writeProgram(&fb, &tlvBuf);

	TEST_ASSERT_EQUAL(3, tlvBuf.readyFrame);
}