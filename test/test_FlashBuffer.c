#include "unity.h"
#include "Utils.h"

#include "FlashBuffer.h"
#include "mock_spiMaster.h"

#include "p18f4520.h"
#include "mock_delays.h"

#define ACK 0xA5
#define Write 0x11
#define Read 0x22
#define Done 0xFF

extern uint8 slaveBuffer[64];

void setUp(void)
{
}

void tearDown(void)
{
}

void test_flashBufferGetLastAddress_given_segement_0_offset_10_will_return_10()
{
	FlashBuffer fb;
	fb.segment = 0;
	fb.offset = 10 ;

	TEST_ASSERT_EQUAL(10,flashBufferGetLastAddress(&fb));
}

void test_flashBufferGetLastAddress_given_segement_minus1_will_return_0xFFFFFFFF()
{
	FlashBuffer fb;
	fb.segment = -1;

	TEST_ASSERT_EQUAL(-1,flashBufferGetLastAddress(&fb));
	TEST_ASSERT_EQUAL(0xFFFFFFFF,flashBufferGetLastAddress(&fb));
}

void test_flashBufferFlush_given_segment_20__will_call_spiSendCommand_spiSendAddress_spiSendData_spiCheckStatus_and_given_all_pass_will_return_1()
{
	uint8 data[64] = {1,2,3,4,5,6,7,8,9};
	uint8 addressArray[3] = {10,11,12};
	uint8 acknack;
	FlashBuffer fb;
	fb.segment = 20;
	fb.buffer = data;


	spiSendCommand_ExpectAndReturn(Write,1);
	spiSendAddressSegment_ExpectAndReturn(&fb,1);
	spiSendData_ExpectAndReturn(fb.buffer,64,0,1);
	spiReceiveStatus_ExpectAndReturn(1);


	TEST_ASSERT_EQUAL(1,flashBufferFlush(&fb));

}

void test_flashBufferFlush_given_segment_20__will_call_spiSendCommand_given_spiSendCommand_fail_will_return_0()
{
	uint8 data[64] = {1,2,3,4,5,6,7,8,9};
	uint8 addressArray[3] = {10,11,12};
	uint8 acknack;
	FlashBuffer fb;
	fb.segment = 20;
	fb.buffer = data;

	spiSendCommand_ExpectAndReturn(Write,0);


	TEST_ASSERT_EQUAL(0,flashBufferFlush(&fb));

}

void test_flashBufferFlush_given_segment_20__given_spiReceiveStatus_fail_will_loop_until_pass()
{
	uint8 data[64] = {1,2,3,4,5,6,7,8,9};
	uint8 addressArray[3] = {10,11,12};
	uint8 acknack;
	FlashBuffer fb;
	fb.segment = 20;
	fb.buffer = data;


	spiSendCommand_ExpectAndReturn(Write,1);
	spiSendAddressSegment_ExpectAndReturn(&fb,1);
	spiSendData_ExpectAndReturn(fb.buffer,64,0,1);

	spiReceiveStatus_ExpectAndReturn(0);
	Delay10TCYx_Expect(2);
	spiReceiveStatus_ExpectAndReturn(0);
	Delay10TCYx_Expect(2);
	spiReceiveStatus_ExpectAndReturn(1);

	TEST_ASSERT_EQUAL(1,flashBufferFlush(&fb));

}


void test_flashBufferRead_will_call_spiSendCommand_spiSendAddressSegment_and_spiReceiveData()
{

	FlashBuffer fb;
	fb.segment = 1;

	spiSendCommand_ExpectAndReturn(Read,1);
	spiSendAddressSegment_ExpectAndReturn(&fb,1);
	spiReceiveData_ExpectAndReturn(slaveBuffer,64,1);

	flashBufferRead(&fb);
}


void test_isFlashBufferNull_will_return_1_is_fb_buffer_points_to_NULL()
{
	FlashBuffer fb;
	fb.buffer = NULL;

	TEST_ASSERT_EQUAL(1,isFlashBufferNull(&fb));

}

void test_isFlashBufferNull_will_return_0_is_fb_buffer_points_to_something()
{
	uint8 data[64] = {1,2,3,4,5,6,7,8,9};
	FlashBuffer fb;

	fb.buffer = data;

	TEST_ASSERT_EQUAL(0,isFlashBufferNull(&fb));
}

