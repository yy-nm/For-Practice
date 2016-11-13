#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "sha384.h"
#include "tohex.h"

const int TestSuiteCount = 4;
const char *TestSuite[] = {
	"",
	"abc",
	"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
};
const char *TestSuiteResult[] = {
	"38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b",
	"cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7",
	"3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b",
	"09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039",
};

#define LEN_BUF 48

int main(int argc, char *argv[])
{
	printf("max len: %llu\n", LOW_LEN_MAX);
	char buf[LEN_BUF * 2 + 1];
	char buf_mid[LEN_BUF];
	int i;
	int ret;
	for (i = 0; i < TestSuiteCount; i++) {
		ret = sha384(TestSuite[i], strlen(TestSuite[i]), buf_mid, sizeof(buf_mid));
		if (ret != SHA384_OK) {
			printf("calc \"%s\" sha384 fail, error code: %d\n", TestSuite[i], ret);
			continue;
		}
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		if (strcmp(buf, TestSuiteResult[i]) == 0) {
			printf("calc \"%s\" sha384 success, result: %s\n", TestSuite[i], buf);
		} else {
			printf("calc \"%s\" sha384 success, but not match right one: \n%s\n result: \n%s\n", TestSuite[i], TestSuiteResult[i], buf);
		}
	}

	struct sha384_context c;
	char *repeatchar;
	int repeatcount;

	repeatchar = "a";
	repeatcount = 1000000;
	sha384_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha384_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha384_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA384_OK) {
		printf("calc \"%s\" * %d sha384 fail, error code: %d\n", repeatchar, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		char *result = "9d0e1809716474cb086e834e310a4a1ced149e9c00f248527972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha384 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha384 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	repeatchar = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";
	repeatcount = 16777216;
	sha384_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha384_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha384_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA384_OK) {
		printf("calc \"%s\" * %d sha384 fail, error code: %d\n", repeatchar, repeatcount, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		char *result = "5441235cc0235341ed806a64fb354742b5e5c02a3c5cb71b5f63fb793458d8fdae599c8cd8884943c04f11b31b89f023";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha384 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha384 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	return 0;
}
