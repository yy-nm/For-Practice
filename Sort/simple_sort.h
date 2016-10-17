
/*
* simle_sort.h: implements several sort algorithm in int64_t array
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _SIMPLE_SORT_H_
#define _SIMPLE_SORT_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int bubbly_simple_sort(int64_t *items, size_t size);
int selection_simple_sort(int64_t *items, size_t size);

#ifdef __cplusplus
}
#endif

#endif // _SIMPLE_SORT_H_
