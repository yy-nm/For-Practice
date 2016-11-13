
/*
* sha224.h: accord to rfc 6234 implements sha224
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _SHA224_H_
#define _SHA224_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

typedef uint64_t U64;
typedef int32_t I32;
typedef uint32_t U32;
typedef uint8_t U8;

#define LEN_BLOCK 64
#define LEN_BLOCK_MASK (LEN_BLOCK - 1)
#define LEN_RESULT 28

#define LEN_IN_BIT (LEN_BLOCK << 3)
#define LEN_64 (8)

enum sha224_error
{
	SHA224_OK = 0,
	SHA224_LEN_ERROR = 1,
	SHA224_CONTEXT_NULL = 2,
	SHA224_OUTPUT_NULL = 3,
};

struct sha224_context {
	U32 h[8];
	U64 len;
	U8 block[LEN_BLOCK];
	I32 ptr;
};

//struct sha224_context;
// handle with file which has huge size
int sha224_init(struct sha224_context *context);
int sha224_update(struct sha224_context *context, const char *in, const int len_in);
int sha224_final(struct sha224_context *context, char *out, const int len_out);

// handle with some string which can read all in once
int sha224(const char *input, const int len_input, char *output, const int len_output);

#ifdef __cplusplus
}
#endif

#endif // _SHA224_H_
