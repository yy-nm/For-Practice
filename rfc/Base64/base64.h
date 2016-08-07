/*
* base64.h: accord to rfc4648 implements base64
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/


#ifndef _BASE64_H_
#define _BASE64_H_


#ifdef __cplusplus
extern "C" {
#endif

	int base64_encode(const char *in, const int len_in, char *out, const int len_out);
	int base64_url_encode(const char *in, const int len_in, char *out, const int len_out);
	int base64_encode_length(const int len);

	int base64_decode(const char *in, const int len_in, char *out, const int len_out);
	int base64_url_decode(const char *in, const int len_in, char *out, const int len_out);
	int base64_decode_length(const int len);


#ifdef __cplusplus
}
#endif

#endif
