
/*
* sha384.h: accord to rfc 6234 implements sha384
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _SHA384_H_
#define _SHA384_H_

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
#define LEN_RESULT 48

#define LEN_IN_BIT (LEN_BLOCK << 3)
#define LOW_LEN_MAX (0xffffffffffffffffull - LEN_IN_BIT)
#define LEN_128 (16)

enum sha384_error
{
	SHA384_OK = 0,
	SHA384_LEN_ERROR = 1,
	SHA384_CONTEXT_NULL = 2,
	SHA384_OUTPUT_NULL = 3,
};

struct sha384_context {
	U64 h[8];
	U64 len[2];
	U8 block[LEN_BLOCK];
	I32 ptr;
};

//struct sha384_context;
// handle with file which has huge size
int sha384_init(struct sha384_context *context);
int sha384_update(struct sha384_context *context, const char *in, const int len_in);
int sha384_final(struct sha384_context *context, char *out, const int len_out);

// handle with some string which can read all in once
int sha384(const char *input, const int len_input, char *output, const int len_output);

#ifdef __cplusplus
}
#endif

#endif // _SHA384_H_
