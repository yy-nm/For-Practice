
#ifndef _SHA1_H_
#define _SHA1_H_

///*
#ifdef __cplusplus
extern "C" {
#endif
//*/

// sha1 accord to rfc3174
// author: mardyu<michealyxd@hotmail.com>
// date: 2016-05-12 

#include <stdint.h>

#define LEN_BLOCK 64
#define LEN_RESULT 20
#define LEN_U32 4
#define LEN_U64 8

typedef int32_t I32;
typedef uint32_t U32;
typedef uint64_t U64;
typedef uint8_t U8;

enum sha1_error
{
	SHA1_OK = 0,
	SHA1_LEN_ERROR = 1,
	SHA1_CONTEXT_NULL = 2,
	SHA1_OUTPUT_NULL = 3,
};

struct sha1_context {
	U32 h[5];
	U64 len;
	U8 block[LEN_BLOCK];
	U32 ptr;
};

//struct sha1_context;
// handle with file which has huge size
int sha1_init(struct sha1_context *context);
int sha1_update(struct sha1_context *context, const char *in, const int len_in);
int sha1_final(struct sha1_context *context, char *out, const int len_out);

// handle with some string which can read all in once
int sha1(const char *input, const int len_input, char *output, const int len_output);

///*
#ifdef __cplusplus
}
#endif
//*/
#endif