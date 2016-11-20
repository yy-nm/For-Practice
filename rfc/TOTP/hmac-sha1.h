
/*
* hmac-sha1.h: accord to rfc 2104 implements hmac-sha1
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _HMAC_SHA1_H_
#define _HMAC_SHA1_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


	int hmac_sha1(const char *k, size_t klen, const char *text, size_t textlen, char *out, size_t outlen);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // !_HMAC_SHA1_H_
