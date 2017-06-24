
/*
* totp.h: accord to rfc 6238 implements TOTP: Time-Based One-Time Password Algorithm
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _TOTP_H_
#define _TOTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint64_t (*gettimestep)();
typedef int (*hmac)(const char *k, const int klen, const char *text, const int textlen, char *out, const int outlen);

int totp(const char *k, const int klen, char *out, const int outlen);
int totp_change_timestep(const char *k, const int klen, gettimestep func, char *out, const int outlen);
int totp_change_hmac(const char *k, const int klen, hmac func, int hmacresultcount, char *out, const int outlen);
int totp_change_timestep_hmac(const char *k, const int klen, gettimestep functime, hmac funchmac, int hmacresultcount
							  , char *out, const int outlen, const int lenindigit);

#ifdef __cplusplus
}
#endif

#endif // _TOTP_H_
