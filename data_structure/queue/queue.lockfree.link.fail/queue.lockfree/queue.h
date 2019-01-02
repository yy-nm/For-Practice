
#ifndef _QUEUE_H_
#define _QUEUE_H_


#include <atomic>
#include <cassert>
#include <memory>


template<typename T>
class Queue
{
protected:
	struct Node
	{
		T value;
		std::atomic<struct Node*> next;
		std::atomic_size_t refcount;

		Node() : next(nullptr), refcount(1) {}
	};
	std::atomic<struct Node*> head;
	std::atomic<struct Node*> tail;
	std::atomic_size_t len;
	std::atomic_size_t popindex;

	static void GiveBack(struct Node* n) {
		if (nullptr != n) {
			n->refcount--;
			if (0 >= n->refcount.load()) {
				delete n;
			}
		}
	}

	static struct Node* GetNode() {
		return new struct Node();
	}

	void Push2Tail(struct Node* n) {
		bool ret = false;
		struct Node* emptypointer = nullptr;
		struct Node* now;
		struct Node* next;
		while (!ret) {
			
			if (0 == len.load()) {
				// in case to avoid pop op during middle.
				if (nullptr != tail.load() || nullptr != head.load()) {
					continue;
				}

				ret = tail.compare_exchange_weak(emptypointer, n);
			}
			else {
				now = tail.load();
				// ?? why must add the statement(nullptr != now->next.load()), or multiple-thread push will has some node which next pointer is nullptr
				//if (nullptr == now)
				if (nullptr == now || nullptr != now->next.load())
					continue;
				ret = now->next.compare_exchange_weak(emptypointer, n);
				//ret = now->next.compare_exchange_strong(emptypointer, n);
				if (!ret) {
					continue;
				}

				tail.store(n);
			}
		}

		if (0 == len.load()) {
			head.store(tail);
		}

		len++;
	}

public:

	Queue() : len(0), head(nullptr), tail(nullptr), popindex(0) {}
	~Queue() {
		struct Node* now = head.load();
		struct Node *next = nullptr;
		while (nullptr != now) {
			next = now->next.load();
			GiveBack(now);
			now = next;
		}

		len = 0;
	}

	size_t size() {
		return len.load();
	}

	bool empty() {
		return 0 == len.load();
	}

	void push(const T& t) {
		struct Node* n = GetNode();
		n->value = t;

		Push2Tail(n);
	}

	void push(T&& t) {
		struct Node* n = GetNode();
		n->value = std::move(t);

		Push2Tail(n);
	}

	T front() {
		assert(len.load() > 0);

		struct Node *n = head.load();
		assert(nullptr != n);
		return n->value;
	}

	T back() {
		assert(len.load() > 0);
		struct Node *n = tail.load();
		assert(nullptr != n);
		return n->value;
	}

	void pop() {
		assert(len.load() > 0);
		len--;
		struct Node* lastone = tail.load();
		assert(len.load() >= 0);

		struct Node* now = nullptr;
		struct Node* next = nullptr;

		size_t index;
		bool ret = false;
		while (!ret) {
			GiveBack(now);
			index = popindex.load();
			now = head.load();
			if (index != popindex.load()) {
				continue;
			}
			now->refcount++;

			if (index != popindex.load()) {
				continue;
			}

			next = now->next;
			if (index != popindex.load()) {
				continue;
			}

			ret = head.compare_exchange_weak(now, next);
			popindex++;
		}
		GiveBack(now);

		if (0 == len.load()) {
			tail.compare_exchange_strong(lastone, nullptr);
		}

		popindex++;
		GiveBack(now);
	}
};


#endif