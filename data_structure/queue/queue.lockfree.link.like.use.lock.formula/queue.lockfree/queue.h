
#ifndef _QUEUE_H_
#define _QUEUE_H_


#include <atomic>
#include <cassert>
#include <thread>


template<typename T>
class Queue
{
protected:
	struct Node
	{
		T value;
		std::atomic<struct Node*> next;

		Node() : next(nullptr) {}
	};
	std::atomic<struct Node*> head;
	std::atomic<struct Node*> tail;
	std::atomic_size_t len;
	std::atomic_size_t executecount;
	std::atomic_size_t requestcount;


	static void GiveBack(struct Node* n) {
		if (nullptr != n) {
			delete n;
		}
	}

	static struct Node* GetNode() {
		return new struct Node();
	}

	void Push2Tail(struct Node* n) {

		GetOccupation();

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
				next = now->next.load();
				// ?? why must add the statement(nullptr != now->next.load()), or multiple-thread push will has some node which next pointer is nullptr
				//if (nullptr == now)
				if (nullptr == now || nullptr != next)
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

		GiveBackOccupation();
	}

	void GetOccupation()
	{
		size_t count = 0;
		size_t next = 0;
		bool ret = false;
		while (!ret) {
			count = requestcount.load();
			next = count + 1;
			ret = requestcount.compare_exchange_weak(count, next);
		}

		
		while (executecount.load() != next) {
			// maybe use sleep(0) can slow cpu down
			std::this_thread::sleep_for(std::chrono::milliseconds(0));
		}
	}

	void GiveBackOccupation()
	{
		executecount++;
	}

public:

	Queue() : len(0), head(nullptr), tail(nullptr), executecount(1), requestcount(0){}
	~Queue() {
		GetOccupation();

		struct Node* now = head.load();
		struct Node *next = nullptr;
		while (nullptr != now) {
			next = now->next.load();
			GiveBack(now);
			now = next;
		}

		len = 0;

		GiveBackOccupation();
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
		GetOccupation();
		assert(len.load() > 0);

		struct Node* n = std::atomic_load(&head);
		assert(nullptr != n);
		T t = std::move(n->value);
		GiveBackOccupation();
		return t;
	}

	T back() {
		GetOccupation();

		assert(len.load() > 0);
		struct Node* n = std::atomic_load(&tail);
		assert(nullptr != n);
		T t = std::move(n->value);
		GiveBackOccupation();
		return t;
	}

	void pop() {
		GetOccupation();

		assert(len.load() > 0);
		len--;
		struct Node* lastone = std::atomic_load(&tail);
		assert(len.load() >= 0);

		struct Node* now = nullptr;
		struct Node* next = nullptr;

		size_t index;
		bool ret = false;

		while (!ret) {
			now = head.load();

			if (nullptr == now)
				continue;

			next = now->next.load();

			ret = std::atomic_compare_exchange_weak(&head, &now, next);
		}

		if (0 == len.load()) {
			tail.compare_exchange_strong(lastone, nullptr);
		}

		GiveBack(now);

		GiveBackOccupation();
	}
};


#endif