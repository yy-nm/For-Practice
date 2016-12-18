#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "kmp.h"
#include "testvectors.h"

int main(int argc, char *argv[])
{

	int match;
	int i;
	bool allpass = true;
	for (i = 0; i < cTestVectorCount; i++) {
		match = kmp_string_search(cTestVectorOrigin[i], cTestVectorSub[i]);
		if (match == cTestVectorResult[i])
			printf("origin: %s, sub: %s, match at index: %d, pass test\n", cTestVectorOrigin[i]
				   , cTestVectorSub[i], match);
		else {
			printf("origin: %s, sub: %s, match at index: %d, it shoud be at index: %d, not pass test\n"
				   , cTestVectorOrigin[i]
				   , cTestVectorSub[i], match, cTestVectorResult[i]);
			allpass = false;
		}
	}

	if (allpass)
		printf("all test pass!\n");
	else
		printf("some test not pass!\n");
	return 0;
}
