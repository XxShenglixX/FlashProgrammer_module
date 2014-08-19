#include "unity.h"
#include "IntelHex16Operation.h"
#include "SerialInterrupt.h"
#include "mock_UART.h"
#include "p18f4520.h"
#include "CustomTypeAssertion.h"
#include "mock_DelayedWrite.h"
#include "mock_FlashBuffer.h"
#include <stdio.h>

extern uint8 tlvBuffer[160];
extern uint8 tlvFrame;
// extern uint8 tlvFrameReady;
// extern uint8 *bufferPointers[];

extern TLV_Buffer tlvBuf;

void setUp(void)
{
	initTlvBuffer(&tlvBuf);
}

void tearDown(void)
{
}

void setupUartGetByte(uint8 frame[],int size)
{
	uint8 i;

	for(i = 0; i <size; i++)
	{
		uartGetByte_ExpectAndReturn(frame[i]);
	}
}

void test_getNonReadyTLVframe_given_both_frame_not_ready_should_return_bufferPointers0(void)
{
	// tlvFrame = 0x11;
	tlvBuf.readyFrame = 3;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlvBuf);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getNonReadyTLVframe_given_frame_1_not_ready_should_return_bufferPointers0(void)
{
	// tlvFrame = 0x01;
	tlvBuf.readyFrame = 1;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlvBuf);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getNonReadyTLVframe_given_frame_2_not_ready_should_return_bufferPointers1(void)
{
	// tlvFrame = 0x10;
	tlvBuf.readyFrame = 2;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlvBuf);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
}

void test_getNonReadyTLVframe_should_return_0_if_all_frame_ready(void)
{
	tlvBuf.readyFrame = 0;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlvBuf);

	TEST_ASSERT_EQUAL_PTR(0, ptr);
}

void test_setTLVframe_given_frame_1_ready_should_set_frame_1_to_0(void)
{
	// TLV tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}, 1};
	tlvBuf.readyFrame = 1;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlvBuf);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
	setTLVframe(&tlvBuf, ptr);
	TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_setTLVframe_given_frame_2_ready_should_set_frame_2_to_0(void)
{
	// TLV tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}, 2};
	tlvBuf.readyFrame = 2;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlvBuf);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
	setTLVframe(&tlvBuf, ptr);
	TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_SerialISR_given_tlv_in_the_frame_should_get_byte_from_the_uart(void)
{
	int i;
	tlvBuf.readyFrame = 3;
	uint8 frame[] = {0x01,						//type
					0x07,						//length
					0x04, 0x00, 0x00, 0x00,		//address
					0x02, 0x0e,					//data
					0x47};						//checksum

	setupUartGetByte(frame, sizeof(frame));

	for(i = 0; i < sizeof(frame); i++)
	{
		SerialISR();
	}

	TEST_ASSERT_EQUAL(2, tlvBuf.readyFrame);
}

void test_SerialISR_given_2_tlv_in_the_frame_should_get_byte_from_the_uart(void)
{
	int i;
	tlvBuf.readyFrame = 3;
	uint8 frame1[] = {0x01,						//type
					0x07,						//length
					0x04, 0x00, 0x00, 0x00,		//address
					0x02, 0x0e,					//data
					0x47};						//checksum

	uint8 frame2[] = {0x01,						//type
					 0x09,						//length
					 0x01, 0x00, 0x30, 0x00,	//address
					 0x45, 0x18, 0x73, 0x27,	//data
					 0x47};						//checksum

	setupUartGetByte(frame1, sizeof(frame1));

	for(i = 0; i < sizeof(frame1); i++)
	{
		SerialISR();
	}

	setupUartGetByte(frame2, sizeof(frame2));

	for(i = 0; i < sizeof(frame2); i++)
	{
		SerialISR();
	}

	TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_SerialISR_given_3_tlv_in_the_frame_should_get_byte_from_the_uart_and_sent_NACK_to_PC_if_both_buffer_not_available(void)
{
	int i;
	tlvBuf.readyFrame = 3;
	uint8 frame1[] = {0x01,						//type
					0x07,						//length
					0x04, 0x00, 0x00, 0x00,		//address
					0x02, 0x0e,					//data
					0x47};						//checksum

	uint8 frame2[] = {0x01,						//type
					 0x09,						//length
					 0x01, 0x00, 0x30, 0x00,	//address
					 0x45, 0x18, 0x73, 0x27,	//data
					 0x47};						//checksum

	uint8 frame3[] = {0x01,						//type
					 0x09,						//length
					 0x01, 0x00, 0x30, 0x00,	//address
					 0x45, 0x18, 0x73, 0x27,	//data
					 0x47};						//checksum

	setupUartGetByte(frame1, sizeof(frame1));

	for(i = 0; i < sizeof(frame1); i++)
	{
		SerialISR();
	}

	setupUartGetByte(frame2, sizeof(frame2));

	for(i = 0; i < sizeof(frame2); i++)
	{
		SerialISR();
	}

	uartSendByte_Expect(NACK);
	SerialISR();
	TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_getReadyTLVframe_given_frame_1_ready_should_return_bufferPointers0(void)
{
	// TLV tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlvBuf.readyFrame = 2;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlvBuf);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getReadyTLVframe_given_frame_2_ready_should_return_bufferPointers1(void)
{
	// TLV tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlvBuf.readyFrame = 1;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlvBuf);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
}

void test_getReadyTLVframe_given_both_frame_ready_should_return_bufferPointers0(void)
{
	// TLV tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlvBuf.readyFrame = 0;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlvBuf);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_releaseTLVframe_given_frame_1_done_writing_should_release_frame_1_to_1(void)
{
	// TLV tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlvBuf.readyFrame = 1;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlvBuf);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
	releaseTLVframe(&tlvBuf, ptr);
	TEST_ASSERT_EQUAL(3, tlvBuf.readyFrame);
}

void test_releaseTLVframe_given_frame_0_done_writing_should_release_frame_0_to_1(void)
{
	// TLV tlvBuf = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlvBuf.readyFrame = 2;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlvBuf);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
	releaseTLVframe(&tlvBuf, ptr);
	TEST_ASSERT_EQUAL(3, tlvBuf.readyFrame);
}

void test_getAddress_should_return_the_complete_4byte_address_from_the_frame(void)
{
	tlvBuf.readyFrame = 2;
	uint8 *ptr;
	uint8 data[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};

	ptr	= data;
	TEST_ASSERT_EQUAL(0x00300001, getAddress(ptr));
}

void test_getLength_should_return_the_length_of_the_data_from_the_frame(void)
{
	tlvBuf.readyFrame = 2;
	uint8 *ptr;
	uint8 data[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};

	ptr	= data;
	TEST_ASSERT_EQUAL(2, getLength(ptr));
}

void test_getData_given_frame_should_get_the_data(void)
{
	tlvBuf.readyFrame = 2;
	uint8 *ptr, *dataReturn, length;
	uint8 frame[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};
	uint8 BigEndianData[] = {0x02, 0xe7};

	ptr	= frame;
	dataReturn = getData(ptr);
	length = getLength(ptr);
	TEST_ASSERT_EQUAL_tlvDecode(length, BigEndianData, dataReturn);
}

void test_decodeCommand_should_call_bufferHandler(void)
{
	FlashBuffer fb;
	uint8 frame[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};
	uint8 *ptrTLV;

	ptrTLV	= frame;

	bufferHandler_Expect(0x00300001, &ptrTLV[6], 2, &fb);

	decodeCommand(&fb, ptrTLV);
}

void test_isReadyFrameAvailable_should_return_1_if_one_of_the_frame_is_ready(void)
{
	tlvBuf.readyFrame = 1;
	
	TEST_ASSERT_EQUAL(1, isReadyFrameAvailable(&tlvBuf));
}

void test_isReadyFrameAvailable_should_return_0_if_both_of_the_frame_is_not_ready(void)
{
	tlvBuf.readyFrame = 3;
	
	TEST_ASSERT_EQUAL(0, isReadyFrameAvailable(&tlvBuf));
}