
#include <stdio.h>
#include <string.h>
#include <mem.h>

#include "totp.h"
#include "testvector.h"

#include "hmac-sha1.h"
#include "hmac-sha256.h"
#include "hmac-sha512.h"

uint64_t getstep();

#define HMAC hmac_sha1
#define HMAC_RESULT_LEN 20
#define TIMESTEP getstep
#define TESTVECTOR_RESULT testvector_result_sha1
#define TESTVECTOR_KEY testvector_key_sha1

#define RESULT_LEN 8

#define LEN_BUF 8

int index;

uint64_t getstep()
{
	return testvector_timestep[index] / 30;
}

int main(int argc, char *argv[])
{
	char buf[LEN_BUF + 1];
	int i;
	int ret;

	for (i = 0; i < testvector_count; i++)
	{
		index = i;
		ret = totp_change_timestep_hmac(TESTVECTOR_KEY[i], strlen(TESTVECTOR_KEY[i]), TIMESTEP, HMAC, HMAC_RESULT_LEN, buf, sizeof(buf), RESULT_LEN);
		if (ret) {
			printf("totp fail: error code: %d, testvector key: %s\n", ret, TESTVECTOR_KEY[i]);
			continue;
		}
		buf[LEN_BUF] = '\0';

		if (!strcmp(TESTVECTOR_RESULT[i], buf)) {
			printf("result match: key: %s, step: %llX, result: %s\n", TESTVECTOR_KEY[i], getstep(), TESTVECTOR_RESULT[i]);
		} else {
			printf("result match fail: key: %s, step: %llX, should result: %s, actual: %s\n", TESTVECTOR_KEY[i]
				   , getstep(), TESTVECTOR_RESULT[i], buf);
		}
	}

	return 0;
}
