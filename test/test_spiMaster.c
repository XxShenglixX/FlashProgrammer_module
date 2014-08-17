#include "unity.h"
#include "Utils.h"
#include "spiMaster.h"
#include "FlashBuffer.h"

#include "p18f4520.h"
#include "mock_spi.h"
#include "mock_delays.h"

#define ACK 0xA5
#define Write 0x11
#define Read 0x22
#define CONFIG 0x33
#define Done 0xFF

void setUp(void)
{
}

void tearDown(void)
{
}

void test_spiConfigureMaster_should_call_CloseSPI_and_OpenSPI()
{
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	spiConfigureMaster();
}

void test_spiSendByte_should_call_WriteSPI_and_Delay10TCyx()
{
	WriteSPI_ExpectAndReturn(0xAA,0);
	Delay10TCYx_Expect(2);

	spiSendByte(0xAA);
}

void test_spiSendData_given_data_count_4_startPoint_0_should_send_each_data_if_ACK_is_received_each_time_and_return_1()
{
	uint8 data[64] = {1,2,3,4,5},count = 4,startPoint = 0;


	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);

	
	WriteSPI_ExpectAndReturn(1,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(2,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(3,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(4,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,spiSendData(data,count,startPoint));
}

void test_spiSendData_give_data_count_3_startPoint_1_receive_not_ACK_at_count_2_should_return_0()
{
	uint8 data[64] = {1,2,3,4,5},count = 3,startPoint = 1;


	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);


	WriteSPI_ExpectAndReturn(2,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(3,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0x5A;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	TEST_ASSERT_EQUAL(0,spiSendData(data,count,startPoint));


}
void test_spiSendData_given_data_count_0_startPoint_5_should_do_nothing_and_return_1()
{
	uint8 data[64] = {1,2,3,4,5},count = 0,startPoint = 5;


	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);

	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,spiSendData(data,count,startPoint));
}



void test_spiReceiveData_given_count_3_should_call_ReadSPI_3_times_and_return_data_from_SSPBUF()
{
	uint8 data[64],count = 3;
	
	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);

	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0x10 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xAA ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,spiReceiveData(data,count));
	TEST_ASSERT_EQUAL(0x10,data[0]);
	TEST_ASSERT_EQUAL(0xAA,data[1]);
	TEST_ASSERT_EQUAL(0xA5,data[2]);
	
}

void test_spiReceiveData_given_0_count_will_do_nothing()
{
	uint8 data[64],count = 0;
	
	/*From spiConfigureMaster*/
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,spiReceiveData(data,count));
}

void test_spiSendCommand_given_Write_should_send_command_and_return_1_if_ACK_is_received()
{	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(Write,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,spiSendCommand(Write));
}

void test_spiSendCommand_given_Read_should_send_command_and_return_0_if_ACK_is_not_received()
{	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x11,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0x5A ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	TEST_ASSERT_EQUAL(0,spiSendCommand(Write));
}

void test_spiSendAddressSegment_given_segment_1_should_send_64_and_return_1_if_ACK_is_received()
{
	FlashBuffer fb;
	fb.segment = 1;
	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x40,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);

	TEST_ASSERT_EQUAL(1,spiSendAddressSegment(&fb));
}

void test_spiSendAddressSegment_given_segment_510_should_send_64_and_return_0_if_ACK_is_received()
{
	FlashBuffer fb;
	fb.segment = 510;
	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x80,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(0x7F,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0x5A ;
	ReadSPI_ExpectAndReturn(SSPBUF);

	TEST_ASSERT_EQUAL(0,spiSendAddressSegment(&fb));
}



void test_spiReceiveStatus_will_return_1_if_received_Done()
{
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = Done ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,spiReceiveStatus());

}

void test_spiReceiveStatus_will_return_0_if_received_not_Done()
{
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = Write ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(0,spiReceiveStatus());

}

void test_readID_will_return_1_if_slave_target_is_found_and_match()
{
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x44,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);

	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0x12 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,readID());

}

void test_readID_will_return_0_if_received_not_0x12()
{
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x44,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);

	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x00,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0x22 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(0,readID());

}

void test_checkACK_will_return_1_if_the_data_passed_in_is_ACK()
{
	uint8 data = ACK ;
	
	TEST_ASSERT_EQUAL(1,checkACK(&data));
}

void test_checkACK_will_return_0_if_the_data_passed_in_is_not_ACK()
{
	uint8 data = 0x01 ;
	
	TEST_ASSERT_EQUAL(0,checkACK(&data));
}

void test_spiSendConfig_and_return_1_if_OK()
{
	uint8 data[64] = {1,2,3,4,5,6};
	uint32 address = 0x312345;

	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);

	
	WriteSPI_ExpectAndReturn(CONFIG,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	
	
	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(0x45,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(0x23,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	WriteSPI_ExpectAndReturn(0x31,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	
	CloseSPI_Expect();
	OpenSPI_Expect(SPI_FOSC_64,MODE_01,SMPMID);
	
	WriteSPI_ExpectAndReturn(1,0);
	Delay10TCYx_Expect(2);
	SSPBUF = 0xA5 ;
	ReadSPI_ExpectAndReturn(SSPBUF);
	
	CloseSPI_Expect();
	Delay10TCYx_Expect(1);
	
	TEST_ASSERT_EQUAL(1,spiSendConfig(address,data));

}