
/*
* simle_sort.c: implements several sort algorithm in int64_t array
*
*
* History:
* - 2016-10-11 finish bubbly sort
* - 2016-10-17 finish selection sort
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
