

/*
* sha1.c: accord to rfc 3174 implements sha1
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/


///*
#ifdef __cplusplus
extern "C" {
#endif
//*/

#include "sha1.h"


#include <string.h>

//#define LEN_BLOCK 64
//#define LEN_RESULT 20
//#define LEN_U32 4
//#define LEN_U64 8

static const U8 _PAD[LEN_BLOCK] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const U32 _H[5] =
{
	0x67452301,
	0xEFCDAB89,
	0x98BADCFE,
	0x10325476,
	0xC3D2E1F0,
};

#define f1(B, C, D) (((B) & (C)) | ((~(B)) & (D)))
#define f2(B, C, D) ((B) ^ (C) ^ (D))
#define f3(B, C, D) (((B) & (C)) | ((B) & (D)) | ((C) & (D)))
#define f4(B, C, D) ((B) ^ (C) ^ (D))

#define K1 0x5A827999
#define K2 0x6ED9EBA1
#define K3 0x8F1BBCDC
#define K4 0xCA62C1D6

#define S(X, n) (((X) << (n)) | ((X) >> (32 - n)))

#define R(a, b, c, d, e, tmp) \
	e = d; d = c; c = S(b, 30); b = a; a = tmp;

#define R1(a, b, c, d, e, i, tmp) \
	tmp  = S(a, 5) + f1(b, c, d) + e +W[i] + K1; \
	R(a, b, c, d, e, tmp);

#define R2(a, b, c, d, e, i, tmp) \
	tmp  = S(a, 5) + f2(b, c, d) + e +W[i] + K2; \
	R(a, b, c, d, e, tmp);

#define R3(a, b, c, d, e, i, tmp) \
	tmp  = S(a, 5) + f3(b, c, d) + e +W[i] + K3; \
	R(a, b, c, d, e, tmp);

#define R4(a, b, c, d, e, i, tmp) \
	tmp  = S(a, 5) + f4(b, c, d) + e +W[i] + K4; \
	R(a, b, c, d, e, tmp);

int sha1_init(struct sha1_context *context)
{
	if (!context)
		return SHA1_CONTEXT_NULL;
	context->len = 0;
	context->ptr = 0;
	memcpy(context->h, _H, sizeof(_H));
	return SHA1_OK;
}

void _sha1_update_1(struct sha1_context *context, const char *in, const U32 len_in)
{
	U32 W[80];
	int i;
	const U8 *M = in;

	U32 in_index;

	U32 A;
	U32 B;
	U32 C;
	U32 D;
	U32 E;

	U32 AA = context->h[0];
	U32 BB = context->h[1];
	U32 CC = context->h[2];
	U32 DD = context->h[3];
	U32 EE = context->h[4];
	U32 temp;

	for (in_index = 0; in_index < len_in; in_index += LEN_BLOCK)
	{
		M = in + in_index;
		A = AA;
		B = BB;
		C = CC;
		D = DD;
		E = EE;

		for (i = 0; i < 16; i++) {
			W[i] = (*M++) << 24;
			W[i] |= (*M++) << 16;
			W[i] |= (*M++) << 8;
			W[i] |= (*M++);
		}

		for (i = 16; i < 80; i++) {
			W[i] = S(W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16], 1);
		}

		R1(A, B, C, D, E, 0, temp); R1(A, B, C, D, E, 1, temp);
		R1(A, B, C, D, E, 2, temp); R1(A, B, C, D, E, 3, temp);
		R1(A, B, C, D, E, 4, temp); R1(A, B, C, D, E, 5, temp);
		R1(A, B, C, D, E, 6, temp); R1(A, B, C, D, E, 7, temp);
		R1(A, B, C, D, E, 8, temp); R1(A, B, C, D, E, 9, temp);
		R1(A, B, C, D, E, 10, temp); R1(A, B, C, D, E, 11, temp);
		R1(A, B, C, D, E, 12, temp); R1(A, B, C, D, E, 13, temp);
		R1(A, B, C, D, E, 14, temp); R1(A, B, C, D, E, 15, temp);
		R1(A, B, C, D, E, 16, temp); R1(A, B, C, D, E, 17, temp);
		R1(A, B, C, D, E, 18, temp); R1(A, B, C, D, E, 19, temp);

		R2(A, B, C, D, E, 20, temp); R2(A, B, C, D, E, 21, temp);
		R2(A, B, C, D, E, 22, temp); R2(A, B, C, D, E, 23, temp);
		R2(A, B, C, D, E, 24, temp); R2(A, B, C, D, E, 25, temp);
		R2(A, B, C, D, E, 26, temp); R2(A, B, C, D, E, 27, temp);
		R2(A, B, C, D, E, 28, temp); R2(A, B, C, D, E, 29, temp);
		R2(A, B, C, D, E, 30, temp); R2(A, B, C, D, E, 31, temp);
		R2(A, B, C, D, E, 32, temp); R2(A, B, C, D, E, 33, temp);
		R2(A, B, C, D, E, 34, temp); R2(A, B, C, D, E, 35, temp);
		R2(A, B, C, D, E, 36, temp); R2(A, B, C, D, E, 37, temp);
		R2(A, B, C, D, E, 38, temp); R2(A, B, C, D, E, 39, temp);

		R3(A, B, C, D, E, 40, temp); R3(A, B, C, D, E, 41, temp);
		R3(A, B, C, D, E, 42, temp); R3(A, B, C, D, E, 43, temp);
		R3(A, B, C, D, E, 44, temp); R3(A, B, C, D, E, 45, temp);
		R3(A, B, C, D, E, 46, temp); R3(A, B, C, D, E, 47, temp);
		R3(A, B, C, D, E, 48, temp); R3(A, B, C, D, E, 49, temp);
		R3(A, B, C, D, E, 50, temp); R3(A, B, C, D, E, 51, temp);
		R3(A, B, C, D, E, 52, temp); R3(A, B, C, D, E, 53, temp);
		R3(A, B, C, D, E, 54, temp); R3(A, B, C, D, E, 55, temp);
		R3(A, B, C, D, E, 56, temp); R3(A, B, C, D, E, 57, temp);
		R3(A, B, C, D, E, 58, temp); R3(A, B, C, D, E, 59, temp);

		R4(A, B, C, D, E, 60, temp); R4(A, B, C, D, E, 61, temp);
		R4(A, B, C, D, E, 62, temp); R4(A, B, C, D, E, 63, temp);
		R4(A, B, C, D, E, 64, temp); R4(A, B, C, D, E, 65, temp);
		R4(A, B, C, D, E, 66, temp); R4(A, B, C, D, E, 67, temp);
		R4(A, B, C, D, E, 68, temp); R4(A, B, C, D, E, 69, temp);
		R4(A, B, C, D, E, 70, temp); R4(A, B, C, D, E, 71, temp);
		R4(A, B, C, D, E, 72, temp); R4(A, B, C, D, E, 73, temp);
		R4(A, B, C, D, E, 74, temp); R4(A, B, C, D, E, 75, temp);
		R4(A, B, C, D, E, 76, temp); R4(A, B, C, D, E, 77, temp);
		R4(A, B, C, D, E, 78, temp); R4(A, B, C, D, E, 79, temp);

		AA += A;
		BB += B;
		CC += C;
		DD += D;
		EE += E;
	}

	context->h[0] = AA;
	context->h[1] = BB;
	context->h[2] = CC;
	context->h[3] = DD;
	context->h[4] = EE;
}

int sha1_update(struct sha1_context *context, const char *in, const int len_in)
{
	if (!context)
		return SHA1_CONTEXT_NULL;
	if (in) {
		U32 l = len_in;
		context->len += ((U64)l) << 3;

		if (context->ptr != 0 && context->ptr + l >= LEN_BLOCK) {
			int cpycount = LEN_BLOCK - context->ptr;
			memcpy(context->block + context->ptr, in, cpycount);
			_sha1_update_1(context, context->block, LEN_BLOCK);
			in += cpycount;
			l -= cpycount;
			context->ptr = 0;
		}
		if (context->ptr == 0) {
			if (l >= LEN_BLOCK) {
				_sha1_update_1(context, in, l & (~LEN_BLOCK_MASK));
				in += l & (~LEN_BLOCK_MASK);
				l -= l & (~LEN_BLOCK_MASK);
			}
		}
		if (l > 0) {
			memcpy(context->block, in, l);
			context->ptr = l;
		}
	}
	else {
		if (context->ptr + LEN_U64 + 1 <= LEN_BLOCK) {
			memcpy(context->block + context->ptr, _PAD, LEN_BLOCK - context->ptr - LEN_U64);
		}
		else {
			memcpy(context->block + context->ptr, _PAD, LEN_BLOCK - context->ptr);
			_sha1_update_1(context, context->block, LEN_BLOCK);
			memset(context->block, 0, LEN_BLOCK);
		}

		U8 *last = context->block + LEN_BLOCK - LEN_U64;
		U64 l = context->len;
		*last++ = (l >> 56) & 0xff;
		*last++ = (l >> 48) & 0xff;
		*last++ = (l >> 40) & 0xff;
		*last++ = (l >> 32) & 0xff;
		*last++ = (l >> 24) & 0xff;
		*last++ = (l >> 16) & 0xff;
		*last++ = (l >> 8) & 0xff;
		*last++ = (l)& 0xff;
		_sha1_update_1(context, context->block, LEN_BLOCK);
	}

	return SHA1_OK;
}

int sha1_final(struct sha1_context *context, char *out, const int len_out)
{
	if (!out)
		return SHA1_OUTPUT_NULL;
	if (len_out < LEN_RESULT)
		return SHA1_LEN_ERROR;

	int ret = sha1_update(context, NULL, 0);
	if (ret != SHA1_OK)
		return ret;
	U32 h;
	int i;
	for (i = 0; i < 5; i++) {
		h = context->h[i];
		*out++ = (h >> 24) & 0xff;
		*out++ = (h >> 16) & 0xff;
		*out++ = (h >> 8) & 0xff;
		*out++ = (h) & 0xff;
	}

	return SHA1_OK;
}

int sha1(const char *input, const int len_input, char *output, const int len_output)
{
	if (len_output < LEN_RESULT)
		return SHA1_LEN_ERROR;
	struct sha1_context c;
	sha1_init(&c);
	sha1_update(&c, input, len_input);
	sha1_final(&c, output, len_output);
	return SHA1_OK;
}


#ifdef __cplusplus
}
#endif
