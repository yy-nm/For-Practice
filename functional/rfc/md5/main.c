

#include <stdio.h>

#include "md5.h"
#include "tohex.h"
#include "TestSuite.h"


int main(void)
{
	int i;
	char buff[16];
	char hex[33];
	int count = 0;

	// md5
	for (i = 0; i < TestSuiteCount; i++)
	{
		md5(TestSuite[i], strlen(TestSuite[i]), buff, sizeof(buff));
		tohex(buff, sizeof(buff), hex, sizeof(hex));
		hex[32] = '\0';

		if (strcmp(TestSuiteResult[i], hex) != 0)
		{
			printf("md5 cannot pass TestSuite: %s, md5: %s\n", TestSuite[i], hex);
			break;
		}

		printf("md5 pass TestSuite: %s, md5: %s\n", TestSuite[i], hex);
	}

	return 0;
}