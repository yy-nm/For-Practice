
#include <stdio.h>
#include <string.h>

#include "hmac-md5.h"

#include "tohex.h"

void check(char *k, int klen, char *text, int textlen, char *digst)
{
	char buff[16];
	char hex[33];

	hmac_md5(k, klen, text, textlen, buff, sizeof(buff));
	tohex(buff, sizeof(buff), hex, sizeof(hex));
	hex[32] = '\0';

	if (strcmp(digst, hex) != 0)
	{
		printf("md5 cannot pass digst: %s, md5: %s\n", digst, hex);
		return;
	}

	printf("md5 pass digst: %s, md5: %s\n", digst, hex);
}

/// use rfc 2202 testvector
int main(void)
{
	int i;
	int count = 0;

	char test[16];
	char test_text[] = "Hi There";
	char test_result[] = "9294727a3638bb1c13f48ef8158bfc9d";
	for (i = 0; i < sizeof(test); i++)
	{
		test[i] = 0x0b;
	}

	check(test, sizeof(test), test_text, strlen(test_text), test_result);

	char test2[] = "Jefe";
	char test2_text[] = "what do ya want for nothing?";
	char test2_result[] = "750c783e6ab0b503eaa86e310a5db738";

	check(test2, strlen(test2), test2_text, strlen(test2_text), test2_result);

	char test3[16];
	char test3_text[50];
	char test3_result[] = "56be34521d144c88dbb8c733f0e8b3f6";
	for (i = 0; i < sizeof(test3); i++)
		test3[i] = 0xaa;
	for (i = 0; i < sizeof(test3_text); i++)
		test3_text[i] = 0xdd;

	check(test3, sizeof(test3), test3_text, sizeof(test3_text), test3_result);

	char test4[25];
	char test4_text[50];
	char test4_result[] = "697eaf0aca3a3aea3a75164746ffaa79";
	for (i = 0; i < sizeof(test4); i++)
		test4[i] = i + 1;
	for (i = 0; i < sizeof(test4_text); i++)
		test4_text[i] = 0xcd;

	check(test4, sizeof(test4), test4_text, sizeof(test4_text), test4_result);

	char test5[16];
	char test5_text[] = "Test With Truncation";
	char test5_result[] = "56461ef2342edc00f9bab995690efd4c";
	for (i = 0; i < sizeof(test5); i++)
		test5[i] = 0x0c;

	check(test5, sizeof(test5), test5_text, strlen(test5_text), test5_result);

	char test6[80];
	char test6_text[] = "Test Using Larger Than Block-Size Key - Hash Key First";
	char test6_result[] = "6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd";
	for (i = 0; i < sizeof(test6); i++)
		test6[i] = 0xaa;

	check(test6, sizeof(test6), test6_text, strlen(test6_text), test6_result);

	char test7[80];
	char test7_text[] = "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data";
	char test7_result[] = "6f630fad67cda0ee1fb1f562db3aa53e";
	for (i = 0; i < sizeof(test7); i++)
		test7[i] = 0xaa;

	check(test7, sizeof(test7), test7_text, strlen(test7_text), test7_result);

	return 0;
}