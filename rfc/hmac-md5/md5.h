
/*
* md5.h: accord to rfc 1321 implements md5
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _MD5_H_
#define _MD5_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define LEN_MODER 0x40
#define LEN_MASK (LEN_MODER - 1)
#define LEN_U64 8

	typedef int32_t I32;
	typedef uint32_t U32;
	typedef uint64_t U64;
	typedef uint8_t U8;

	enum md5_error
	{
		MD5_OK = 0,
		MD5_LEN_ERROR = 1,
		MD5_CONTEXT_NULL = 2,
		MD5_OUTPUT_NULL = 3,
	};

	struct md5_context {
		U32 abcd[4];
		U64 len;
		U8 block[LEN_MODER];
		U32 offset;
	};

	int md5_init(struct md5_context *context);
	int md5_update(struct md5_context *context, const char *in, const int len_in);
	int md5_final(struct md5_context *context, char *out, const int len_out);

	int md5(const char *in, const int len_in, char *out, const int len_out);

#ifdef __cplusplus
}
#endif

#endif