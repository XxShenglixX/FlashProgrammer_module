S#include <stdio.h>
#include "unity.h"
#include "CustomTypeAssertion.h"

void assertEqual_tlvDecode(char length, char *expectedData, char *actualData, int line)
{
	int i;

	for(i = 0; i < length; i++)
	{
		TEST_ASSERT_EQUAL(expectedData[i], actualData[i]);
	}
}