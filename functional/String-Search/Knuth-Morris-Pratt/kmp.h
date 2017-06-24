
/*
* kmp.h: implements string search Algorithm accord to a paper named FAST PATTERN MATCHING IN STRINGS
*  Algorithm(https://pdfs.semanticscholar.org/4479/9559a1067e06b5a6bf052f8f10637707928f.pdf) which write by
*  DONALD E. KNUTH, JAMES H. MORRIS AND VAUGHAN R. PRATT
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _KMP_H_
#define _KMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*
 * for use self malloc user, otherwise use stdlib malloc
 * */
typedef void *(*Self_Malloc)(size_t sz);
void kmp_set_malloc_method(Self_Malloc method);

// using kmp seach sub in origin and return first match index, or -1
int kmp_string_search(const char *origin, const char *sub);

#ifdef __cplusplus
}
#endif

#endif // _KMP_H_
