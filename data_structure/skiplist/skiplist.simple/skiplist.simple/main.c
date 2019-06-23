

#include "skiplist.simple.h"

#include <stdio.h>

int main()
{
	struct skiplist list;

	skiplist_init(&list);

	for (int i = 1; i < 10; i++) {
		skiplist_insert(&list, i);
	}

	for (int i = 1; i < 20; i++) {
		printf("find item (%d): result: %d\n", i, skiplist_search(&list, i));
	}

	for (int i = 20; i > 0; i--) {
		skiplist_delete(&list, i);
	}

	for (int i = 1; i < 20; i++) {
		printf("find item (%d): result: %d\n", i, skiplist_search(&list, i));
	}

	skiplist_destroy(&list);
}