

#ifndef _SKIPLIST_SIMPLE_H_
#define _SKIPLIST_SIMPLE_H_

#include <stdbool.h>

struct skiplist_item {
	struct skiplist_item *next;
	struct skiplist_item *next_level;

	int value;
};

struct skiplist {
	struct skiplist_item *value;
	int len;
};

void skiplist_init(struct skiplist *list);
void skiplist_destroy(struct skiplist *list);

void skiplist_insert(struct skiplist *list, int value);
bool skiplist_search(struct skiplist *list, int target);
void skiplist_delete(struct skiplist *list, int value);

#endif
