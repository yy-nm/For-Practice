

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>


#define STEP(array, size, t) (t *)((char *)(array) + (size))
#define ASSIGN(l, r, t) (*(t *)(l) = *(t *)(r))


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

unsigned int hash_func_SDBM(void *v, int len)
{
	unsigned int result = 0;
	char *p = (char *)v;
	int i = 0;
	for (i = 0; i < len; i++) {
		result = p[i] + (result << 6) + (result << 16) - result;
	}

	return result;
}

unsigned int hash_func_DJB(void *v, int len)
{
	unsigned int result = 5381;
	int i = 0;
	char *p = (char *)v;
	for (i = 0; i < len; i++) {
		result = ((result << 5) + result) + p[i];
	}

	return result;
}

unsigned int hash_func_DEK(void *v, int len)
{
	unsigned int result = len;
	int i = 0;
	char *p = (char *)v;
	for (i = 0; i < len; i++) {
		result = ((result << 5) ^ (result >> 27)) ^ p[i];
	}

	return result;
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

int search_context_sethash(struct search_context *c, hash_func func)
{
	if (!c)
		return -1;
	if (func)
		c->hash = func;

	return 0;
}

int search_context_setcomparer(struct search_context *c, compare func)
{
	if (!c)
		return -1;
	if (func)
		c->comparer = func;

	return 0;
}


inline static void _copy_value(search_item_t *l, search_item_t *r, size_t sz)
{
	while (sz > 0) {
		if (sz >= sizeof(uint64_t)) {
			ASSIGN(l, r, uint64_t);
			STEP(l, sizeof(uint64_t), search_item_t);
			STEP(r, sizeof(uint64_t), search_item_t);
			sz -= sizeof(uint64_t);
		} else if (sz >= sizeof(uint32_t)) {
			ASSIGN(l, r, uint32_t);
			STEP(l, sizeof(uint32_t), search_item_t);
			STEP(r, sizeof(uint32_t), search_item_t);
			sz -= sizeof(uint32_t);
		} else if(sz >= sizeof(uint16_t)) {
			ASSIGN(l, r, uint16_t);
			STEP(l, sizeof(uint16_t), search_item_t);
			STEP(r, sizeof(uint16_t), search_item_t);
			sz -= sizeof(uint16_t);
		} else {
			ASSIGN(l, r, uint8_t);
			STEP(l, sizeof(uint8_t), search_item_t);
			STEP(r, sizeof(uint8_t), search_item_t);
			sz -= sizeof(uint8_t);
		}
	}
}

static void rehash(struct search_context *c)
{
	int nlen = 2 * c->cap;
	struct search_node **na = NULL;

	if (0 == nlen)
		nlen = 4;
	na = (struct search_node **)MALLOC(nlen * sizeof(struct search_node*));

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
		FREE(c->array);

	c->array = na;
	c->cap = nlen;
}

static int _add(struct search_context *c, search_item_t *data, int sz)
{
	struct search_node *node = (struct search_node *) MALLOC(sizeof(struct search_node));
	if (!node)
		return errno;
	struct search_item_t *value = (struct search_item_t *) MALLOC(sz);
	if (!value)
		return errno;
	_copy_value(value, data, sz);

	node->hashvalue = (*c->hash)(value, sz);
	int index = node->hashvalue % c->cap;
	node->value = value;
	node->sz = sz;
	node->next = c->array[index];
	c->array[index] = node;
	return 0;
}

int search_add(struct search_context *c, search_item_t *data, int sz)
{
	if (!c)
		return -1;
	if (!data)
		return 1;
	if (++c->count > c->cap) {
		rehash(c);
	}

	_add(c, data, sz);
	return 0;
}

int _search(struct search_context *c, search_item_t *target, int sz
			, int *pos, struct search_node **ptarget, struct search_node **ptargetparent)
{
	unsigned int hv = (*c->hash)(target, sz);
	int index = hv % c->cap;
	struct search_node *tmp;
	struct search_node *p = NULL;
	tmp = c->array[index];
	while (tmp) {
		if (!(*c->comparer)(tmp->value, tmp->sz, target, sz)) {
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

int search(struct search_context *c, search_item_t *target, int sz)
{
	if (!c)
		return -1;
	if (!target)
		return 1;

	return _search(c, target, sz, NULL, NULL, NULL);
}

int search_remove(struct search_context *c, search_item_t *target, int sz)
{
	if (!c)
		return -1;
	if (!target)
		return 1;

	int index;
	int ret;
	struct search_node *t = NULL;
	struct search_node *p = NULL;

	ret = _search(c, target, sz, &index, &t, &p);
	if (ret)
		return ret;
	if (p) {
		p->next = t->next;
		FREE(t->value);
		FREE(t);
	} else {
		c->array[index] = t->next;
		FREE(t->value);
		FREE(t);
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
			FREE(tmp->value);
			FREE(tmp);
			tmp = p;
		}
		c->array[i] = NULL;
	}
	c->cap = 0;
	c->count = 0;
	FREE(c->array);
	return 0;
}
