

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
	c->array[c->len - 1] = data;

	return 0;
}

int search(struct search_context *c, int target)
{
	if (!c)
		return -1;

	int i;
	for (i = 0; i < c->len; i++) {
		if (c->array[i] == target)
			return 0;
	}

	return 1;
}

int search_remove(struct search_context *c, int target)
{
	if (!c)
		return -1;

	int i;
	for (i = 0; i < c->len; i++) {
		if (c->array[i] == target)
			break;
	}
	if (i == c->len)
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
