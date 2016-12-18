
/*
* boyer-moore-string-search.h: implements string search Algorithm accord to a paper named A Fast String Searching
*  Algorithm(http://www.cs.utexas.edu/users/moore/publications/fstrpos.pdf) which write by Robert S. Boyer and J Strother Moore
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _BOYER_MOORE_STRING_SEARCH_H_
#define _BOYER_MOORE_STRING_SEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*
 * for use self malloc user, otherwise use stdlib malloc
 * */
typedef void *(*Self_Malloc)(size_t sz);
void boyer_moore_set_malloc_method(Self_Malloc method);

// using Boyer-moore seach sub in origin and return first match index, or -1
int boyer_moore_string_search(const char *origin, const char *sub);

#ifdef __cplusplus
}
#endif

#endif // _BOYER_MOORE_STRING_SEARCH_H_
