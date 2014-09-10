#include "unity.h"
#include "SerialInterrupt.h"
#include "CustomTypeAssertion.h"
#include "mock_UART.h"
#include "p18f4520.h"
#include "mock_DelayedWrite.h"
#include "mock_FlashBuffer.h"
#include "masterMain.h"
#include <stdio.h>

extern uint8 tlvBuffer[160];

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

void test_isFrame0Ready_given_readyFrame_equal_to_0_should_check_frame0_and_return_1_if_ready(void)
{
    tlvBuf.readyFrame = 0;

    TEST_ASSERT_EQUAL_PTR(1, isFrame0Ready(tlvBuf.readyFrame));
}

void test_isFrame0Ready_given_FRAME0_NOT_READY_should_check_frame0_and_return_0(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY;

    TEST_ASSERT_EQUAL_PTR(0, isFrame0Ready(tlvBuf.readyFrame));
}

void test_isFrame1Ready_given_FRAME0_NOT_READY_should_check_frame1_and_return_1_if_ready(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY;

    TEST_ASSERT_EQUAL_PTR(1, isFrame1Ready(tlvBuf.readyFrame));
}

void test_isFrame1Ready_given_FRAME1_NOT_READY_should_check_frame1_and_return_0(void)
{
    tlvBuf.readyFrame = FRAME1_NOT_READY;

    TEST_ASSERT_EQUAL_PTR(0, isFrame1Ready(tlvBuf.readyFrame));
}

void test_getNonReadyTLVframe_given_frame_0_non_ready_should_return_bufferPointers0(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY;
    uint8 *ptr;

    ptr	= getNonReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getNonReadyTLVframe_given_frame_1_non_ready_should_return_bufferPointers1(void)
{
    tlvBuf.readyFrame = FRAME1_NOT_READY;
    uint8 *ptr;

    ptr	= getNonReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
}

void test_getNonReadyTLVframe_given_frame_0_and_1_non_ready_should_return_null(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    uint8 *ptr;

    ptr	= getNonReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getNonReadyTLVframe_given_frame_0_and_1_ready_should_return_bufferPointers0(void)
{
    tlvBuf.readyFrame = 0;
    uint8 *ptr;

    ptr	= getNonReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(NULL, ptr);
}

void test_getReadyTLVframe_given_frame_0_ready_should_return_bufferPointers0(void)
{
    tlvBuf.readyFrame = FRAME1_NOT_READY;
    uint8 *ptr;

    ptr	= getReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getReadyTLVframe_given_frame_1_ready_should_return_bufferPointers1(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY;
    uint8 *ptr;

    ptr	= getReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
}

void test_getReadyTLVframe_given_frame_0_and_1_ready_should_return_bufferPointers0(void)
{
    tlvBuf.readyFrame = 0;
    uint8 *ptr;

    ptr	= getReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
}

void test_getReadyTLVframe_given_frame_0_and_1_non_ready_should_return_null(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    uint8 *ptr;

    ptr	= getReadyTLVframe(&tlvBuf);

    TEST_ASSERT_EQUAL_PTR(NULL, ptr);
}

void test_setTLVframe_given_frame_0_non_ready_should_set_frame_0_from_1_to_0(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY;
    uint8 *ptr;

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
    setTLVframe(&tlvBuf, ptr);
    TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_setTLVframe_given_frame_1_non_ready_should_set_frame_1_from_1_to_0(void)
{
    tlvBuf.readyFrame = FRAME1_NOT_READY;
    uint8 *ptr;

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
    setTLVframe(&tlvBuf, ptr);
    TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_setProgrammingMode_should_set_runTargetPin_and_reset_target_to_0_and_send_ACK_to_PC(void)
{
    uartSendByte_Expect(ACK);

    setProgrammingMode();
    TEST_ASSERT_EQUAL(0, runTargetPin);
    TEST_ASSERT_EQUAL(0, resetTarget);
}

void test_setStartRunningMode_should_set_runTargetPin_to_1_and_resetTarget_to_0_and_stop_interrupt(void)
{
    setStartRunningMode();
    TEST_ASSERT_EQUAL(1, runTargetPin);
    TEST_ASSERT_EQUAL(0, resetTarget);
    TEST_ASSERT_EQUAL(0, stopInterrupt);
}

void test_tlvReceiveFSM_should_get_byte_from_PC(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
    uint8 *ptr;
    uint8 frame[] = {0x01,                      //type
                     0x07,                      //length
                     0x04, 0x00, 0x00, 0x00,    //address
                     0x02, 0x0e,                //data
                     0x47};                     //checksum

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);

    uartGetByte_ExpectAndReturn(frame[0]);

    tlvReceiveFSM(&fsm, &tlvBuf, ptr);

    TEST_ASSERT_EQUAL(frame[0], ptr[0]);
}

void test_tlvReceiveFSM_given_first_frame_is_PROGRAMMING_MODE_should_set_programming_mode(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
    uint8 *ptr;
    uint8 frame[] = {PROGRAMMING_MODE};         //type

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);

    uartGetByte_ExpectAndReturn(frame[0]);
    uartSendByte_Expect(ACK);

    tlvReceiveFSM(&fsm, &tlvBuf, ptr);

    TEST_ASSERT_EQUAL(frame[0], ptr[0]);
    TEST_ASSERT_EQUAL(WAIT_FOR_TYPE, fsm.state);
    TEST_ASSERT_EQUAL(0, fsm.i);
    TEST_ASSERT_EQUAL(0, runTargetPin);
    TEST_ASSERT_EQUAL(1, resetTarget);
}

void test_tlvReceiveFSM_given_first_frame_is_START_RUNNING_should_set_start_running_mode(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
    uint8 *ptr;
    uint8 frame[] = {START_RUNNING};            //type

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);

    uartGetByte_ExpectAndReturn(frame[0]);

    tlvReceiveFSM(&fsm, &tlvBuf, ptr);

    TEST_ASSERT_EQUAL(frame[0], ptr[0]);
    TEST_ASSERT_EQUAL(WAIT_FOR_TYPE, fsm.state);
    TEST_ASSERT_EQUAL(0, fsm.i);
    TEST_ASSERT_EQUAL(1, runTargetPin);
    TEST_ASSERT_EQUAL(1, resetTarget);
    TEST_ASSERT_EQUAL(0, stopInterrupt);
}

void test_tlvReceiveFSM_given_frame_should_change_state_from_WAIT_FOR_TYPE_to_WAIT_FOR_LENGTH(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    TLV_FSM fsm = {WAIT_FOR_TYPE, 0};
    uint8 *ptr;
    uint8 frame[] = {0x01,                      //type
                     0x07,                      //length
                     0x04, 0x00, 0x00, 0x00,    //address
                     0x02, 0x0e,                //data
                     0x47};                     //checksum

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);

    uartGetByte_ExpectAndReturn(frame[0]);

    tlvReceiveFSM(&fsm, &tlvBuf, ptr);

    TEST_ASSERT_EQUAL(frame[0], ptr[0]);
    TEST_ASSERT_EQUAL(WAIT_FOR_LENGTH, fsm.state);
    TEST_ASSERT_EQUAL(1, fsm.i);
}

void test_tlvReceiveFSM_given_frame_should_change_state_from_WAIT_FOR_LENGTH_to_WAIT_FOR_VALUE(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    TLV_FSM fsm = {WAIT_FOR_LENGTH, 1};
    uint8 *ptr;
    uint8 frame[] = {0x01,                      //type
                     0x07,                      //length
                     0x04, 0x00, 0x00, 0x00,    //address
                     0x02, 0x0e,                //data
                     0x47};                     //checksum

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);

    uartGetByte_ExpectAndReturn(frame[1]);

    tlvReceiveFSM(&fsm, &tlvBuf, ptr);

    TEST_ASSERT_EQUAL(frame[1], ptr[1]);
    TEST_ASSERT_EQUAL(WAIT_FOR_VALUE, fsm.state);
    TEST_ASSERT_EQUAL(frame[1], fsm.length);
    TEST_ASSERT_EQUAL(2, fsm.i);
}

void test_tlvReceiveFSM_given_frame_should_remain_WAIT_FOR_VALUE_state_if_there_is_still_remaining_frame_to_be_receive(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    TLV_FSM fsm = {WAIT_FOR_VALUE, 6, 7};
    uint8 *ptr;
    uint8 frame[] = {0x01,                      //type
                     0x07,                      //length
                     0x04, 0x00, 0x00, 0x00,    //address
                     0x02, 0x0e,                //data
                     0x47};                     //checksum

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);

    uartGetByte_ExpectAndReturn(frame[6]);

    tlvReceiveFSM(&fsm, &tlvBuf, ptr);

    TEST_ASSERT_EQUAL(frame[6], ptr[6]);
    TEST_ASSERT_EQUAL(WAIT_FOR_VALUE, fsm.state);
    TEST_ASSERT_EQUAL(frame[1], fsm.length);
    TEST_ASSERT_EQUAL(7, fsm.i);
}

void test_tlvReceiveFSM_given_frame_should_state_from_WAIT_FOR_VALUE_to_WAIT_FOR_TYPE_if_there_is_no_remaining_frame_to_be_receive(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    TLV_FSM fsm = {WAIT_FOR_VALUE, 10};
    uint8 *ptr;
    uint8 frame[] = {0x01,                      //type
                     0x09,                      //length
                     0x04, 0x00, 0x00, 0x00,    //address
                     0x02, 0x0e, 0x45, 0x18,    //data
                     0x47};                     //checksum

    ptr	= getNonReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);

    uartGetByte_ExpectAndReturn(frame[10]);

    tlvReceiveFSM(&fsm, &tlvBuf, ptr);

    TEST_ASSERT_EQUAL(frame[10], ptr[10]);
    TEST_ASSERT_EQUAL(WAIT_FOR_TYPE, fsm.state);
    TEST_ASSERT_EQUAL(0, fsm.i);
    TEST_ASSERT_EQUAL(FRAME1_NOT_READY, tlvBuf.readyFrame);
}

void test_SerialISR_given_frame_0_non_ready_should_get_byte_from_the_uart_and_set_frame_0_to_0(void)
{
    int i;
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    uint8 frame[] = {0x01,                      //type
                     0x07,                      //length
                     0x04, 0x00, 0x00, 0x00,    //address
                     0x02, 0x0e,                //data
                     0x47};                     //checksum

    setupUartGetByte(frame, sizeof(frame));

    for(i = 0; i < sizeof(frame); i++)
    {
        SerialISR();
    }

    TEST_ASSERT_EQUAL(FRAME1_NOT_READY, tlvBuf.readyFrame);
}

void test_SerialISR_given_frame_0_and_1_non_ready_should_get_byte_from_the_uart_and_set_both_frame_to_0(void)
{
    int i;
    tlvBuf.readyFrame = FRAME0_NOT_READY | FRAME1_NOT_READY;
    uint8 frame1[] = {0x01,                     //type
                      0x07,                     //length
                      0x04, 0x00, 0x00, 0x00,   //address
                      0x02, 0x0e,               //data
                      0x47};                    //checksum

	uint8 frame2[] = {0x01,                     //type
                      0x09,                     //length
                      0x01, 0x00, 0x30, 0x00,   //address
                      0x45, 0x18, 0x73, 0x27,   //data
                      0x47};                    //checksum

    setupUartGetByte(frame1, sizeof(frame1));

    for(i = 0; i < sizeof(frame1); i++)
    {
        SerialISR();
    }

    TEST_ASSERT_EQUAL(FRAME1_NOT_READY, tlvBuf.readyFrame);

    setupUartGetByte(frame2, sizeof(frame2));

    for(i = 0; i < sizeof(frame2); i++)
    {
        SerialISR();
    }

    TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_SerialISR_should_sent_NACK_to_PC_if_both_frame_ready(void)
{
    int i;
    tlvBuf.readyFrame = 0;

    uartSendByte_Expect(NACK);
    SerialISR();

    TEST_ASSERT_EQUAL(0, tlvBuf.readyFrame);
}

void test_releaseTLVframe_given_frame_0_done_writing_should_release_frame_0_to_1(void)
{
    tlvBuf.readyFrame = FRAME1_NOT_READY;
    uint8 *ptr;

    ptr	= getReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[0], ptr);
    releaseTLVframe(&tlvBuf, ptr);
    TEST_ASSERT_EQUAL(FRAME1_NOT_READY | FRAME0_NOT_READY, tlvBuf.readyFrame);
}

void test_releaseTLVframe_given_frame_1_done_writing_should_release_frame_1_to_1(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY;
    uint8 *ptr;

    ptr	= getReadyTLVframe(&tlvBuf);
    TEST_ASSERT_EQUAL_PTR(&tlvBuffer[80], ptr);
    releaseTLVframe(&tlvBuf, ptr);
    TEST_ASSERT_EQUAL(FRAME1_NOT_READY | FRAME0_NOT_READY, tlvBuf.readyFrame);
}

void test_getAddress_should_return_the_complete_4byte_address_from_the_frame(void)
{
    uint8 *ptr;
    uint8 data[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};

    ptr	= data;
    TEST_ASSERT_EQUAL(0x00300001, getAddress(ptr));
}

void test_getLength_should_return_the_length_of_the_data_from_the_frame(void)
{
    uint8 *ptr;
    uint8 data[] = {0x01, 0x07, 0x01, 0x00, 0x30, 0x00, 0x02, 0xe7, 0xff};

    ptr	= data;
    TEST_ASSERT_EQUAL(2, getLength(ptr));
}

void test_getData_given_frame_should_get_the_data(void)
{
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

void test_isAnyFrameReady_should_return_1_if_frame_1_ready(void)
{
    tlvBuf.readyFrame = FRAME0_NOT_READY;

    TEST_ASSERT_EQUAL(1, isAnyFrameReady(&tlvBuf));
}

void test_isAnyFrameReady_should_return_1_if_frame_0_ready(void)
{
    tlvBuf.readyFrame = FRAME1_NOT_READY;

    TEST_ASSERT_EQUAL(1, isAnyFrameReady(&tlvBuf));
}

void test_isAnyFrameReady_should_return_0_if_both_frame_non_ready(void)
{
    tlvBuf.readyFrame = FRAME1_NOT_READY | FRAME0_NOT_READY;

    TEST_ASSERT_EQUAL(0, isAnyFrameReady(&tlvBuf));
}