#include <stdio.h>
#include "unity.h"
#include "CustomTypeAssertion.h"

void assertEqual_tlvDecode(char *expectedData, char *actualData, int line)
{
	int i;
	
	for(i = 0; actualData[i] != '\0'; i++)
	{
		TEST_ASSERT_EQUAL(expectedData[i], actualData[i]);
	}
}