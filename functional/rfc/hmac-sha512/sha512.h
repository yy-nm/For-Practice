
/*
* sha512.h: accord to rfc 6234 implements sha512
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _SHA512_H_
#define _SHA512_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

typedef uint64_t U64;
typedef int32_t I32;
typedef uint32_t U32;
typedef uint8_t U8;

#define LEN_BLOCK 128
#define LEN_BLOCK_MASK (LEN_BLOCK - 1)
#define LEN_RESULT 64

#define LEN_IN_BIT (LEN_BLOCK << 3)
#define LOW_LEN_MAX (0xffffffffffffffffull - LEN_IN_BIT)
#define LEN_128 (16)

enum sha512_error
{
	SHA512_OK = 0,
	SHA512_LEN_ERROR = 1,
	SHA512_CONTEXT_NULL = 2,
	SHA512_OUTPUT_NULL = 3,
};

struct sha512_context {
	U64 h[8];
	U64 len[2];
	U8 block[LEN_BLOCK];
	I32 ptr;
};

//struct sha512_context;
// handle with file which has huge size
int sha512_init(struct sha512_context *context);
int sha512_update(struct sha512_context *context, const char *in, const int len_in);
int sha512_final(struct sha512_context *context, char *out, const int len_out);

// handle with some string which can read all in once
int sha512(const char *input, const int len_input, char *output, const int len_output);

#ifdef __cplusplus
}
#endif

#endif // _SHA512_H_
