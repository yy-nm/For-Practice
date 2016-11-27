

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int search_context_init(struct search_context *c)
{
	if (!c)
		return -1;
	c->len = 0;
	c->cap = 0;
	c->array = NULL;
	return 0;
}

static void _insert(int array[], int len, int data)
{
	int i;
	for (i = len - 1; i >= 0; i--) {
		if (array[i] > data)
			array[i + 1] = array[i];
		else
			break;
	}
	array[i + 1] = data;
}

int search_add(struct search_context *c, int data)
{
	if (!c)
		return -1;
	if (++c->len > c->cap) {
		int nlen = c->cap * 2;
		if (0 == c->cap)
			nlen = 4;
		int *na = (int *)malloc(nlen * sizeof(int));
		if (!na)
			return errno;
		if (0 != c->cap) {
			memcpy(na, c->array, c->cap * sizeof(int));
			free(c->array);
		}
		c->array = na;
		c->cap = nlen;
	}

	_insert(c->array, c->len - 1, data);
	return 0;
}

static int _search(int array[], int len, int target)
{
	if (len <= 0)
		return -1;

	int start = 0;
	int mid = (len + start) / 2;

	while(start <= len) {

		if (array[mid] > target) {
			len = mid - 1;
		} else if (array[mid] < target) {
			start = mid + 1;
		} else {
			return mid;
		}
		mid = (len + start) / 2;
	}

	return -1;
}

int search(struct search_context *c, int target)
{
	if (!c)
		return -1;

	int ret = _search(c->array, c->len, target);
	if (-1 == ret)
		return 1;

	return 0;
}

int search_remove(struct search_context *c, int target)
{
	if (!c)
		return -1;

	int i = _search(c->array, c->len, target);
	if (-1 == i)
		return 1;

	int j;
	for (j = i + 1; j < c->len; j++) {
		c->array[j - 1] = c->array[j];
	}

	c->len --;

	return 0;
}

int search_collect(struct search_context *c)
{
	if (!c)
		return -1;

	if (c->cap > 0)
		free(c->array);
	c->cap = c->len = 0;
	return 0;
}
