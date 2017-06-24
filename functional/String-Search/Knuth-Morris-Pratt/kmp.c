
/*
* kmp.c: implements string search Algorithm accord to a paper named FAST PATTERN MATCHING IN STRINGS
*  Algorithm(https://pdfs.semanticscholar.org/4479/9559a1067e06b5a6bf052f8f10637707928f.pdf) which write by
*  DONALD E. KNUTH, JAMES H. MORRIS AND VAUGHAN R. PRATT
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "kmp.h"

#include <string.h>


static Self_Malloc MALLOC_METHOD = malloc;

void boyer_moore_set_malloc_method(Self_Malloc method)
{
	if (NULL == method)
		MALLOC_METHOD = malloc;
	else
		MALLOC_METHOD = method;
}



static int _search(const char *origin, int len_origin, const char *sub, int len_sub, Self_Malloc mallocer)
{
	if (0 == len_origin || 0 == len_sub || len_sub > len_origin)
		return -1;

	int *next = mallocer(len_sub * sizeof(int));
	if (!next)
		return -2;

	int i;
	int j;
	int tmp;
	next[0] = 0;

	tmp = 0;
	for (i = 1; i < len_sub; i++) {
		while (tmp > 0 && sub[i - 1] != sub[tmp - 1]) {
			tmp = next[tmp - 1];
		}
		tmp = tmp + 1;

		if (sub[i] == sub[tmp - 1]) {
			next[i] = next[tmp - 1];
		} else {
			next[i] = tmp;
		}
	}

	i = j = 0;
	while (i < len_origin && j < len_sub) {
		if (origin[i] != sub[j]) {
			if (j == 0)
				i++;
			else {
				i += j + 1 - next[j];
				i -= j;
				j = 0;
			}
		} else {
			i++;
			j++;
		}
	}

	if (i >= len_origin && j < len_sub)
		return -1;
	return i - len_sub;
}

int kmp_string_search(const char *origin, const char *sub)
{
	return _search(origin, strlen(origin), sub, strlen(sub), MALLOC_METHOD);
}
