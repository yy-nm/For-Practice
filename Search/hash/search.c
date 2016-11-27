

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// use c# string hash func
static unsigned int _default_hash_func(void *v, int len)
{
	unsigned int hash1 = 5381;
	unsigned int hash2 = hash1;
	char *p = (char *)v;
	int i;
	for (i = 0; i < len; i+= 2) {
		hash1 = ((hash1 << 5) + hash1) ^ p[i];
		if (i + 1 >= len)
			break;
		hash2 = ((hash2 << 5) + hash2) ^ p[i + 1];
	}

	return hash1 + (hash2 * 1566083941);
}

static int _default_compare(void *l, int llen, void *r, int rlen)
{
	int ret;
	int len = llen;
	if (len > rlen)
		len = rlen;
	ret = memcmp(l, r, len);
	if (ret)
		return ret;
	if (llen == rlen)
		return 0;
	else if(llen > rlen)
		return -1;
	else
		return 1;
}


int search_context_init(struct search_context *c)
{
	if (!c)
		return -1;
	c->count = 0;
	c->cap = 0;
	c->array = NULL;
	c->comparer = _default_compare;
	c->hash = _default_hash_func;
	return 0;
}

static void rehash(struct search_context *c)
{
	int nlen = 2 * c->cap;
	struct search_node **na = NULL;

	if (0 == nlen)
		nlen = 4;
	na = (struct search_node **)malloc(nlen * sizeof(struct search_node*));

	int i;
	for (i = 0; i < nlen; i++) {
		na[i] = NULL;
	}

	struct search_node *tmp;
	struct search_node *p;
	int index;
	for (i = 0; i < c->cap; i++) {
		tmp = c->array[i];
		while(tmp) {
			p = tmp->next;
			index = tmp->hashvalue % nlen;

			tmp->next = na[index];
			na[index] = tmp;

			tmp = p;
		}
		c->array[i] = NULL;
	}
	if (c->cap > 0)
		free(c->array);

	c->array = na;
	c->cap = nlen;
}

static int _add(struct search_context *c, int data)
{
	unsigned int hv = (*c->hash)(&data, sizeof(data));
	struct search_node *node = (struct search_node *) malloc(sizeof(struct search_node));
	if (!node)
		return errno;

	int index = hv % c->cap;
	node->value = data;
	node->hashvalue = hv;
	node->next = c->array[index];
	c->array[index] = node;
	return 0;
}

int search_add(struct search_context *c, int data)
{
	if (!c)
		return -1;
	if (++c->count > c->cap) {
		rehash(c);
	}

	_add(c, data);
	return 0;
}

int _search(struct search_context *c, int target, int *pos, struct search_node **ptarget, struct search_node **ptargetparent)
{
	unsigned int hv = (*c->hash)(&target, sizeof(target));
	int index = hv % c->cap;
	struct search_node *tmp;
	struct search_node *p = NULL;
	tmp = c->array[index];
	while (tmp) {
		if (!(*c->comparer)(&tmp->value, sizeof(tmp->value), &target, sizeof(target))) {
			if (pos)
				*pos = index;
			if (ptarget)
				*ptarget = tmp;
			if (ptargetparent)
				*ptargetparent = p;
			return 0;
		}
		p = tmp;
		tmp = tmp->next;
	}

	return 1;
}

int search(struct search_context *c, int target)
{
	if (!c)
		return -1;

	return _search(c, target, NULL, NULL, NULL);
}

int search_remove(struct search_context *c, int target)
{
	if (!c)
		return -1;

	int index;
	int ret;
	struct search_node *t = NULL;
	struct search_node *p = NULL;

	ret = _search(c, target, &index, &t, &p);
	if (ret)
		return ret;
	if (p) {
		p->next = t->next;
		free(t);
	} else {
		c->array[index] = t->next;
		free(t);
	}
	return 0;
}

int search_collect(struct search_context *c)
{
	if (!c)
		return -1;

	struct search_node *tmp;
	struct search_node *p;
	int i;
	for (i = 0; i < c->cap; i++) {
		tmp = c->array[i];
		while(tmp) {
			p = tmp->next;
			free(tmp);
			tmp = p;
		}
		c->array[i] = NULL;
	}
	c->cap = 0;
	c->count = 0;
	free(c->array);
	return 0;
}
