

/*
* hmac-sha256.c: accord to rfc 2104, rfc 4231 implements hmac-sha256
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "hmac-sha256.h"

#include <string.h>

#include "sha256.h"

#define L 32
#define B 64

#define H sha256

#define H_CONTEXT struct sha256_context
#define H_INIT sha256_init
#define H_UPDATE sha256_update
#define H_FINAL sha256_final

#define ipad 0x36
#define opad 0x5c

#define UNUSED(x) ((void) (x))

static void _xor(uint8_t bstring[B], uint8_t pad)
{
	int i;
	for (i = 0; i < B; i++)
	{
		bstring[i] ^= pad;
	}
}

int hmac_sha256(const char *k, size_t klen, const char *text, size_t textlen, char *out, size_t outlen)
{
	if (!k || !text || !out)
		return -1;
	if (outlen < L)
		return -1;

	char block[B];
	char mid[L];

	memset(block, 0, B);

	// 1
	if (klen > B) {
		H(k, klen, block, B);
	}
	else {
		memcpy(block, k, klen);
	}

	// 2
	_xor((uint8_t *)block, ipad);

	int ret;
	UNUSED(ret);
	// 3 - 4
	H_CONTEXT c1;
	ret = H_INIT(&c1);
	ret = H_UPDATE(&c1, block, B);
	ret = H_UPDATE(&c1, text, textlen);
	ret = H_FINAL(&c1, mid, L);

	// 5
	_xor((uint8_t *)block, opad ^ ipad);

	// 6 - 7
	H_CONTEXT c2;
	ret = H_INIT(&c2);
	ret = H_UPDATE(&c2, block, B);
	ret = H_UPDATE(&c2, mid, L);
	ret = H_FINAL(&c2, out, outlen);

	return 0;
}
