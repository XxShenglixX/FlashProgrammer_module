#include "unity.h"
#include "DelayedWrite.h"
#include "mock_spiMaster.h"
#include "mock_FlashBuffer.h"
#include "Utils.h"



void setUp(void)
{
}

void tearDown(void)
{
}

void test_dataToBuffer_given_address_0x10_dataCount_5_should_return_data_and_offset_21_segment_0()
{
	uint32 address = 0x10 ;
	uint8 buffer[64] = {11};
	uint8 data[20] = {1,2,3,4,5,6,7,8,9} ;
	uint8 length = 5 ;
	uint8 dataStartPoint = 0;

	FlashBuffer fb ;
	fb.buffer = buffer;

	dataToBuffer(address,data,length,dataStartPoint,&fb);

 	TEST_ASSERT_EQUAL(1,buffer[16]);
	TEST_ASSERT_EQUAL(2,buffer[17]);
	TEST_ASSERT_EQUAL(3,buffer[18]);
	TEST_ASSERT_EQUAL(4,buffer[19]);
	TEST_ASSERT_EQUAL(5,buffer[20]);
	TEST_ASSERT_EQUAL(21,fb.offset);
	TEST_ASSERT_EQUAL(0,fb.segment);
}

void test_dataToBuffer_given_address_0x10_dataCount_5_address_0x1A_dataCount_3_should_return_offset_29_segment_0()
{
	uint32 address = 0x10 ;
	uint8 buffer[64] ;
	uint8 data[20] = {1,2,3,4,5,6,7,8,9} ;
	uint8 length = 5 ;
	uint8 dataStartPoint = 0;

	FlashBuffer fb ;
	fb.buffer = buffer;

	dataToBuffer(address,data,length,dataStartPoint,&fb);

  	TEST_ASSERT_EQUAL(1,buffer[16]);
	TEST_ASSERT_EQUAL(2,buffer[17]);
	TEST_ASSERT_EQUAL(3,buffer[18]);
	TEST_ASSERT_EQUAL(4,buffer[19]);
	TEST_ASSERT_EQUAL(5,buffer[20]);
	TEST_ASSERT_EQUAL(21,fb.offset);
	TEST_ASSERT_EQUAL(0,fb.segment);

	data[0] = 14 ; data[1] = 15 ; data[2]=16 ;
	dataToBuffer(0x1A,data,3,dataStartPoint,&fb);

 	TEST_ASSERT_EQUAL(14,buffer[26]);
	TEST_ASSERT_EQUAL(15,buffer[27]);
	TEST_ASSERT_EQUAL(16,buffer[28]);
	TEST_ASSERT_EQUAL(29,fb.offset);
	TEST_ASSERT_EQUAL(0,fb.segment);
}

void test_bufferHandler_given_segment_0_address_0_length_10_null_buffer_should_call_readBuffer_dataToBuffer_()
{
	uint32 address = 0;
	uint8 length = 10, buffer[64] = {1,2,3,4,5,6,7,8,9,10};
	uint8 data[20] = {11,12,13,14,15,16,17,18,19,20};

	FlashBuffer fb;
	fb.buffer = 0 ;
	fb.segment = 0 ;

	isFlashBufferNull_ExpectAndReturn(&fb,1);
	flashBufferRead_ExpectAndReturn(&fb,1);
	fb.buffer = buffer ;
	bufferHandler(address,data,length,&fb);

}

void test_bufferHandler_given_segment_0_address_0_length_10_should_call_dataToBuffer_and_dont_read_and_flush_buffer()
{
	uint32 address = 0;
	uint8 length = 10 , buffer[64] = {1,2,3,4,5,6,7,8,9,10};
	uint8 data[20] = {11,12,13,14,15,16,17,18,19,20};

	FlashBuffer fb;
	fb.buffer = buffer ;
	fb.segment = 0 ;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	bufferHandler(address,data,length,&fb);

	TEST_ASSERT_EQUAL(0,fb.segment);
	TEST_ASSERT_EQUAL(10,fb.offset);

	TEST_ASSERT_EQUAL(11,buffer[0]);TEST_ASSERT_EQUAL(12,buffer[1]);
	TEST_ASSERT_EQUAL(13,buffer[2]);TEST_ASSERT_EQUAL(14,buffer[3]);
	TEST_ASSERT_EQUAL(15,buffer[4]);TEST_ASSERT_EQUAL(16,buffer[5]);
	TEST_ASSERT_EQUAL(17,buffer[6]);TEST_ASSERT_EQUAL(18,buffer[7]);
	TEST_ASSERT_EQUAL(19,buffer[8]);TEST_ASSERT_EQUAL(20,buffer[9]);

}

void test_bufferHandler_given_segment_0_address_60_length_5_should_call_dataToBuffer_flush_buffer_read_buffer_and_dataToBuffer()
{
	uint32 address = 60;
	uint8 length = 5 , buffer[64] = {1,2,3,4,5,6,7,8,9,10};
	uint8 data[20] = {11,12,13,14,15,16,17,18,19,20};

	FlashBuffer fb;
	fb.offset = 60;
	fb.buffer = buffer ;
	fb.segment = 0 ;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb,1);
	flashBufferRead_ExpectAndReturn(&fb,1);
	bufferHandler(address,data,length,&fb);

	TEST_ASSERT_EQUAL(1,fb.segment);
	TEST_ASSERT_EQUAL(1,fb.offset);

	TEST_ASSERT_EQUAL(15,buffer[0]);TEST_ASSERT_EQUAL(2,buffer[1]);
	TEST_ASSERT_EQUAL(3,buffer[2]);TEST_ASSERT_EQUAL(4,buffer[3]);
	TEST_ASSERT_EQUAL(5,buffer[4]);TEST_ASSERT_EQUAL(6,buffer[5]);
	TEST_ASSERT_EQUAL(7,buffer[6]);TEST_ASSERT_EQUAL(8,buffer[7]);
	TEST_ASSERT_EQUAL(9,buffer[8]);TEST_ASSERT_EQUAL(10,buffer[9]);
}


void test_bufferHandler_given_segment_0_address_64_length_2_should_call_flush_buffer_read_buffer_and_dataToBuffer()
{
	uint32 address = 64;
	uint8 length = 2 , buffer[64] = {1,2,3,4,5,6,7,8,9,10};
	uint8 data[20] = {21,22,23,24,25,26,27,28,29,30};

	FlashBuffer fb;
	fb.buffer = buffer ;
	fb.segment = 0 ;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb,1);
	flashBufferRead_ExpectAndReturn(&fb,1);

	bufferHandler(address,data,length,&fb);

	TEST_ASSERT_EQUAL(1,fb.segment);
	TEST_ASSERT_EQUAL(2,fb.offset);

	TEST_ASSERT_EQUAL(21,buffer[0]);TEST_ASSERT_EQUAL(22,buffer[1]);
	TEST_ASSERT_EQUAL(3,buffer[2]);TEST_ASSERT_EQUAL(4,buffer[3]);
	TEST_ASSERT_EQUAL(5,buffer[4]);TEST_ASSERT_EQUAL(6,buffer[5]);
	TEST_ASSERT_EQUAL(7,buffer[6]);TEST_ASSERT_EQUAL(8,buffer[7]);
	TEST_ASSERT_EQUAL(9,buffer[8]);TEST_ASSERT_EQUAL(10,buffer[9]);

}

void test_bufferHandler_given_segment_1_address_0x300000_should_call_flushBufferFlush_spiSendConfig()
{
	uint32 address = 0x300000;
	uint8 length = 1 , buffer[64] = {1,2,3,4,5,6,7,8,9,10};
	uint8 data[20] = {21,22,23,24,25,26,27,28,29,30};

	FlashBuffer fb;
	fb.buffer = buffer ;
	fb.segment = 1 ;

	isFlashBufferNull_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb,1);
	spiSendConfig_ExpectAndReturn(address,data,1);

	bufferHandler(address,data,length,&fb);
}








 void test_crossSegmentHandler_should_call_dataToBuffer_flushBufferFlush_flushBufferRead_and_dataToBuffer()
{
	uint32 address = 63;
	uint8 length = 2 , buffer[64] = {1,2,3,4,5,6,7,8,9,10};
	uint8 data[20] = {21,22,23,24,25,26,27,28,29,30};

	FlashBuffer fb;
	fb.buffer = buffer ;
	fb.segment = 0 ;
	fb.offset = 63;

	flashBufferFlush_ExpectAndReturn(&fb,1);
	flashBufferRead_ExpectAndReturn(&fb,1);
	crossSegmentHandler(address,data,length,0,&fb);

	TEST_ASSERT_EQUAL(1,fb.segment);
	TEST_ASSERT_EQUAL(1,fb.offset);

	TEST_ASSERT_EQUAL(22,buffer[0]);TEST_ASSERT_EQUAL(2,buffer[1]);
	TEST_ASSERT_EQUAL(3,buffer[2]);TEST_ASSERT_EQUAL(4,buffer[3]);
	TEST_ASSERT_EQUAL(5,buffer[4]);TEST_ASSERT_EQUAL(6,buffer[5]);
	TEST_ASSERT_EQUAL(7,buffer[6]);TEST_ASSERT_EQUAL(8,buffer[7]);
	TEST_ASSERT_EQUAL(9,buffer[8]);TEST_ASSERT_EQUAL(10,buffer[9]);

}

 void test_differentSegmentHandler_call_flushBufferFlush_flushBufferRead_and_dataToBuffer()
{
	uint32 address = 64;
	uint8 length = 2 , buffer[64] = {1,2,3,4,5,6,7,8,9,10};
	uint8 data[20] = {21,22,23,24,25,26,27,28,29,30};

	FlashBuffer fb;
	fb.buffer = buffer ;
	fb.segment = 0 ;

	flashBufferFlush_ExpectAndReturn(&fb,1);
	flashBufferRead_ExpectAndReturn(&fb,1);

	differentSegmentHandler(address,data,length,0,&fb);

	TEST_ASSERT_EQUAL(1,fb.segment);
	TEST_ASSERT_EQUAL(2,fb.offset);


	TEST_ASSERT_EQUAL(21,buffer[0]);TEST_ASSERT_EQUAL(22,buffer[1]);
	TEST_ASSERT_EQUAL(3,buffer[2]);TEST_ASSERT_EQUAL(4,buffer[3]);
	TEST_ASSERT_EQUAL(5,buffer[4]);TEST_ASSERT_EQUAL(6,buffer[5]);
	TEST_ASSERT_EQUAL(7,buffer[6]);TEST_ASSERT_EQUAL(8,buffer[7]);
	TEST_ASSERT_EQUAL(9,buffer[8]);TEST_ASSERT_EQUAL(10,buffer[9]);
}

void test_configurationDataHandler_should_call_flushBufferFlush_and_spiSendConfig()
{
	uint32 address = 0x300000;
	uint8 data[20];
	FlashBuffer fb;

	flashBufferFlush_ExpectAndReturn(&fb,1);
	spiSendConfig_ExpectAndReturn(address,data,1);

	configurationDataHandler(address,data,&fb);
}
























































void test_isAddressWithinRange_given_address_0x32_length_0_fb_segment_0_should_return_1_for_within_range()
{
	uint32 address = 0x32 ;
	uint32 length = 0 ;
	FlashBuffer fb ;
	fb.segment = 0;



	TEST_ASSERT_EQUAL(1,isAddressInRange(address,length,&fb));
}

void test_isAddressWithinRange_given_address_0x32_length_10_fb_segment_0_should_return_0_for_out_of_range()
{
	uint32 address = 0x32 ;
	uint32 length = 10 ;
	FlashBuffer fb ;
	fb.segment = 0;

	TEST_ASSERT_EQUAL(1,isAddressInRange(address,length,&fb));
}

void test_isSameSegment_given_address_0x40_fb_segmen_1_should_return_1_for_same_segment()
{
	uint32 address = 0x40;
	FlashBuffer fb ;
	fb.segment = 1;

	TEST_ASSERT_EQUAL(1,isSameSegment(address,&fb));

}

void test_isSameSegment_given_address_0x39_fb_segmen_1_should_return_0_for_different_segment()
{
	uint32 address = 0x39;
	FlashBuffer fb ;
	fb.segment = 1;

	TEST_ASSERT_EQUAL(0,isSameSegment(address,&fb));

}

void test_isConfigData_given_address_0x300000_should_return_1_for_true()
{
	uint32 address = 0x300000;

	TEST_ASSERT_EQUAL(1,isConfigData(address));
}

void test_isConfigData_given_address_0x10_should_return_0_for_false()
{
	uint32 address = 0x10;

	TEST_ASSERT_EQUAL(0,isConfigData(address));
}