
/*
* sha224.c: accord to rfc 6234 implements sha224
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "sha224.h"

#include <string.h>


static const U8 _PAD[LEN_BLOCK] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const U32 _H[8] =
{
	0xc1059ed8,
	0x367cd507,
	0x3070dd17,
	0xf70e5939,
	0xffc00b31,
	0x68581511,
	0x64f98fa7,
	0xbefa4fa4,
};

#define SHR(x, n) ((x) >> (n))
#define ROTR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define CH(x, y, z) (((x) & (y)) ^ ((~(x)) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define BSIG0(x) (ROTR32((x), 2) ^ ROTR32((x), 13) ^ ROTR32((x), 22))
#define BSIG1(x) (ROTR32((x), 6) ^ ROTR32((x), 11) ^ ROTR32((x), 25))

#define SSIG0(x) (ROTR32((x), 7) ^ ROTR32((x), 18) ^ SHR((x), 3))
#define SSIG1(x) (ROTR32((x), 17) ^ ROTR32((x), 19) ^ SHR((x), 10))


static const U32 _K[80] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};



void _sha224_update(struct sha224_context *context, const U8 *in, const size_t len_in)
{

	size_t in_index;

	U32 W[64];
	U8 *M;
	int i;
	int tmp;

	U32 a;
	U32 b;
	U32 c;
	U32 d;
	U32 e;
	U32 f;
	U32 g;
	U32 h;

	U32 aa;
	U32 bb;
	U32 cc;
	U32 dd;
	U32 ee;
	U32 ff;
	U32 gg;
	U32 hh;

	U32 T1;
	U32 T2;

	aa = context->h[0];
	bb = context->h[1];
	cc = context->h[2];
	dd = context->h[3];
	ee = context->h[4];
	ff = context->h[5];
	gg = context->h[6];
	hh = context->h[7];

	for (in_index = 0; in_index < len_in; in_index += LEN_BLOCK)
	{
		M = (U8 *)(in + in_index);

		for (i = 0; i < 16; i++) {
			tmp = i * 4;

			W[i]  = (U32)M[tmp++] << 24;
			W[i] |= (U32)M[tmp++] << 16;
			W[i] |= (U32)M[tmp++] << 8;
			W[i] |= (U32)M[tmp++];
		}
		for (i = 16; i < 64; i++) {
			W[i] = SSIG1(W[i - 2]) + W[i - 7] + SSIG0(W[i - 15]) + W[i - 16];
		}

		a = aa;
		b = bb;
		c = cc;
		d = dd;
		e = ee;
		f = ff;
		g = gg;
		h = hh;

		for (i = 0; i < 64; i++) {
			T1 = h + BSIG1(e) + CH(e, f, g) + _K[i] + W[i];
			T2 = BSIG0(a) + MAJ(a, b ,c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		aa += a;
		bb += b;
		cc += c;
		dd += d;
		ee += e;
		ff += f;
		gg += g;
		hh += h;
	}

	context->h[0] = aa;
	context->h[1] = bb;
	context->h[2] = cc;
	context->h[3] = dd;
	context->h[4] = ee;
	context->h[5] = ff;
	context->h[6] = gg;
	context->h[7] = hh;
}

int sha224_init(struct sha224_context *context)
{
	if (!context)
		return SHA224_CONTEXT_NULL;

	context->len = 0;
	context->ptr = 0;
	memcpy(context->h, _H, sizeof(_H));
	return SHA224_OK;
}

int sha224_update(struct sha224_context *context, const char *in, const size_t len_in)
{
	if (!context)
		return SHA224_CONTEXT_NULL;
	if (in)
	{
		size_t l = len_in;
		context->len += ((U64)l << 3);
		if (context->ptr != 0 && context->ptr + l >= LEN_BLOCK) {
			int cpycount = LEN_BLOCK - context->ptr;
			memcpy(context->block + context->ptr, in, cpycount);
			_sha224_update(context, context->block, LEN_BLOCK);
			in += cpycount;
			l -= cpycount;
			context->ptr = 0;
		}

		if (context->ptr == 0) {
			if (l >= LEN_BLOCK) {
				_sha224_update(context, (U8 *)in, l &(~LEN_BLOCK_MASK));
				in += l & (~LEN_BLOCK_MASK);
				l = l & LEN_BLOCK_MASK;
			}
		}

		if (l > 0) {
			memcpy(context->block + context->ptr, in, l);
			context->ptr += l;
		}
	} else {
		if (context->ptr + LEN_64 + 1 <= LEN_BLOCK) {
			memcpy(context->block + context->ptr, _PAD, LEN_BLOCK - context->ptr - LEN_64);
		} else {
			memcpy(context->block + context->ptr, _PAD, LEN_BLOCK - context->ptr);
			_sha224_update(context, context->block, LEN_BLOCK);
			memset(context->block, 0, LEN_BLOCK);
		}

		U8 *last = context->block + LEN_BLOCK - LEN_64;
		U64 l;
		l = context->len;
		*last++ = (l >> 56) & 0xff;
		*last++ = (l >> 48) & 0xff;
		*last++ = (l >> 40) & 0xff;
		*last++ = (l >> 32) & 0xff;
		*last++ = (l >> 24) & 0xff;
		*last++ = (l >> 16) & 0xff;
		*last++ = (l >> 8) & 0xff;
		*last++ = (l) & 0xff;

		_sha224_update(context, context->block, LEN_BLOCK);
	}

	return SHA224_OK;
}

int sha224_final(struct sha224_context *context, char *out, const int len_out)
{
	if (!out)
		return SHA224_OUTPUT_NULL;
	if (len_out < LEN_RESULT)
		return SHA224_LEN_ERROR;

	int ret = sha224_update(context, NULL, 0);
	if (ret != SHA224_OK)
		return ret;

	U64 h;
	int i;
	U8 *o = (U8 *)out;
	for (i = 0; i < 7; i++) {
		h = context->h[i];

		*o++ = (h >> 24) & 0xff;
		*o++ = (h >> 16) & 0xff;
		*o++ = (h >> 8) & 0xff;
		*o++ = (h) & 0xff;
	}

	return SHA224_OK;
}


int sha224(const char *input, const size_t len_input, char *output, const int len_output)
{
	if (len_output < LEN_RESULT)
		return SHA224_LEN_ERROR;
	struct sha224_context c;
	sha224_init(&c);
	sha224_update(&c, input, len_input);
	sha224_final(&c, output, len_output);
	return SHA224_OK;
}
