
/*
* totp.c: accord to rfc 6238 implements TOTP: Time-Based One-Time Password Algorithm
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "totp.h"

#include <sys/time.h>
#include <math.h>

//#include "hmac-sha1.h"
//#include "hmac-sha256.h"
#include "hmac-sha512.h"

#define X (30ull)

#define DEFAULT_LEN 8
#define DEFAULT_MODULO ((int)pow(10, DEFAULT_LEN))

#define HMAC_DEFAULT_FUNC hmac_sha512
#define HMAC_DEFAULT_FUNC_RESULT_LEN 64
#define HMAC_RESULT_LEN_MAX 64

uint64_t _default_gettimestep()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (uint64_t) (tv.tv_sec / X);
}

int _totp(const char *k, const int klen, gettimestep functime, hmac funchmac, int hmacresultcount
		  , char *out, const int outlen, const int lenindigit)
{
	if (!k || !functime || !funchmac || !out || outlen < DEFAULT_LEN)
		return -1;

	char result[HMAC_RESULT_LEN_MAX];
	char buf_step[sizeof(uint64_t)];
	uint64_t step = functime();
	buf_step[0] = (step >> 56) & 0xff;
	buf_step[1] = (step >> 48) & 0xff;
	buf_step[2] = (step >> 40) & 0xff;
	buf_step[3] = (step >> 32) & 0xff;
	buf_step[4] = (step >> 24) & 0xff;
	buf_step[5] = (step >> 16) & 0xff;
	buf_step[6] = (step >> 8) & 0xff;
	buf_step[7] = (step) & 0xff;
	int ret = funchmac(k, klen, buf_step, sizeof(buf_step), result, sizeof(result));
	if (ret)
		return ret;

	int offset = (result[hmacresultcount - 1] & 0xf);
	int bin_code = ((result[offset] & 0x7f) << 24
											  |  (result[offset + 1] & 0xff) << 16
											  |  (result[offset + 2] & 0xff) << 8
											  |  (result[offset + 3] & 0xff));
	bin_code %= ((int) pow(10, lenindigit));
	int i;
	for (i = DEFAULT_LEN - 1; i >= 0; i--) {
		out[i] = bin_code % 10 + '0';
		bin_code /= 10;
	}
	return 0;
}


int totp(const char *k, const int klen, char *out, const int outlen)
{
	return totp_change_timestep_hmac(k, klen, _default_gettimestep, HMAC_DEFAULT_FUNC, HMAC_DEFAULT_FUNC_RESULT_LEN, out, outlen, DEFAULT_LEN);
}

int totp_change_timestep(const char *k, const int klen, gettimestep func, char *out, const int outlen)
{
	return totp_change_timestep_hmac(k, klen, func, HMAC_DEFAULT_FUNC, HMAC_DEFAULT_FUNC_RESULT_LEN, out, outlen, DEFAULT_LEN);
}

int totp_change_hmac(const char *k, const int klen, hmac func, int hmacresultcount, char *out, const int outlen)
{
	return totp_change_timestep_hmac(k, klen, _default_gettimestep, func, hmacresultcount, out, outlen, DEFAULT_LEN);
}

int totp_change_timestep_hmac(const char *k, const int klen, gettimestep functime, hmac funchmac, int hmacresultcount
							  , char *out, const int outlen, const int lenindigit)
{
	return _totp(k, klen, functime, funchmac, hmacresultcount, out, outlen, lenindigit);
}
