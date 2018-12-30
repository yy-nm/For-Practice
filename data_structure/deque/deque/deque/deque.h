

#ifndef _DEQUE_H_
#define _DEQUE_H_


#include <cassert>


template<typename T>
class Deque 
{
protected:

	struct Node {
		T value;
		struct Node *next;
		struct Node *pre;
		Node() : next(nullptr), pre(nullptr) {}
	};

	struct Node *head;
	struct Node *tail;
	size_t len;

	static void GiveBack(struct Node* n) {
		if (nullptr != n)
			delete n;
	}

	static struct Node* GetNode() {
		return new struct Node();
	}

	void push_back_node(struct Node* n) {
		n->pre = tail;
		if (0 == len) {
			head = tail = n;
			n->pre = nullptr;
		}
		else {
			tail->next = n;
			tail = n;
		}

		len++;
	}

	void push_front_node(struct Node* n) {
		n->next = head;

		if (0 == len) {
			tail = head = n;
			n->next = nullptr;
		}
		else {
			head->pre = n;
			head = n;
		}
		
		len++;
	}

	void insert_node(int pos, struct Node* n) {

		pos = len - pos;

		struct Node* pre = tail;

		while (pos-- > 0) {
			pre = pre->pre;
		}

		if (0 == len) {
			head = tail = n;
		}
		else {
			n->pre = pre;
			if (nullptr != pre) {
				n->next = pre->next;
				pre->next = n;
			}
			else {
				n->next = head;
				head = n;
			}
			
			if (nullptr != n->next) {
				n->next->pre = n;
			}
			else {
				tail = n;
			}
		}

		len++;
	}
public:

	Deque() : head(nullptr), tail(nullptr), len(0) {}
	~Deque() {
		clear();
	}

	bool empty() {
		return 0 == len;
	}

	size_t size() {
		return len;
	}

	T front() {
		assert(len > 0);
		return head->value;
	}

	T back() {
		assert(len > 0);
		return tail->value;
	}

	void clear() {
		if (nullptr != head) {
			struct Node* next = nullptr;
			while (head != nullptr) {
				next = head->next;
				GiveBack(head);
				head = next;
			}

			tail = nullptr;
			len = 0;
		}
	}

	void insert(int pos, const T& t) {
		assert(pos <= len && pos >= 0);
		struct Node* n = GetNode();
		n->value = t;

		insert_node(n);
	}

	void insert(int pos, T&& t) {
		assert(pos <= len && pos >= 0);

		struct Node* n = GetNode();
		n->value = std::move(t);

		insert_node(pos, n);
	}

	void erase(int pos) {
		assert(pos < len && pos >= 0);


		struct Node* n = head;
		while (pos-- > 0) {
			n = n->next;
		}

		struct Node* next = n->next;
		struct Node* pre = n->pre;

		GiveBack(n);
		if (nullptr != next && nullptr != pre) {
			pre->next = next;
			next->pre = pre;
		}
		else if (nullptr != next) {
			next->pre = pre;
			head = next;
		}
		else if (nullptr != pre) {
			pre->next = next;
			tail = pre;
		}
		else {
			tail = head = next;
		}

		len--;
	}

	void push_back(const T& t) {
		struct Node *n = GetNode();
		n->value = t;

		push_back_node(n);
	}

	void push_back(T&& t) {
		struct Node *n = GetNode();
		n->value = std::move(t);

		push_back_node(n);
	}

	void pop_back() {
		assert(len > 0);

		len--;

		struct Node* pre = tail->pre;
		GiveBack(tail);
		tail = pre;

		if (0 == len) {
			head = tail;
		}
		else {
			tail->next = nullptr;
		}
	}

	void push_front(const T& t) {
		struct Node* n = GetNode();
		n->value = t;

		push_front_node(n);
	}

	void push_front(T&& t) {
		struct Node* n = GetNode();
		n->value = std::move(t);

		push_front_node(n);
	}

	void pop_front() {
		assert(len > 0);

		len--;

		struct Node* next = head->next;
		GiveBack(head);
		head = next;

		if (0 == len) {
			tail = head;
		}
		else {
			head->pre = nullptr;
		}
	}
};

#endif