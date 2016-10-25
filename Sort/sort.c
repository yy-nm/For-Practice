
/*
* sort.c: implements several sort algorithm
*
*
* History:
* - 2016-10-11 finish bubbly sort
* - 2016-10-17 finish selection sort
* - 2016-10-25 finish merge sort
* - 2016-10-26 finish quick sort
*
* Authors:
* mardyu<michealyxd@hotmail.com>
*
* Copyright 2016 mardyu<michealyxd@hotmail.com>
* Licensed under the MIT license. See LICENSE file in the project root for full license information.
*/

#include "sort.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

inline int sort_comparer_int32(sort_item_t *l, sort_item_t *r)
{
	int32_t lhv = *(int32_t *)l;
	int32_t rhv = *(int32_t *)r;

	if (lhv < rhv)
		return 1;
	else if (lhv > rhv)
		return -1;
	else
		return 0;
}

inline int sort_comparer_int64(sort_item_t *l, sort_item_t *r)
{
	int64_t lhv = *(int64_t *)l;
	int64_t rhv = *(int64_t *)r;

	if (lhv < rhv)
		return 1;
	else if (lhv > rhv)
		return -1;
	else
		return 0;
}

inline int sort_comparer_float(sort_item_t *l, sort_item_t *r)
{
	float lhv = *(float *)l;
	float rhv = *(float *)r;

	if (lhv < rhv)
		return 1;
	else if (lhv > rhv)
		return -1;
	else
		return 0;
}

inline int sort_comparer_double(sort_item_t *l, sort_item_t *r)
{
	double lhv = *(double *)l;
	double rhv = *(double *)r;

	if (lhv < rhv)
		return 1;
	else if (lhv > rhv)
		return -1;
	else
		return 0;
}


/* sort ***********************************************************/

typedef int (*sort_method)(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare, sort_item_t *tmp);

#define STEP(array, size, t) (t *)((char *)(array) + (size))
#define STEPN(array, size, n, t) (t *)((char *)(array) + (size) * (n))

#define ASSIGN(l, r, t) (*(t *)(l) = *(t *)(r))

#define UNIT_BUFFER_SZ 64

#define SWAP(l, r, tmp, sz) \
	(_copy_value(tmp, l, sz), _copy_value(l, r, sz), _copy_value(r, tmp, sz))



#ifdef USE_SELF_ITEM_TYPE
#define _copy_value(l, r, sz) (*(l) = *(r))
#else
inline static void _copy_value(sort_item_t *l, sort_item_t *r, size_t sz)
{
	while (sz > 0) {
		if (sz >= sizeof(int64_t)) {
			ASSIGN(l, r, int64_t);
			STEP(l, sizeof(int64_t), sort_item_t);
			STEP(r, sizeof(int64_t), sort_item_t);
			sz -= sizeof(int64_t);
		} else if (sz >= sizeof(int32_t)) {
			ASSIGN(l, r, int32_t);
			STEP(l, sizeof(int32_t), sort_item_t);
			STEP(r, sizeof(int32_t), sort_item_t);
			sz -= sizeof(int32_t);
		} else if(sz >= sizeof(int16_t)) {
			ASSIGN(l, r, int16_t);
			STEP(l, sizeof(int16_t), sort_item_t);
			STEP(r, sizeof(int16_t), sort_item_t);
			sz -= sizeof(int16_t);
		} else {
			ASSIGN(l, r, int8_t);
			STEP(l, sizeof(int8_t), sort_item_t);
			STEP(r, sizeof(int8_t), sort_item_t);
			sz -= sizeof(int8_t);
		}
	}
}
#endif

static int _sort (sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare, sort_method m)
{
	if (!items || size <= 0 || unit_size <= 0 || compare == NULL || m == NULL)
		return -1;
	int ret;
#ifndef USE_SELF_ITEM_TYPE
	sort_item_t *tmp;
	char buf[UNIT_BUFFER_SZ];
	char *buf_alloc = NULL;
	if (unit_size <= UNIT_BUFFER_SZ)
		tmp = (sort_item_t *)buf;
	else
	{
		buf_alloc = (char *)MALLOC(unit_size);
		if (buf_alloc == NULL) /* mem is out of running */
			return ;
		tmp = (sort_item_t *)buf_alloc;
	}
#else
	sort_item_t _tmp;
	sort_item_t *tmp = &_tmp;
#endif

	ret = (*m)(items, size, unit_size, compare, tmp);

#ifndef USE_SELF_ITEM_TYPE
	if (buf_alloc) {
		free(buf_alloc);
		buf_alloc = NULL;
	}
#endif

	return ret;
}

/* implements ************************************************/

/* Bubbly **************************************/
static int _bubbly_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare, sort_item_t *tmp)
{
	sort_item_t *l, *r;
	int i;
	int j;
	for (i = 0; i < size; i++)
	{
		for (j = size - 1; j > i; j--)
		{
			l = STEPN(items, unit_size, j, sort_item_t);
			r = STEPN(items, unit_size, j - 1, sort_item_t);
			if ((*compare)(l, r) > 0) {
				_copy_value(tmp, l, unit_size);
				_copy_value(l, r, unit_size);
				_copy_value(r, tmp, unit_size);
			}
		}
	}

	return 0;
}

/* Selection **************************************/
static int _selection_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare, sort_item_t *tmp)
{
	sort_item_t *min, *nor;
	int min_index;
	int i;
	int j;
	for (i = 0; i < size; i++) {
		min = STEPN(items, unit_size, i, sort_item_t);
		min_index = i;
		for (j = i + 1; j < size; j++) {
			tmp = STEPN(items, unit_size, j, sort_item_t);
			if ((*compare)(min, tmp) < 0) {
				min_index = j;
				min = STEPN(items, unit_size, min_index, sort_item_t);
			}
		}

		if (min_index != i) {
			nor = STEPN(items, unit_size, i, sort_item_t);
			min = STEPN(items, unit_size, min_index, sort_item_t);

			_copy_value(tmp, min, unit_size);
			_copy_value(min, nor, unit_size);
			_copy_value(nor, tmp, unit_size);
		}
	}

	return 0;
}

/* Merge **************************************/
static void _merge_split_sort(sort_item_t **start, size_t totalsize, size_t stepsize, sort_item_t **newone
							  , size_t unit_size, sort_comparer compare, sort_item_t *tmp)
{
	int leftpartsize = stepsize;
	int rightpartsize = totalsize - stepsize;
	int i = 0;
	int j = 0;
	sort_item_t *l, *r;
	int ret;
	int ni = 0;
	while (i < leftpartsize && j < rightpartsize) {
		l = STEPN(start, unit_size, i, sort_item_t);
		r = STEPN(start, unit_size, j + stepsize, sort_item_t);
		ret = (*compare)(l, r);
		if (ret < 0) {
			_copy_value(STEPN(newone, unit_size, ni, sort_item_t), r, unit_size);
			j++;
		} else {
			_copy_value(STEPN(newone, unit_size, ni, sort_item_t), l, unit_size);
			i++;
		}
		ni++;
	}
	if (i < leftpartsize) {
		for (i; i < leftpartsize; i++, ni++)
			_copy_value(STEPN(newone, unit_size, ni, sort_item_t),
						STEPN(start, unit_size, i, sort_item_t), unit_size);
	} else if(j < rightpartsize) {
		for (j; j < rightpartsize; j++, ni++)
			_copy_value(STEPN(newone, unit_size, ni, sort_item_t),
						STEPN(start, unit_size, j + stepsize, sort_item_t), unit_size);
	}
}

static int _merge_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare, sort_item_t *tmp)
{
	int i;
	int step;
	sort_item_t **tmpitemsl = NULL;
	sort_item_t **tmpitemsr = NULL;
	sort_item_t **newone = NULL;
	newone = (sort_item_t **)MALLOC(unit_size * size);
	if (!newone) {
		perror("malloc fail");
		return -1;
	}

	for (step = 1; step < size; step += step) {
		if (!tmpitemsl || tmpitemsl == items) {
			tmpitemsl = newone;
			tmpitemsr = items;
		} else {
			tmpitemsl = items;
			tmpitemsr = newone;
		}

		for (i = 0; i < size; i += 2 * step) {
			_merge_split_sort(STEPN(tmpitemsr, unit_size, i, sort_item_t*),
							  i + 2 * step < size ? 2 * step : size - i,
							  i + step < size ? step : size - i
							  , STEPN(tmpitemsl, unit_size, i, sort_item_t*)
							  , unit_size, compare, tmp);
		}
	}

	if (tmpitemsl != items) {
		for (i = 0; i < size; i++) {
			_copy_value(STEPN(items, unit_size, i, sort_item_t), STEPN(newone, unit_size, i, sort_item_t), unit_size);
		}
	}

	return 0;
}

/* Quick **************************************/

static void _quick_split_sort(sort_item_t **start, size_t size, size_t unit_size, sort_comparer compare, sort_item_t *tmp)
{
	if (size <= 1)
		return;
	sort_item_t *base = STEPN(start, unit_size, 0, sort_item_t);
	int i = 0;
	int j = size;
	sort_item_t *l = NULL;
	sort_item_t *r = NULL;
	int retl;
	int retr;
	while (i<= j) {

		do {
			++i;
			l = STEPN(start, unit_size, i, sort_item_t);
			retl = (*compare)(l, base);
		} while (retl > 0 && i < size);

		do {
			--j;
			r = STEPN(start, unit_size, j, sort_item_t);
			retr = (*compare)(base, r);
		} while (retr > 0 && 0 <= j);

		if (i < j) {
			if (retr != 0 || retl != 0)
				SWAP(l, r, tmp, unit_size);
			l = NULL;
			r = NULL;
		} else if (i >= j) {
			break;
		}
	}

	if (j > 0) {
		SWAP(base, r, tmp, unit_size);
	}
	if (j > 0) {
		_quick_split_sort(start, j, unit_size, compare, tmp);
	}
	if (j < size) {
		_quick_split_sort(STEPN(start, unit_size, j + 1, sort_item_t*), size - j - 1, unit_size, compare, tmp);
	}
}

static int _quick_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare, sort_item_t *tmp)
{
	_quick_split_sort(items, size, unit_size, compare, tmp);
	return 0;
}


/* wrapper ************************************************/

int bubbly_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare)
{
	return _sort(items, size, unit_size, compare, _bubbly_sort);
}

int selection_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare)
{
	return _sort(items, size, unit_size, compare, _selection_sort);
}

int merge_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare)
{
	return _sort(items, size, unit_size, compare, _merge_sort);
}

int quick_sort(sort_item_t **items, size_t size, size_t unit_size, sort_comparer compare)
{
	return _sort(items, size, unit_size, compare, _quick_sort);
}
