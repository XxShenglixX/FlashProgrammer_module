#include "unity.h"
#include "DelayedWrite.h"

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
	uint8 dataCount = 5 ;
	uint8 dataStartPoint = 0;
	
	FlashBuffer fb ;
	fb.buffer = buffer;
	
	dataToBuffer(address,data,dataCount,dataStartPoint,&fb);
	
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
	uint8 dataCount = 5 ;
	uint8 dataStartPoint = 0;
	
	FlashBuffer fb ;
	fb.buffer = buffer;
	
	dataToBuffer(address,data,dataCount,dataStartPoint,&fb);
	
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

void test_bufferHandler_given_address_0x10_segment_0_dataCount_10_should_not_flush_buffer()
{
	uint32 address = 0x10 ;
	uint8 buffer[64] ;
	uint8 data[20] = {1,2,3,4,5,6,7,8,9,10} ;
	uint8 dataCount = 10 ;
	
	FlashBuffer fb ;
	fb.buffer = buffer;
	fb.segment = 0 ;
	
	bufferHandler(address,data,dataCount,&fb);
	TEST_ASSERT_EQUAL(1,buffer[16]);
	TEST_ASSERT_EQUAL(2,buffer[17]);
	TEST_ASSERT_EQUAL(3,buffer[18]);
	TEST_ASSERT_EQUAL(4,buffer[19]);
	TEST_ASSERT_EQUAL(5,buffer[20]);
	TEST_ASSERT_EQUAL(6,buffer[21]);
	TEST_ASSERT_EQUAL(7,buffer[22]);
	TEST_ASSERT_EQUAL(8,buffer[23]);
	TEST_ASSERT_EQUAL(9,buffer[24]);
	TEST_ASSERT_EQUAL(10,buffer[25]);
	TEST_ASSERT_EQUAL(26,fb.offset);
	TEST_ASSERT_EQUAL(0,fb.segment); 

}

void test_bufferHandler_given_address_0x10_segment_1_dataCount_5_should_flush_buffer()
{
	uint32 address = 0x10 ;
	uint8 buffer[64] ;
	uint8 data[20] = {1,2,3,4,5,6,7,8,9,10} ;
	uint8 dataCount = 5 ;
	
	FlashBuffer fb ;
	fb.buffer = buffer;
	fb.segment = 1 ;
	
	
	flashBufferFlush_ExpectAndReturn(&fb,1);
	bufferHandler(address,data,dataCount,&fb);
	
	TEST_ASSERT_EQUAL(1,buffer[16]);
	TEST_ASSERT_EQUAL(2,buffer[17]);
	TEST_ASSERT_EQUAL(3,buffer[18]);
	TEST_ASSERT_EQUAL(4,buffer[19]);
	TEST_ASSERT_EQUAL(5,buffer[20]);
	TEST_ASSERT_EQUAL(21,fb.offset);
	TEST_ASSERT_EQUAL(0,fb.segment); 
}

void test_bufferHandler_given_flushBufferflush_fail_will_repeat_flushing_until_success()
{
	uint32 address = 0x10 ;
	uint8 buffer[64] ;
	uint8 data[20] = {1,2,3,4,5,6,7,8,9,10} ;
	uint8 dataCount = 5 ;
	
	FlashBuffer fb ;
	fb.buffer = buffer;
	fb.segment = 1 ;
	
	
	flashBufferFlush_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb,0);
	flashBufferFlush_ExpectAndReturn(&fb,1);
	bufferHandler(address,data,dataCount,&fb);
	
	TEST_ASSERT_EQUAL(1,buffer[16]);
	TEST_ASSERT_EQUAL(2,buffer[17]);
	TEST_ASSERT_EQUAL(3,buffer[18]);
	TEST_ASSERT_EQUAL(4,buffer[19]);
	TEST_ASSERT_EQUAL(5,buffer[20]);
	TEST_ASSERT_EQUAL(21,fb.offset);
	TEST_ASSERT_EQUAL(0,fb.segment); 
}

void test_bufferHandler_given_address_0x3C_segment_0_dataCount_5_should_flush_buffer()
{
	uint32 address = 60 ;
	uint8 buffer[64] ;
	uint8 data[20] = {1,2,3,4,5,6,7,8,9,10} ;
	uint8 dataCount = 5 ;
	
	FlashBuffer fb ;
	fb.buffer = buffer;
	fb.segment = 0 ;
	fb.offset = address - (fb.segment * 64);
	
	flashBufferFlush_ExpectAndReturn(&fb,1);
	bufferHandler(address,data,dataCount,&fb);
	
	TEST_ASSERT_EQUAL(5,buffer[0]);
	TEST_ASSERT_EQUAL(1,fb.offset);
	TEST_ASSERT_EQUAL(1,fb.segment); 
}

