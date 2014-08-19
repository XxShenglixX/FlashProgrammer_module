#ifndef CustomTypeAssertion_H
#define CustomTypeAssertion_H

#define TEST_ASSERT_EQUAL_tlvDecode(length, expectedData, actualData)	\
			   assertEqual_tlvDecode(length,							\
									 expectedData,						\
									 actualData,						\
									 __LINE__							\
									 );

void assertEqual_tlvDecode(char length, char *expectedData, char *actualData, int line);

#endif // CustomTypeAssertion_H