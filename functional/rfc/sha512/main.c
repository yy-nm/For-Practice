#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "sha512.h"
#include "tohex.h"

const int TestSuiteCount = 4;
const char *TestSuite[] = {
	"",
	"abc",
	"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
};
const char *TestSuiteResult[] = {
	"cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
	"ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f",
	"204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445",
	"8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909",
};

int main(int argc, char *argv[])
{
	printf("max len: %llu\n", LOW_LEN_MAX);
	char buf[129];
	char buf_mid[64];
	int i;
	int ret;
	for (i = 0; i < TestSuiteCount; i++) {
		ret = sha512(TestSuite[i], strlen(TestSuite[i]), buf_mid, sizeof(buf_mid));
		if (ret != SHA512_OK) {
			printf("calc \"%s\" sha512 fail, error code: %d\n", TestSuite[i], ret);
			continue;
		}
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[128] = '\0';
		if (strcmp(buf, TestSuiteResult[i]) == 0) {
			printf("calc \"%s\" sha512 success, result: %s\n", TestSuite[i], buf);
		} else {
			printf("calc \"%s\" sha512 success, but not match right one: \n%s\n result: \n%s\n", TestSuite[i], TestSuiteResult[i], buf);
		}
	}

	struct sha512_context c;
	char *repeatchar;
	int repeatcount;

	repeatchar = "a";
	repeatcount = 1000000;
	sha512_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha512_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha512_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA512_OK) {
		printf("calc \"%s\" * %d sha512 fail, error code: %d\n", repeatchar, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[128] = '\0';
		char *result = "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha512 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha512 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	repeatchar = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";
	repeatcount = 16777216;
	sha512_init(&c);
	for (i = 0; i < repeatcount; i++) {
		sha512_update(&c, repeatchar, strlen(repeatchar));
	}
	ret = sha512_final(&c, buf_mid, sizeof(buf_mid));
	if (ret != SHA512_OK) {
		printf("calc \"%s\" * %d sha512 fail, error code: %d\n", repeatchar, repeatcount, ret);
	} else {
		tohex(buf_mid, sizeof(buf_mid), buf, sizeof(buf));
		buf[128] = '\0';
		char *result = "b47c933421ea2db149ad6e10fce6c7f93d0752380180ffd7f4629a712134831d77be6091b819ed352c2967a2e2d4fa5050723c9630691f1a05a7281dbe6c1086";
		if (strcmp(buf, result) == 0) {
			printf("calc \"%s\" * %d sha512 success, result: %s\n", repeatchar, repeatcount, buf);
		} else {
			printf("calc \"%s\" * %d sha512 success, but not match right one: \n%s\n result: \n%s\n", repeatchar, repeatcount, result, buf);
		}
	}

	return 0;
}
