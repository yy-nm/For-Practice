
/*
* hmac-md5.c: accord to rfc 2104 implements hmac-md5
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "hmac-md5.h"

#include <string.h>

#include "md5.h"

#define L 16
#define B 64

#define H md5

#define H_CONTEXT struct md5_context
#define H_INIT md5_init
#define H_UPDATE md5_update
#define H_FINAL md5_final

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

int hmac_md5(const char *k, size_t klen, const char *text, size_t textlen, char *out, size_t outlen)
{
	if (!k || !text || !out)
		return -1;
	if (outlen < L)
		return -1;

	uint8_t block[B];
	uint8_t mid[L];

	memset((char *)block, 0, B);

	// 1
	if (klen > B) {
		H(k, klen, (char *)block, B);
	}
	else {
		memcpy(block, k, klen);
	}

	// 2
	_xor(block, ipad);

	int ret;
	UNUSED(ret);
	// 3 - 4
	H_CONTEXT c1;
	ret = H_INIT(&c1);
	ret = H_UPDATE(&c1, (char *)block, B);
	ret = H_UPDATE(&c1, text, textlen);
	ret = H_FINAL(&c1, (char *)mid, L);

	// 5
	_xor(block, opad ^ ipad);

	// 6 - 7
	H_CONTEXT c2;
	ret = H_INIT(&c2);
	ret = H_UPDATE(&c2, (char *)block, B);
	ret = H_UPDATE(&c2, (char *)mid, L);
	ret = H_FINAL(&c2, out, outlen);

	return 0;
}
