
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
		volatile std::atomic<struct Node*> next;
		std::atomic_size_t refcount;

		Node() : next(nullptr), refcount(0) {}
	};
	volatile std::atomic<struct Node*> head;
	volatile std::atomic<struct Node*> tail;

	static void GiveBack(struct Node* n) {
		if (nullptr != n) {
			delete n;
		}
	}

	static struct Node* GetNode() {
		return new struct Node();
	}

	void _push(struct Node *n) {
		struct Node * t = nullptr;
		struct Node * next = nullptr;
		struct Node* null_p = nullptr;
		size_t ref = 0;

		while (true) {
			t = tail.load();
			next = t->next.load();
			null_p = nullptr;
			if (next == n)
				break;
			t->next.compare_exchange_weak(null_p, n);
		}

		while (true) {
			t = tail.load();
			next = t->next.load();
			
			if (next == nullptr)
				continue;
			if (tail.compare_exchange_weak(t, next)) {
				ref = next->refcount.fetch_add(1);
				if (ref + 1 == next->refcount)
					break;
			}
		}

		n->refcount.fetch_add(1);
	}

public:

	Queue() {
		struct Node *n = GetNode();
		head = tail = n;
	}
	~Queue() {
		struct Node* now = head;
		struct Node *next = nullptr;
		while (nullptr != now) {
			next = now->next;
			GiveBack(now);
			now = next;
		}

		head = nullptr;
		tail = nullptr;
	}

	void push(const T& t) {
		struct Node* n = GetNode();
		n->value = t;
		_push(n);
	}

	void push(T&& t) {
		struct Node* n = GetNode();
		n->value = t;
		_push(n);
	}

	bool pop(T& result) {
		struct Node *n = nullptr;
		struct Node *t = nullptr;
		struct Node *h = nullptr;
		struct Node *tmp = nullptr;
		size_t ref = 0;

		while (true)
		{
			h = head.load();
			t = tail.load();
			n = h->next.load();
			
			if (h == t)
				return false;
			if (n == nullptr)
				return false;
			if (h != head)
				continue;
			tmp = h;
			if (head.compare_exchange_weak(h, n)) {
				ref = h->refcount.fetch_add(1);
				if (ref + 1 == h->refcount && tmp == h)
					break;
			}

		}

		n->refcount.fetch_add(1);
		result = std::move(n->value);

		GiveBack(h);
		return true;
	}
};


#endif