

#include "skiplist.simple.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


struct trace_item {
	struct trace_item *next;
	struct skiplist_item *value;
};


struct skiplist_item* _get_skiplist_item()
{
	struct skiplist_item* item = (struct skiplist_item*)malloc(sizeof(struct skiplist_item));
	item->next = item->next_level = NULL;
	item->value = 0;
	return item;
}

void _give_back_skiplist_item(struct skiplist_item *item)
{
	if (NULL != item) {
		free(item);
	}
}

struct trace_item* _get_trace_item()
{
	struct trace_item* item = (struct trace_item*)malloc(sizeof(struct trace_item));
	item->next = NULL;
	item->value = NULL;

	return item;
}

void _give_back_trace_item(struct trace_item *item)
{
	if (NULL != item) {
		free(item);
	}
}


void _destroy_skiplist_same_level(struct skiplist_item *item)
{
	struct skiplist_item *tmp;
	while (NULL != item) {
		tmp = item->next;
		_give_back_skiplist_item(item);
		item = tmp;
	}
}

void _destroy_trace_item(struct trace_item *item)
{
	struct trace_item *tmp;
	while (NULL != item) {
		tmp = item->next;
		_give_back_trace_item(item);
		item = tmp;
	}
}

void skiplist_init(struct skiplist * list)
{
	if (NULL != list) {
		list->value = NULL;
		list->len = 0;
	}
}

void skiplist_destroy(struct skiplist * list)
{
	if (NULL == list || NULL == list->value) {
		return;
	}

	struct skiplist_item *item = list->value;
	struct skiplist_item *tmp;
	list->value = NULL;
	list->len = 0;
	
	while (NULL != item) {
		tmp = item->next_level;
		_destroy_skiplist_same_level(item->next);
		_give_back_skiplist_item(item);
		item = tmp;
	}
}

bool _skiplist_find_target_with_trace(struct skiplist_item *item, struct trace_item *trace, int target)
{
	struct trace_item *t = NULL;

	while (NULL != item) {

		t = _get_trace_item();
		t->value = item;
		t->next = trace->next;
		trace->next = t;

		if (NULL == item->next) {
			item = item->next_level;
			continue;
		}

		if (item->next->value > target) {
			item = item->next_level;
		}
		else if (item->next->value < target) {
			item = item->next;
		}
		else {
			//t = _get_trace_item();
			//t->value = item->next;
			//t->next = trace->next;
			//trace->next = t;
			return true;
		}
	}

	return false;
}

int _get_level(int len)
{
	int i = 1;
	int level = 1;
	srand(time(NULL));
	int half = RAND_MAX / 2;
	while (i < len) {
		i *= 2;
		if (rand() > half) {
			level++;
		}
	}

	return level;
	//return 10 - len;
	//return len;
}

void skiplist_insert(struct skiplist * list, int value)
{
	struct trace_item trace;
	bool find = false;
	int level;

	trace.next = NULL;

	if (NULL == list) {
		return;
	}
	if (NULL == list->value) {
		list->value = _get_skiplist_item();
	}

	find = _skiplist_find_target_with_trace(list->value, &trace, value);
	if (!find) {
		list->len++;
		level = _get_level(list->len);
		struct trace_item *t = trace.next;
		struct skiplist_item *tmp = NULL;
		struct skiplist_item *item_pre = NULL;
		while (level > 0) {
			tmp = _get_skiplist_item();
			tmp->value = value;
			tmp->next_level = item_pre;
			tmp->next = t->value->next;
			t->value->next = tmp;
			
			item_pre = tmp;
			level--;

			while (NULL != t->next) {
				if (t->value == t->next->value->next_level) {
					t = t->next;
					break;
				}
				t = t->next;
			}

			if (NULL == t->next && level > 0) {
				tmp = _get_skiplist_item();
				tmp->next_level = t->value;
				t->value = tmp;
				list->value = tmp;
			}
		}
	}

	_destroy_trace_item(trace.next);
}

bool skiplist_search(struct skiplist * list, int target)
{
	if (NULL == list || NULL == list->value) {
		return false;
	}

	struct skiplist_item *item = list->value;

	while (NULL != item) {
		if (NULL == item->next) {
			item = item->next_level;
			continue;
		}
		if (item->next->value > target) {
			item = item->next_level;
		}
		else if (item->next->value < target) {
			item = item->next;
		}
		else {
			return true;
		}
	}

	return false;
}

void skiplist_delete(struct skiplist * list, int value)
{
	struct trace_item trace;
	bool find = false;

	trace.next = NULL;

	if (NULL == list || NULL == list->value) {
		return;
	}

	find = _skiplist_find_target_with_trace(list->value, &trace, value);
	if (!find) {
		return;
	}

	list->len--;

	struct trace_item *t = trace.next;
	struct skiplist_item *target = t->value->next;
	struct skiplist_item *tmp;
	struct skiplist_item *front = t->value;
	while (NULL != target) {
		while (front->next != target) {
			front = front->next;
		}
		front->next = target->next;
		front = front->next_level;
		tmp = target->next_level;
		_give_back_skiplist_item(target);
		target = tmp;
	}

	_destroy_trace_item(trace.next);

	// do some clean
	target = list->value;
	while (NULL != target && target->next == NULL) {
		tmp = target->next_level;
		_give_back_skiplist_item(target);
		target = tmp;
	}
	list->value = target;
}
