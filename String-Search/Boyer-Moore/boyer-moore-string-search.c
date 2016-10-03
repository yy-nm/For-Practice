
/*
* boyer-moore-string-search.c: implements string search Algorithm accord to a paper named A Fast String Searching
* Algorithm which write by Robert S. Boyer and J Strother Moore
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifdef __cplusplus
extern "C" {
#endif


#include "boyer-moore-string-search.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define DELTA1_LENGTH 256
#define DELTA2_STANDBY_LENGTH 256

#define MAX(x, y) (x) > (y) ? (x) : (y)

static Self_Malloc MALLOC_METHOD = malloc;

void boyer_moore_set_malloc_method(Self_Malloc method)
{
	if (NULL == method)
		MALLOC_METHOD = malloc;
	else
		MALLOC_METHOD = method;
}

static void _init_delta1(int delta1[DELTA1_LENGTH], char *sub, int len_sub)
{
	int i;
	for (i = 0; i < DELTA1_LENGTH; i++)
		delta1[i] = len_sub;
	for (i = 0; i < len_sub; i++) {
		delta1[(int)sub[i]] = len_sub - i - 1;
	}
}

static void _init_delta2(int *delta2, char *sub, int len_sub)
{
	int i;
	int j;
	int k;
	int len;
	int len_match;

	for (i = 1; i < len_sub; i++) {
		len_match = len_sub - i;
		j = 0;
		while (++j < len_sub) {

			len = len_sub - j;
			for (k = 0; k < len; k ++) {
				if (k >= len_match) {
					if (sub[len_sub - k - 1] == sub[len - k - 1])
						break;
				} else {
					if (sub[len_sub - k - 1] != sub[len - k - 1])
						break;
				}
			}
			if (k == len)
				break;
		}

		delta2[i - 1] = len_match + j;
	}
	delta2[len_sub - 1] = 1;
}

static void _print_delta(int *delta, int len_delta)
{
	int i;
	for (i = 0; i < len_delta; i++)
		printf("%d ", delta[i]);
}

static int _search(char *origin, int len_origin, char *sub, int len_sub, Self_Malloc mallocer)
{
	if (0 == len_origin || 0 == len_sub || len_sub > len_origin)
		return -1;
	int delta1[DELTA1_LENGTH];
	int delta2_standby[DELTA2_STANDBY_LENGTH];
	int *delta2 = NULL;

	if (len_sub > DELTA2_STANDBY_LENGTH) {
		delta2 = (int *)mallocer(len_sub * sizeof(int));
		assert(delta2 != NULL);
	} else {
		delta2 = delta2_standby;
	}



	_init_delta1(delta1, sub, len_sub);
	_init_delta2(delta2, sub, len_sub);

	int i;
	int j;

	i = len_sub - 1;
	while(i < len_origin) {
		j = len_sub;
		while (--j >= 0) {
			if (origin[i] == sub[j]) {
				i--;
			} else {
				i += MAX(delta1[(int)origin[i]], delta2[j]);
				break;
			}
		}
		if (j < 0)
			return i + 1;
	}

	return -1;
}

int boyer_moore_string_search(char *origin, char *sub)
{
	return _search(origin, strlen(origin), sub, strlen(sub), MALLOC_METHOD);
}


#ifdef __cplusplus
}
#endif
