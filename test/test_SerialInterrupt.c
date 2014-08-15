#include "unity.h"
#include "SerialInterrupt.h"
#include "mock_UART.h"
#include "p18f4520.h"

extern uint8 tlvBuffer[160];
extern uint8 tlvFrameReady;
extern uint8 *bufferPointers;
extern uint8 byteReceive;
extern uint8 bufferIndex;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_tlvReceiveFSM_given_bufferIndex_0_and_fsm_should_put_value_into_the_tlvBuffer_start_from_location_0_and_move_to_next_state(void)
{
	TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
	byteReceive = 1;
	bufferIndex = 0;
	
	tlvReceiveFSM(&fsm);
	
	TEST_ASSERT_EQUAL(1, tlvBuffer[0]);
	TEST_ASSERT_EQUAL(WAIT_FOR_LENGTH, fsm.state);
	TEST_ASSERT_EQUAL(1, fsm.i);
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
}