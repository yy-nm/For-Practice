#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "sha224.h"
#include "tohex.h"

const int TestSuiteCount = 4;
const char *TestSuite[] = {
	"",
	"abc",
	"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
};
const char *TestSuiteResult[] = {
	"d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f",
	"23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7",
	"75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525",
	"c97ca9a559850ce97a04a96def6d99a9e0e0e2ab14e6b8df265fc0b3",
};

#define LEN_BUF 28

int main(int argc, char *argv[])
{
	char buf[LEN_BUF * 2 + 1];
	char buf_mid[LEN_BUF];
	int i;
	int ret;
	for (i = 0; i < TestSuiteCount; i++) {
		ret = sha224(TestSuite[i], strlen(TestSuite[i]), buf_mid, sizeof(buf_mid));
		if (ret != SHA224_OK) {
			printf("calc \"%s\" sha224 fail, error code: %d\n", TestSuite[i], ret);
			continue;
		}
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		if (strcmp(buf, TestSuiteResult[i]) == 0) {
			printf("calc \"%s\" sha224 success, result: %s\n", TestSuite[i], buf);
		} else {
			printf("calc \"%s\" sha224 success, but not match right one: \n%s\n result: \n%s\n", TestSuite[i], TestSuiteResult[i], buf);
		}
	}

	struct sha224_context c;
	char *repeatchar;
	int repeatcount;

	repeatchar = "a";
	repeatcount = 1000000;
	sha224_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha224_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha224_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA224_OK) {
		printf("calc \"%s\" * %d sha224 fail, error code: %d\n", repeatchar, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		char *result = "20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha224 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha224 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	repeatchar = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";
	repeatcount = 16777216;
	sha224_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha224_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha224_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA224_OK) {
		printf("calc \"%s\" * %d sha224 fail, error code: %d\n", repeatchar, repeatcount, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		char *result = "b5989713ca4fe47a009f8621980b34e6d63ed3063b2a0a2c867d8a85";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha224 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha224 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	return 0;
}
