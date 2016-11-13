
/*
* sha384.c: accord to rfc 6234 implements sha384
* **note**: rtf 4634 has a error about formula Wt:
*  For t = 16 to 79
*    Wt = SSIG1(W(t-2)) + W(t-7) + SSIG0(t-15) + W(t-16)
* the right formula is:
*  For t = 16 to 79
*    Wt = SSIG1(W(t-2)) + W(t-7) + SSIG0(W(t-15)) + W(t-16)
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "sha384.h"

#include <string.h>


static const U8 _PAD[LEN_BLOCK] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const U64 _H[8] =
{
	0xcbbb9d5dc1059ed8,
	0x629a292a367cd507,
	0x9159015a3070dd17,
	0x152fecd8f70e5939,
	0x67332667ffc00b31,
	0x8eb44a8768581511,
	0xdb0c2e0d64f98fa7,
	0x47b5481dbefa4fa4,
};

#define SHR(x, n) ((x) >> (n))
#define ROTR64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
#define ROTL64(x, n) (((x) << (n)) | ((x) >> (64 - (n))))

#define CH(x, y, z) (((x) & (y)) ^ ((~(x)) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define BSIG0(x) (ROTR64((x), 28) ^ ROTR64((x), 34) ^ ROTR64((x), 39))
#define BSIG1(x) (ROTR64((x), 14) ^ ROTR64((x), 18) ^ ROTR64((x), 41))

#define SSIG0(x) (ROTR64((x), 1) ^ ROTR64((x), 8) ^ SHR((x), 7))
#define SSIG1(x) (ROTR64((x), 19) ^ ROTR64((x), 61) ^ SHR((x), 6))


static const U64 _K[80] =
{
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
	0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
	0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
	0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
	0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
	0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
	0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
	0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
	0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
	0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
	0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
	0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
	0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
	0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
	0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
	0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
	0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817,
};


static inline void LENADDBLOCK(U64 lens[2], U64 add)
{
	if (lens[1] >= LOW_LEN_MAX && lens[1] - LOW_LEN_MAX + add >= LEN_IN_BIT + 1) {
		lens[0]++;
		lens[1] = lens[1] - LOW_LEN_MAX + add - (LEN_IN_BIT + 1);
	} else if (lens[1] < LOW_LEN_MAX && lens[1] > 1 && LOW_LEN_MAX - lens[1] + LEN_IN_BIT + 1 <= add) {
		lens[0]++;
		lens[1] = add - (LOW_LEN_MAX - lens[1] + LEN_IN_BIT + 1);
	} else {
		lens[1] += add;
	}
}


void _sha384_update(struct sha384_context *context, const char *in, const U32 len_in)
{

	U32 in_index;

	U64 W[80];
	U8 *M;
	int i;
	int tmp;

	U64 a;
	U64 b;
	U64 c;
	U64 d;
	U64 e;
	U64 f;
	U64 g;
	U64 h;

	U64 aa;
	U64 bb;
	U64 cc;
	U64 dd;
	U64 ee;
	U64 ff;
	U64 gg;
	U64 hh;

	U64 T1;
	U64 T2;

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
			tmp = i * 8;
			W[i]  = (U64)M[tmp++] << 56;
			W[i] |= (U64)M[tmp++] << 48;
			W[i] |= (U64)M[tmp++] << 40;
			W[i] |= (U64)M[tmp++] << 32;
			W[i] |= (U64)M[tmp++] << 24;
			W[i] |= (U64)M[tmp++] << 16;
			W[i] |= (U64)M[tmp++] << 8;
			W[i] |= (U64)M[tmp++];
		}
		for (i = 16; i < 80; i++) {
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

		for (i = 0; i < 80; i++) {
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

int sha384_init(struct sha384_context *context)
{
	if (!context)
		return SHA384_CONTEXT_NULL;

	context->len[0] = 0;
	context->len[1] = 0;
	context->ptr = 0;
	memcpy(context->h, _H, sizeof(_H));
	return SHA384_OK;
}

int sha384_update(struct sha384_context *context, const char *in, const int len_in)
{
	if (!context)
		return SHA384_CONTEXT_NULL;
	if (in)
	{
		U32 l = len_in;
		LENADDBLOCK(context->len, ((U64)l << 3));
		if (context->ptr != 0 && context->ptr + l >= LEN_BLOCK) {
			int cpycount = LEN_BLOCK - context->ptr;
			memcpy(context->block + context->ptr, in, cpycount);
			_sha384_update(context, context->block, LEN_BLOCK);
			in += cpycount;
			l -= cpycount;
			context->ptr = 0;
		}

		if (context->ptr == 0) {
			if (l >= LEN_BLOCK) {
				_sha384_update(context, in, l &(~LEN_BLOCK_MASK));
				in += l & (~LEN_BLOCK_MASK);
				l = l & LEN_BLOCK_MASK;
			}
		}

		if (l > 0) {
			memcpy(context->block + context->ptr, in, l);
			context->ptr += l;
		}
	} else {
		if (context->ptr + LEN_128 + 1 <= LEN_BLOCK) {
			memcpy(context->block + context->ptr, _PAD, LEN_BLOCK - context->ptr - LEN_128);
		} else {
			memcpy(context->block + context->ptr, _PAD, LEN_BLOCK - context->ptr);
			_sha384_update(context, context->block, LEN_BLOCK);
			memset(context->block, 0, LEN_BLOCK);
		}

		U8 *last = context->block + LEN_BLOCK - LEN_128;
		U64 l;
		l = context->len[0];
		*last++ = (l >> 56) & 0xff;
		*last++ = (l >> 48) & 0xff;
		*last++ = (l >> 40) & 0xff;
		*last++ = (l >> 32) & 0xff;
		*last++ = (l >> 24) & 0xff;
		*last++ = (l >> 16) & 0xff;
		*last++ = (l >> 8) & 0xff;
		*last++ = (l) & 0xff;
		l = context->len[1];
		*last++ = (l >> 56) & 0xff;
		*last++ = (l >> 48) & 0xff;
		*last++ = (l >> 40) & 0xff;
		*last++ = (l >> 32) & 0xff;
		*last++ = (l >> 24) & 0xff;
		*last++ = (l >> 16) & 0xff;
		*last++ = (l >> 8) & 0xff;
		*last++ = (l) & 0xff;
		_sha384_update(context, context->block, LEN_BLOCK);
	}

	return SHA384_OK;
}

int sha384_final(struct sha384_context *context, char *out, const int len_out)
{
	if (!out)
		return SHA384_OUTPUT_NULL;
	if (len_out < LEN_RESULT)
		return SHA384_LEN_ERROR;

	int ret = sha384_update(context, NULL, 0);
	if (ret != SHA384_OK)
		return ret;

	U64 h;
	int i;
	U8 *o = (U8 *)out;
	for (i = 0; i < 6; i++) {
		h = context->h[i];
		*o++ = (h >> 56) & 0xff;
		*o++ = (h >> 48) & 0xff;
		*o++ = (h >> 40) & 0xff;
		*o++ = (h >> 32) & 0xff;
		*o++ = (h >> 24) & 0xff;
		*o++ = (h >> 16) & 0xff;
		*o++ = (h >> 8) & 0xff;
		*o++ = (h) & 0xff;
	}

	return SHA384_OK;
}


int sha384(const char *input, const int len_input, char *output, const int len_output)
{
	if (len_output < LEN_RESULT)
		return SHA384_LEN_ERROR;
	struct sha384_context c;
	sha384_init(&c);
	sha384_update(&c, input, len_input);
	sha384_final(&c, output, len_output);
	return SHA384_OK;
}
