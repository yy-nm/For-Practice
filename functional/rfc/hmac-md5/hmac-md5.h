
/*
* hmac-md5.h: accord to rfc 2104 implements hmac-md5
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/


#ifndef _HMAC_MD5_H_
#define _HMAC_MD5_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


	int hmac_md5(const char *k, size_t klen, const char *text, size_t textlen, char *out, size_t outlen);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // !_HMAC_MD5_H_

