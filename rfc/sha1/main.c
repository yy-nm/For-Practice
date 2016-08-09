

#include <stdio.h>
#include <string.h>

#include "sha1.h"


const char *test_vector[] =
{
	"",
	"a",
	"abc",
	"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
	"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
};

#define test_vector_count 5

int tohex(const char *input, const int len_input, char *output, const int len_output)
{
	if (len_output < len_input * 2)
		return -1;

	char low;
	char high;
	int i;
	for (i = 0; i < len_input; i++) {
		high = 0xf & input[i];
		low = (0xf0 & input[i]) >> 4;
		if (low >= 10)
			output[i * 2] = 'a' + low - 10;
		else
			output[i * 2] = '0' + low;
		if (high >= 10)
			output[i * 2 + 1] = 'a' + high - 10;
		else
			output[i * 2 + 1] = '0' + high;
	}

	return 0;
}

int main(void) {
	
	char result[20];
	char show[41];

	int i;
	int ret;
	for (i = 0; i < test_vector_count; i++) {
		ret = sha1(test_vector[i], strlen(test_vector[i]), result, sizeof(result));
		if (ret != SHA1_OK) {
			printf("sha1 error, origin: %s\n", test_vector[i]);
			continue;
		}
		tohex(result, sizeof(result), show, sizeof(show));
		show[40] = '\0';

		printf("SHA1(\"%s\"): %s\n", test_vector[i], show);
	}

	char other[1000000];
	memset(other, 'a', sizeof(other));
	ret = sha1(other, sizeof(other), result, sizeof(result));
	if (ret != SHA1_OK) {
		printf("sha1 error, origin:\n");
		return;
	}
	tohex(result, sizeof(result), show, sizeof(show));
	show[40] = '\0';

	printf("SHA1(): %s\n", show);

	char *last = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno";
	int count = 16777216;
	
	struct sha1_context c;
	sha1_init(&c);
	for (i = 0; i < count; i++) {
		sha1_update(&c, last, strlen(last));
	}
	sha1_final(&c, result, sizeof(result));
	tohex(result, sizeof(result), show, sizeof(show));
	show[40] = '\0';

	printf("SHA1(): %s\n", show);
	return 0;
}
