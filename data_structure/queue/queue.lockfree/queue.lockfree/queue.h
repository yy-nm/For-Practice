
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <atomic>
#include <cassert>
#include <memory>


#define CAS(t, o, n) __sync_bool_compare_and_swap(t, o, n)
#define FetchAndAdd(t, v) __sync_fetch_and_add(t, v)


template<typename T>
class Queue
{
protected:
	struct Node
	{
		T value;
		struct Node* next;
		size_t refcount;

		Node() : next(nullptr), refcount(0) {}
	};
	struct Node* head;
	struct Node* tail;

	static void GiveBack(struct Node* n) {
		if (nullptr != n) {
			delete n;
		}
	}

	static struct Node* GetNode() {
		return new struct Node();
	}

	void _push(struct Node *n) {
		struct Node *t = nullptr;
		struct Node *next = nullptr;

		do {
			t = tail;
		} while (!CAS(&t->next, nullptr, n));

		while (true) {
			t = tail;
			next = t->next;
			if (next == nullptr)
				continue;
			if (CAS(&tail, t, next))
				break;
		}

		FetchAndAdd(&n->refcount, 1);
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

		while (true)
		{
			t = tail;
			h = head;
			n = head->next;

			if (h == t)
				return false;
			if (n == nullptr)
				return false;

			if (CAS(&head, h, n)) {
				break;
			}
		}

		FetchAndAdd(&n->refcount, 1);
		result = std::move(n->value);

		GiveBack(h);
		return true;
	}
};


#endif