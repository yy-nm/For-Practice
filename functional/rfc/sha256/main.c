#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "sha256.h"
#include "tohex.h"

const int TestSuiteCount = 4;
const char *TestSuite[] = {
	"",
	"abc",
	"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
};
const char *TestSuiteResult[] = {
	"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
	"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
	"248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
	"cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1",
};

#define LEN_BUF 32

int main(int argc, char *argv[])
{
	char buf[LEN_BUF * 2 + 1];
	char buf_mid[LEN_BUF];
	int i;
	int ret;
	for (i = 0; i < TestSuiteCount; i++) {
		ret = sha256(TestSuite[i], strlen(TestSuite[i]), buf_mid, sizeof(buf_mid));
		if (ret != SHA256_OK) {
			printf("calc \"%s\" sha256 fail, error code: %d\n", TestSuite[i], ret);
			continue;
		}
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		if (strcmp(buf, TestSuiteResult[i]) == 0) {
			printf("calc \"%s\" sha256 success, result: %s\n", TestSuite[i], buf);
		} else {
			printf("calc \"%s\" sha256 success, but not match right one: \n%s\n result: \n%s\n", TestSuite[i], TestSuiteResult[i], buf);
		}
	}

	struct sha256_context c;
	char *repeatchar;
	int repeatcount;

	repeatchar = "a";
	repeatcount = 1000000;
	sha256_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha256_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha256_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA256_OK) {
		printf("calc \"%s\" * %d sha256 fail, error code: %d\n", repeatchar, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		char *result = "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha256 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha256 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	repeatchar = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";
	repeatcount = 16777216;
	sha256_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha256_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha256_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA256_OK) {
		printf("calc \"%s\" * %d sha256 fail, error code: %d\n", repeatchar, repeatcount, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[LEN_BUF * 2] = '\0';
		char *result = "50e72a0e26442fe2552dc3938ac58658228c0cbfb1d2ca872ae435266fcd055e";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha256 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha256 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	return 0;
}
