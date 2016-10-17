
/*
* sort.h: implements several sort algorithm
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#ifndef _SORT_H_
#define _SORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define USE_SELF_ITEM_TYPE

#ifndef USE_SELF_ITEM_TYPE
typedef void sort_item_t;
#else
typedef int64_t sort_item_t;
#endif

typedef int (*sort_comparer) (sort_item_t *l, sort_item_t *r);

int sort_comparer_int32(sort_item_t *l, sort_item_t *r);
int sort_comparer_int64(sort_item_t *l, sort_item_t *r);
int sort_comparer_float(sort_item_t *l, sort_item_t *r);
int sort_comparer_double(sort_item_t *l, sort_item_t *r);

int bubbly_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare);
int selection_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare);

#ifdef __cplusplus
}
#endif

#endif // _SORT_H_
