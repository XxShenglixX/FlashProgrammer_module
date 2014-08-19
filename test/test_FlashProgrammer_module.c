#include "unity.h"
#include "FlashProgrammer_module.h"
#include "p18f4520.h"
#include "UART.h"
#include "SerialInterrupt.h"
#include "Utils.h"
#include "CustomTypeAssertion.h"
#include <stdio.h>

extern uint8 tlvBuffer[160];
extern uint8 tlvFrameReady;
extern uint8 *bufferPointers[];

void setUp(void)
{
}

void tearDown(void)
{
}

void test_tlvDecode_given_array_of_data_store_in_buffer0_should_decode_the_tlv_in_buffer0_address_length_and_data(void)
{
	uint32 address;
	uint8 length;
	uint8 data[] = {0x01, 0x07, 0x10, 0x00, 0x02, 0x00, 0x02, 0xe7, 0xff}, dataOnly[80];
	bufferPointers[0] = data;
	uint8 BigEndianData[] = {0x02, 0xe7};

	// printf("tlvBuffer[0]: %d\n", tlvBuffer[0]);
	// printf("bufferPointers[0][0]: %d\n", bufferPointers[0][0]);
	// printf("data[0]: %d\n", data[0]);

	tlvDecode(&address, &length, bufferPointers[0], dataOnly);

	// if(dataOnly[2] == '\0')
		// printf("YES\n");

	// printf("dataOnly[2]: %d\n", BigEndianData[3]);

	TEST_ASSERT_EQUAL(0X020010, address);
	TEST_ASSERT_EQUAL(2, length);
	TEST_ASSERT_EQUAL_tlvDecode(BigEndianData, dataOnly);
	// TEST_ASSERT_EQUAL(0xe7, dataOnly[0]);
	// TEST_ASSERT_EQUAL(0x02, dataOnly[1]);
}

void test_tlvDecode_given_different_array_of_data_store_in_buffer1_should_decode_the_tlv_in_buffer1_address_length_and_data(void)
{
	uint32 address;
	uint8 length;
	uint8 data[] = {0x01, 0x06, 0x01, 0x00, 0x30, 0x00, 0x0e, 0xe1}, *dataOnly;
	bufferPointers[1] = data;
	uint8 BigEndianData[] = {0x0e};
	// printf("bufferPointers[1][0]: %d\n", bufferPointers[1][0]);

	tlvDecode(&address, &length, bufferPointers[1], dataOnly);

	TEST_ASSERT_EQUAL(0X00300001, address);
	TEST_ASSERT_EQUAL(1, length);
	TEST_ASSERT_EQUAL_tlvDecode(BigEndianData, dataOnly);
	// TEST_ASSERT_EQUAL(0xe7, dataOnly[0]);
	// TEST_ASSERT_EQUAL(0x02, dataOnly[1]);
	// TEST_ASSERT_EQUAL(0x27, dataOnly[2]);
	// TEST_ASSERT_EQUAL(0x73, dataOnly[3]);
	// TEST_ASSERT_EQUAL(0x18, dataOnly[4]);
	// TEST_ASSERT_EQUAL(0x45, dataOnly[5]);
}