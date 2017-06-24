
#include <stdio.h>
#include <string.h>

#include "hmac-sha1.h"

#include "tohex.h"

void check(char *k, int klen, char *text, int textlen, char *digst)
{
	char buff[20];
	char hex[41];

	hmac_sha1(k, klen, text, textlen, buff, sizeof(buff));
	tohex(buff, sizeof(buff), hex, sizeof(hex));
	hex[sizeof(hex) - 1] = '\0';

	if (strcmp(digst, hex) != 0)
	{
		printf("sha1 cannot pass digst: %s, sha1: %s\n", digst, hex);
		return;
	}

	printf("sha1 pass digst: %s, sha1: %s\n", digst, hex);
}

/// use rfc 2202 testvector
int main(void)
{
	int i;
//	int count = 0;

	char test[20];
	char test_text[] = "Hi There";
	char test_result[] = "b617318655057264e28bc0b6fb378c8ef146be00";
	for (i = 0; i < sizeof(test); i++)
	{
		test[i] = 0x0b;
	}

	check(test, sizeof(test), test_text, strlen(test_text), test_result);

	char test2[] = "Jefe";
	char test2_text[] = "what do ya want for nothing?";
	char test2_result[] = "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79";

	check(test2, strlen(test2), test2_text, strlen(test2_text), test2_result);

	char test3[20];
	char test3_text[50];
	char test3_result[] = "125d7342b9ac11cd91a39af48aa17b4f63f175d3";
	for (i = 0; i < sizeof(test3); i++)
		test3[i] = 0xaa;
	for (i = 0; i < sizeof(test3_text); i++)
		test3_text[i] = 0xdd;

	check(test3, sizeof(test3), test3_text, sizeof(test3_text), test3_result);

	char test4[25];
	char test4_text[50];
	char test4_result[] = "4c9007f4026250c6bc8414f9bf50c86c2d7235da";
	for (i = 0; i < sizeof(test4); i++)
		test4[i] = i + 1;
	for (i = 0; i < sizeof(test4_text); i++)
		test4_text[i] = 0xcd;

	check(test4, sizeof(test4), test4_text, sizeof(test4_text), test4_result);

	char test5[20];
	char test5_text[] = "Test With Truncation";
	char test5_result[] = "4c1a03424b55e07fe7f27be1d58bb9324a9a5a04";
	for (i = 0; i < sizeof(test5); i++)
		test5[i] = 0x0c;

	check(test5, sizeof(test5), test5_text, strlen(test5_text), test5_result);

	char test6[80];
	char test6_text[] = "Test Using Larger Than Block-Size Key - Hash Key First";
	char test6_result[] = "aa4ae5e15272d00e95705637ce8a3b55ed402112";
	for (i = 0; i < sizeof(test6); i++)
		test6[i] = 0xaa;

	check(test6, sizeof(test6), test6_text, strlen(test6_text), test6_result);

	char test7[80];
	char test7_text[] = "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data";
	char test7_result[] = "e8e99d0f45237d786d6bbaa7965c7808bbff1a91";
	for (i = 0; i < sizeof(test7); i++)
		test7[i] = 0xaa;

	check(test7, sizeof(test7), test7_text, strlen(test7_text), test7_result);

	return 0;
}
