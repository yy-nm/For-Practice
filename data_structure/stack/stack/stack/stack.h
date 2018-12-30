

#ifndef _STACK_H_
#define _STACK_H_


#include <cassert>


template <typename T>
class Stack
{
protected:
	struct Node {
		T value;
		struct Node *next;
		Node() : next(nullptr) {}
	};

	struct Node *now;
	size_t len;

	static void GiveBack(struct Node* n) {
		if (nullptr != n)
			delete n;
	}
	static struct Node* GetNode() {
		return new struct Node();
	}
public:

	Stack() : now(nullptr), len(0) {}
	~Stack() {
		struct Node *n = nullptr;
		while (nullptr != now) {
			n = now->next;
			GiveBack(now);
			now = n;
		}
		len = 0;
	}

	bool empty() {
		return len == 0;
	}

	size_t size() {
		return len;
	}

	T top() {
		assert(len > 0);

		return now->value;
	}

	void pop() {
		assert(len > 0);

		len--;
		struct Node *n = now;
		now = now->next;

		GiveBack(n);
	}

	void push(const T& t) {

		struct Node* n = GetNode();
		n->value = t;

		n->next = now;
		now = n;
		len++;
	}

	void push(T&& t) {
		struct Node* n = GetNode();
		n->value = std::move(t);

		n->next = now;
		now = n;
		len++;
	}
};

#endif