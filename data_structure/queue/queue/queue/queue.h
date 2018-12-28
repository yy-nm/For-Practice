

#ifndef _QUEUE_H_
#define _QUEUE_H_


#include <cassert>


template <typename T>
class Queue {
protected:
	
	struct Item {
		T value;
		struct Item *next;

		Item() : next(nullptr) {}
	};

	struct Item *head;
	struct Item *tail;
	size_t len;
	
	static struct Item* GetItem() {
		return new struct Item();
	}
	static void GivebackItem(struct Item *item) {
		if (nullptr != item) {
			delete item;
		}
	}

public:

	Queue() : head(nullptr), tail(nullptr), len(0) {}
	~Queue() {
		struct Item *item = head;
		struct Item *next = nullptr;
		while (nullptr != item) {
			next = item->next;
			GivebackItem(item);
			item = next;
		}

		len = 0;
	}

	T front() {
		assert(head != nullptr);
		return head->value;
	}

	T back() {
		assert(tail != nullptr);
		return tail->value;
	}
	
	bool empty() {
		return len == 0;
	}
	size_t size() {
		return len;
	}

	void push(const T &v) {
		struct Item *item = GetItem();
		item->value = v;

		if (0 == len) {
			head = tail = item;
		}
		else {
			tail->next = item;
			tail = item;
		}

		len++;
	}
	void push(T&& v) {
		struct Item *item = GetItem();
		item->value = std::move(v);
		
		if (0 == len) {
			head = tail = item;
		}
		else {
			tail->next = item;
			tail = item;
		}

		len++;
	}

	void pop() {
		assert(len > 0);

		len--;
		head = head->next;
		if (0 == len) {
			tail = nullptr;
		}
	}
};

#endif
