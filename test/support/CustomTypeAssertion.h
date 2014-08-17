#ifndef CustomTypeAssertion_H
#define CustomTypeAssertion_H

#define TEST_ASSERT_EQUAL_tlvDecode(expectedData, actualData)	\
			   assertEqual_tlvDecode(expectedData,				\
									 actualData,				\
									 __LINE__					\
									 );

void assertEqual_tlvDecode(char *expectedData, char *actualData, int line);

#endif // CustomTypeAssertion_H