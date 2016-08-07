
#include <string.h>
#include <stdio.h>

#include "base64.h"
#include "TestVectors.h"

int main(void)
{
	int i;
	char buff[128];
	int count = 0;

	// base64_encode
	for (i = 0; i < TestVectorsCount; i++)
	{
		count = base64_encode(TestVectors[i], strlen(TestVectors[i]), buff, sizeof(buff));
		buff[count] = '\0';
		if (strcmp(TestVectorsResult[i], buff) != 0)
		{
			printf("base64_encode cannot pass TestVectors: %s, base64: %s\n", TestVectors[i], buff);
			break;
		}

		printf("base64_encode pass TestVectors: %s, base64: %s\n", TestVectors[i], buff);
	}

	// base64_decode
	for (i = 0; i < TestVectorsCount; i++)
	{
		count = base64_decode(TestVectorsResult[i], strlen(TestVectorsResult[i]), buff, sizeof(buff));
		buff[count] = '\0';

		if (strcmp(TestVectors[i], buff) != 0)
		{
			printf("base64_decode cannot pass TestVectors: %s, base64: %s\n", TestVectorsResult[i], buff);
			break;
		}

		printf("base64_decode pass TestVectors: %s, base64: %s\n", TestVectorsResult[i], buff);
	}

	return 0;
}