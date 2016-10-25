
/*
* simle_sort.c: implements several sort algorithm in int64_t array
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

#include "simple_sort.h"


int bubbly_simple_sort(int64_t *items, size_t size)
{
	if (NULL == items)
		return -1;

	int64_t tmp;
	int i;
	int j;
	for (i = 0; i < size; i++) {
		for (j = size - 1; j > i; j--) {
			if (items[j] < items[j - 1]) {
				tmp = items[j - 1];
				items[j - 1] = items[j];
				items[j] = tmp;
			}
		}
	}

	return 0;
}

int selection_simple_sort(int64_t *items, size_t size)
{
	if (NULL == items)
		return -1;

	int64_t tmp;
	int min_index;
	int i;
	int j;

	for (i = 0; i < size; i++) {
		min_index = i;
		for (j = i + 1; j < size; j++) {
			if (items[min_index] > items[j])
				min_index = j;
		}

		if (i != min_index) {
			tmp = items[min_index];
			items[min_index] = items[i];
			items[i] = tmp;
		}
	}

	return 0;
}

static void _merge_split_simple_sort(int64_t *start, size_t totalsize, size_t stepsize, int64_t *newone)
{
	int leftpartsize = stepsize;
	int rightpartsize = totalsize - stepsize;
	int i = 0;
	int j = 0;
	int64_t l, r;
	int ni = 0;
	while (i < leftpartsize && j < rightpartsize) {
		l = start[i];
		r = start[j + stepsize];
		if (l > r) {
			newone[ni] = r;
			j++;
		} else {
			newone[ni] = l;
			i++;
		}
		ni++;
	}
	if (i < leftpartsize) {
		for (i; i < leftpartsize; i++, ni++)
			newone[ni] = start[i];
	} else if(j < rightpartsize) {
		for (j; j < rightpartsize; j++, ni++)
			newone[ni] = start[j + stepsize];
	}
}

int merge_simple_sort(int64_t *items, size_t size)
{
	int i;
	int step;
	int64_t *tmpitemsl = NULL;
	int64_t *tmpitemsr = NULL;
	int64_t *newone = NULL;
	newone = (int64_t *)MALLOC(sizeof(int64_t) * size);
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
			_merge_split_simple_sort(tmpitemsr + i,
							  i + 2 * step < size ? 2 * step : size - i,
							  i + step < size ? step : size - i
							  , tmpitemsl + i);
		}
	}

	if (tmpitemsl != items) {
		for (i = 0; i < size; i++) {
			items[i] = newone[i];
		}
	}
	return 0;
}

static void _quick_split_simple_sort(int64_t start[], size_t size)
{
	if (size <= 1)
		return;
//	int64_t base = start[0];
	int64_t tmp;
	int i = 0;
	int j = size;
	while (i<= j) {

		do {
			++i;
		} while (start[0] > start[i] && i < size);

		do {
			--j;
		} while (start[j] > start[0] && 0 <= j);

		if (i < j) {
			tmp = start[i];
			start[i] = start[j];
			start[j] = tmp;
		} else if (i >= j) {
			break;
		}
	}

	if (j > 0) {
		tmp = start[0];
		start[0] = start[j];
		start[j] = tmp;
	}
	if (j > 0) {
		_quick_split_simple_sort(&start[0], j);
	}
	if (j < size) {
		_quick_split_simple_sort(&start[j + 1], size - j - 1);
	}
}

int quick_simple_sort(int64_t items[], size_t size)
{
	_quick_split_simple_sort(items, size);
}
