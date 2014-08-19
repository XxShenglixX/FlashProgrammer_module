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
// extern uint8 tlvFrameReady;
extern uint8 byteReceive;
extern uint8 bufferIndex;
// extern uint8 *bufferPointers[];

extern TLV_Buffer tlv;

void setUp(void)
{
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
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlv.readyFrame = 3;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlv);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getNonReadyTLVframe_given_frame_2_not_ready_should_return_bufferPointers1(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 2};
	tlv.readyFrame = 2;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlv);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
}

void test_getNonReadyTLVframe_should_return_0_if_all_frame_ready(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 0};
	tlv.readyFrame = 0;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlv);

	// if(ptr == 0)
		// printf("yes\n");

	TEST_ASSERT_EQUAL_PTR(0, ptr);
}

void test_setTLVframe_given_frame_1_ready_should_set_frame_1_to_0(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 1};
	tlv.readyFrame = 1;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlv);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
	setTLVframe(&tlv, ptr);
	TEST_ASSERT_EQUAL(0, tlv.readyFrame);
}

void test_setTLVframe_given_frame_2_ready_should_set_frame_2_to_0(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 2};
	tlv.readyFrame = 2;
	uint8 *ptr;

	ptr	= getNonReadyTLVframe(&tlv);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
	setTLVframe(&tlv, ptr);
	TEST_ASSERT_EQUAL(0, tlv.readyFrame);
}

void test_SerialISR_given_tlv_in_the_frame_should_get_byte_from_the_uart(void)
{
	int i;
	tlv.readyFrame = 3;
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

	TEST_ASSERT_EQUAL(2, tlv.readyFrame);
}

void test_SerialISR_given_2_tlv_in_the_frame_should_get_byte_from_the_uart(void)
{
	int i;
	tlv.readyFrame = 3;
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

	TEST_ASSERT_EQUAL(0, tlv.readyFrame);
}

void test_SerialISR_given_3_tlv_in_the_frame_should_get_byte_from_the_uart_and_sent_NACK_to_PC_if_both_buffer_not_available(void)
{
	int i;
	tlv.readyFrame = 3;
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
	TEST_ASSERT_EQUAL(0, tlv.readyFrame);
}

void test_getReadyTLVframe_given_frame_1_ready_should_return_bufferPointers0(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlv.readyFrame = 2;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlv);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getReadyTLVframe_given_frame_2_ready_should_return_bufferPointers1(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlv.readyFrame = 1;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlv);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
}

void test_getReadyTLVframe_given_both_frame_ready_should_return_bufferPointers0(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlv.readyFrame = 0;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlv);

	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_releaseTLVframe_given_frame_1_done_writing_should_release_frame_1_to_1(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlv.readyFrame = 1;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlv);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
	releaseTLVframe(&tlv, ptr);
	TEST_ASSERT_EQUAL(3, tlv.readyFrame);
}

void test_releaseTLVframe_given_frame_0_done_writing_should_release_frame_0_to_1(void)
{
	// TLV tlv = {{&tlvBuffer[0], &tlvBuffer[80]}, 3};
	tlv.readyFrame = 2;
	uint8 *ptr;

	ptr	= getReadyTLVframe(&tlv);
	TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
	releaseTLVframe(&tlv, ptr);
	TEST_ASSERT_EQUAL(3, tlv.readyFrame);
}

void test_getAddress_should_return_the_complete_4byte_address_from_the_frame(void)
{
	tlv.readyFrame = 2;
	uint8 *ptr;
	uint8 data[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};

	ptr	= data;
	TEST_ASSERT_EQUAL(0x00300001, getAddress(ptr));
}

void test_getLength_should_return_the_length_of_the_data_from_the_frame(void)
{
	tlv.readyFrame = 2;
	uint8 *ptr;
	uint8 data[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};

	ptr	= data;
	TEST_ASSERT_EQUAL(2, getLength(ptr));
}

void test_getData_given_frame_should_get_the_data(void)
{
	tlv.readyFrame = 2;
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
	uint8* ptrTLV;

	ptrTLV	= frame;

	bufferHandler_Expect(0x00300001, &ptrTLV[6], 2, &fb);

	decodeCommand(&fb, ptrTLV);
}

void test_isReadyFrameAvailable_should_return_1_if_one_of_the_frame_is_ready(void)
{
	tlv.readyFrame = 1;
	
	TEST_ASSERT_EQUAL(1, isReadyFrameAvailable(&tlv));
}

void test_isReadyFrameAvailable_should_return_0_if_both_of_the_frame_is_not_ready(void)
{
	tlv.readyFrame = 3;
	
	TEST_ASSERT_EQUAL(0, isReadyFrameAvailable(&tlv));
}

/*void test_tlvDecode_given_array_of_data_store_in_buffer0_should_decode_the_tlv_in_buffer0_address_length_and_data(void)
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

void test_tlvReceiveFSM_given_bufferIndex_0_and_fsm_should_put_value_into_the_tlvBuffer_start_from_location_0_and_move_to_next_state(void)
{
	TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
	bufferIndex = 0;
	RCREG = 0xaa;

	uartGetByte_ExpectAndReturn(RCREG);

	tlvReceiveFSM(&fsm);

	TEST_ASSERT_EQUAL(0xaa, tlvBuffer[0]);
	TEST_ASSERT_EQUAL(WAIT_FOR_LENGTH, fsm.state);
	TEST_ASSERT_EQUAL(1, fsm.i);
	TEST_ASSERT_EQUAL(RCREG, bufferPointers[bufferIndex][fsm.i]);
}

void test_tlvReceiveFSM_given_bufferIndex_1_and_fsm_should_put_value_into_the_tlvBuffer_start_from_location_80_and_move_to_next_state(void)
{
	TLV_FSM fsm = {WAIT_FOR_LENGTH, 1};
	byteReceive = 4;
	bufferIndex = 1;

	tlvReceiveFSM(&fsm);

	TEST_ASSERT_EQUAL(4, tlvBuffer[81]);
	TEST_ASSERT_EQUAL(4, fsm.length);
	TEST_ASSERT_EQUAL(WAIT_FOR_VALUE, fsm.state);
	TEST_ASSERT_EQUAL(2, fsm.i);
}

void test_tlvReceiveFSM_given_WAIT_FOR_VALUE_state_shoud_remain_in_the_state_if_byte_receive_is_the_first_byte_of_the_value(void)
{
	TLV_FSM fsm = {WAIT_FOR_VALUE, 3, 3};
	byteReceive = 4;
	bufferIndex = 1;

	tlvReceiveFSM(&fsm);

	TEST_ASSERT_EQUAL(4, tlvBuffer[83]);
	TEST_ASSERT_EQUAL(3, fsm.length);
	TEST_ASSERT_EQUAL(WAIT_FOR_VALUE, fsm.state);
	TEST_ASSERT_EQUAL(4, fsm.i);
}

void test_tlvReceiveFSM_given_WAIT_FOR_VALUE_state_shoud_move_to_next_state_if_byte_receive_is_the_last_byte_of_the_value(void)
{
	TLV_FSM fsm = {WAIT_FOR_VALUE, 4, 3};
	byteReceive = 4;
	bufferIndex = 1;

	tlvReceiveFSM(&fsm);

	TEST_ASSERT_EQUAL(4, tlvBuffer[84]);
	TEST_ASSERT_EQUAL(3, fsm.length);
	TEST_ASSERT_EQUAL(WAIT_FOR_TYPE, fsm.state);
	TEST_ASSERT_EQUAL(0, fsm.i);
}

void test_SerialISR_should_receive_byte_and_set_bufferIndex_to_0_if_tlvFrameReady_equal_to_both_buffer_available(void)
{
	RCREG = 0xaa;
	tlvFrameReady = BOTH_AVAILABLE;

	uartGetByte_ExpectAndReturn(RCREG);
	uartSendByte_Expect(ACK);

	SerialISR();

	TEST_ASSERT_EQUAL(0, bufferIndex);
}

void test_SerialISR_should_receive_byte_and_set_bufferIndex_to_1_if_tlvFrameReady_equal_to_buffer1_available(void)
{
	RCREG = 0xaa;
	tlvFrameReady = BUFFER1_AVAILABLE;

	uartGetByte_ExpectAndReturn(RCREG);
	uartSendByte_Expect(ACK);

	SerialISR();

	TEST_ASSERT_EQUAL(1, bufferIndex);
}

void test_SerialISR_should_send_byte_of_NACK_if_both_buffer_not_available(void)
{
	tlvFrameReady = NOT_AVAILABLE;

	uartGetByte_ExpectAndReturn(RCREG);
	uartSendByte_Expect(NACK);

	SerialISR();
}

void test_chk_SerialISR_should_call_SerialISR_to_receive_byte_and_set_bufferIndex_to_0_if_both_buffer_available(void)
{
	RCREG = 0x55;
	PIR1bits.RCIF = 1;
	tlvFrameReady = BOTH_AVAILABLE;

	uartGetByte_ExpectAndReturn(RCREG);
	uartSendByte_Expect(ACK);

	chk_SerialISR();

	TEST_ASSERT_EQUAL(0, bufferIndex);
} */