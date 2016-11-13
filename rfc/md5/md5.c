/*
* md5.c: accord to rfc 1321 implements md5
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "md5.h"

#include <string.h>

#define LEN_LEFT 56
#define LEN_RESULT 16

#define WORD_A 0x67452301
#define WORD_B 0xefcdab89
#define WORD_C 0x98badcfe
#define WORD_D 0x10325476

#define F(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G(X, Y, Z) (((X) & (Z)) | ((~(Z)) & (Y)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ ((X) | (~(Z))))

unsigned int T[] =
{ 0,
0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x2441453, 0xD8A1E681, 0xE7D3FBC8,
0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x4881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
};

#define ROTATING_SHIFT(x, s) (((x) << (s)) | ((x) >> (32 - (s))))

#define ROUND_1(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + F(b, c, d) + x[k] + T[i]), s)

#define ROUND_2(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + G(b, c, d) + x[k] + T[i]), s)

#define ROUND_3(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + H(b, c, d) + x[k] + T[i]), s)

#define ROUND_4(a, b, c, d, k, s, i, x) \
	a = b + ROTATING_SHIFT((a + I(b, c, d) + x[k] + T[i]), s)

static const U8 _PAD[LEN_MODER] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static void _md5_update(struct md5_context *context, const U8 *block, U32 len_block)
{
	U32 a = context->abcd[0];
	U32 b = context->abcd[1];
	U32 c = context->abcd[2];
	U32 d = context->abcd[3];

	U32 block_index;
	U32 X[16];
	U32 *in;
	U32 aa;
	U32 bb;
	U32 cc;
	U32 dd;

	int j;

	for (block_index = 0; block_index < len_block; block_index += LEN_MODER)
	{
		aa = a;
		bb = b;
		cc = c;
		dd = d;

		in = (U32 *)(block + block_index);
		for (j = 0; j < 16; j++)
			X[j] = in[j];

		ROUND_1(a, b, c, d, 0, 7, 1, X);
		ROUND_1(d, a, b, c, 1, 12, 2, X);
		ROUND_1(c, d, a, b, 2, 17, 3, X);
		ROUND_1(b, c, d, a, 3, 22, 4, X);
		ROUND_1(a, b, c, d, 4, 7, 5, X);
		ROUND_1(d, a, b, c, 5, 12, 6, X);
		ROUND_1(c, d, a, b, 6, 17, 7, X);
		ROUND_1(b, c, d, a, 7, 22, 8, X);
		ROUND_1(a, b, c, d, 8, 7, 9, X);
		ROUND_1(d, a, b, c, 9, 12, 10, X);
		ROUND_1(c, d, a, b, 10, 17, 11, X);
		ROUND_1(b, c, d, a, 11, 22, 12, X);
		ROUND_1(a, b, c, d, 12, 7, 13, X);
		ROUND_1(d, a, b, c, 13, 12, 14, X);
		ROUND_1(c, d, a, b, 14, 17, 15, X);
		ROUND_1(b, c, d, a, 15, 22, 16, X);

		ROUND_2(a, b, c, d, 1, 5, 17, X);
		ROUND_2(d, a, b, c, 6, 9, 18, X);
		ROUND_2(c, d, a, b, 11, 14, 19, X);
		ROUND_2(b, c, d, a, 0, 20, 20, X);
		ROUND_2(a, b, c, d, 5, 5, 21, X);
		ROUND_2(d, a, b, c, 10, 9, 22, X);
		ROUND_2(c, d, a, b, 15, 14, 23, X);
		ROUND_2(b, c, d, a, 4, 20, 24, X);
		ROUND_2(a, b, c, d, 9, 5, 25, X);
		ROUND_2(d, a, b, c, 14, 9, 26, X);
		ROUND_2(c, d, a, b, 3, 14, 27, X);
		ROUND_2(b, c, d, a, 8, 20, 28, X);
		ROUND_2(a, b, c, d, 13, 5, 29, X);
		ROUND_2(d, a, b, c, 2, 9, 30, X);
		ROUND_2(c, d, a, b, 7, 14, 31, X);
		ROUND_2(b, c, d, a, 12, 20, 32, X);

		ROUND_3(a, b, c, d, 5, 4, 33, X);
		ROUND_3(d, a, b, c, 8, 11, 34, X);
		ROUND_3(c, d, a, b, 11, 16, 35, X);
		ROUND_3(b, c, d, a, 14, 23, 36, X);
		ROUND_3(a, b, c, d, 1, 4, 37, X);
		ROUND_3(d, a, b, c, 4, 11, 38, X);
		ROUND_3(c, d, a, b, 7, 16, 39, X);
		ROUND_3(b, c, d, a, 10, 23, 40, X);
		ROUND_3(a, b, c, d, 13, 4, 41, X);
		ROUND_3(d, a, b, c, 0, 11, 42, X);
		ROUND_3(c, d, a, b, 3, 16, 43, X);
		ROUND_3(b, c, d, a, 6, 23, 44, X);
		ROUND_3(a, b, c, d, 9, 4, 45, X);
		ROUND_3(d, a, b, c, 12, 11, 46, X);
		ROUND_3(c, d, a, b, 15, 16, 47, X);
		ROUND_3(b, c, d, a, 2, 23, 48, X);

		ROUND_4(a, b, c, d, 0, 6, 49, X);
		ROUND_4(d, a, b, c, 7, 10, 50, X);
		ROUND_4(c, d, a, b, 14, 15, 51, X);
		ROUND_4(b, c, d, a, 5, 21, 52, X);
		ROUND_4(a, b, c, d, 12, 6, 53, X);
		ROUND_4(d, a, b, c, 3, 10, 54, X);
		ROUND_4(c, d, a, b, 10, 15, 55, X);
		ROUND_4(b, c, d, a, 1, 21, 56, X);
		ROUND_4(a, b, c, d, 8, 6, 57, X);
		ROUND_4(d, a, b, c, 15, 10, 58, X);
		ROUND_4(c, d, a, b, 6, 15, 59, X);
		ROUND_4(b, c, d, a, 13, 21, 60, X);
		ROUND_4(a, b, c, d, 4, 6, 61, X);
		ROUND_4(d, a, b, c, 11, 10, 62, X);
		ROUND_4(c, d, a, b, 2, 15, 63, X);
		ROUND_4(b, c, d, a, 9, 21, 64, X);

		a += aa;
		b += bb;
		c += cc;
		d += dd;
	}

	context->abcd[0] = a;
	context->abcd[1] = b;
	context->abcd[2] = c;
	context->abcd[3] = d;
}


int md5_init(struct md5_context *context)
{
	if (!context)
		return MD5_CONTEXT_NULL;
	context->len = 0;
	context->offset = 0;
	context->abcd[0] = WORD_A;
	context->abcd[1] = WORD_B;
	context->abcd[2] = WORD_C;
	context->abcd[3] = WORD_D;

	return MD5_OK;
}

int md5_update(struct md5_context *context, const char *in, const int len_in)
{
	if (!context)
		return MD5_CONTEXT_NULL;
	if (in) {
		U32 l = len_in;
		context->len += ((U64)l) << 3;
		if (context->offset != 0 && context->offset + l >= LEN_MODER) {
			int cpycount = LEN_MODER - context->offset;
			memcpy(context->block + context->offset, in, cpycount);
			_md5_update(context, context->block, LEN_MODER);
			in += cpycount;
			l -= cpycount;
			context->offset = 0;
		}

		if (context->offset == 0) {
			if (l > LEN_MODER) {
				_md5_update(context, in, l &(~LEN_MASK));
				in += l &(~LEN_MASK);
				l -= l &(~LEN_MASK);
			}
		}

		if (l > 0) {
			memcpy(context->block, in, l);
			context->offset = l;
		}
	}
	else {
		if (context->offset + LEN_U64 + 1 <= LEN_MODER) {
			memcpy(context->block + context->offset, _PAD, LEN_MODER - context->offset - LEN_U64);
		}
		else {
			memcpy(context->block + context->offset, _PAD, LEN_MODER - context->offset);
			_md5_update(context, context->block, LEN_MODER);
			memset(context->block, 0, LEN_MODER);
		}

		U8 *last = context->block + LEN_MODER - LEN_U64;
		U64 l = context->len;
		*last++ = (l) & 0xff;
		*last++ = (l >> 8) & 0xff;
		*last++ = (l >> 16) & 0xff;
		*last++ = (l >> 24) & 0xff;
		*last++ = (l >> 32) & 0xff;
		*last++ = (l >> 40) & 0xff;
		*last++ = (l >> 48) & 0xff;
		*last++ = (l >> 56) & 0xff;

		_md5_update(context, context->block, LEN_MODER);
	}

	return MD5_OK;
}

int md5_final(struct md5_context *context, char *out, const int len_out)
{
	if (!context)
		return MD5_CONTEXT_NULL;
	if (len_out < LEN_RESULT)
		return MD5_LEN_ERROR;
	int ret = md5_update(context, NULL, 0);
	if (ret != MD5_OK)
		return ret;
	U32 abcd;
	int i;
	for (i = 0; i < 4; i++) {
		abcd = context->abcd[i];
		*out++ = (abcd) & 0xff;
		*out++ = (abcd >> 8) & 0xff;
		*out++ = (abcd >> 16) & 0xff;
		*out++ = (abcd >> 24) & 0xff;
	}

	return MD5_OK;
}

int md5(const char *in, const int len_in, char *out, const int len_out)
{
	if (len_out < LEN_RESULT)
		return MD5_LEN_ERROR;

	struct md5_context c;
	md5_init(&c);
	md5_update(&c, in, len_in);
	md5_final(&c, out, len_out);

	return MD5_OK;
}
