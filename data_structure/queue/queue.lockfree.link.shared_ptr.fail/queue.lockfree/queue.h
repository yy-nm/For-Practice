
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
		std::shared_ptr<struct Node> next;

		Node() : next(nullptr) {}
	};
	std::shared_ptr<struct Node> head;
	std::shared_ptr<struct Node> tail;
	std::atomic_size_t len;
	std::atomic_size_t popindex;

	std::shared_ptr<struct Node> kEmptyPointer;

	static void GiveBack(std::shared_ptr<struct Node> &n) {
	}

	static std::shared_ptr<struct Node> GetNode() {
		auto p = std::make_shared<struct Node>();
		return p;
	}

	void Push2Tail(std::shared_ptr<struct Node> &n) {
		bool ret = false;
		std::shared_ptr<struct Node> emptypointer = kEmptyPointer;
		std::shared_ptr<struct Node> now;
		std::shared_ptr<struct Node> next;
		while (!ret) {
			
			if (0 == len.load()) {
				// in case to avoid pop op during middle.
				if (nullptr != std::atomic_load(&tail) || nullptr != std::atomic_load(&head)) {
					continue;
				}

				ret = std::atomic_compare_exchange_weak(&tail, &emptypointer, n);
			}
			else {
				now = std::atomic_load(&tail);
				next = std::atomic_load(&(now->next));
				// ?? why must add the statement(nullptr != now->next.load()), or multiple-thread push will has some node which next pointer is nullptr
				//if (nullptr == now)
				if (nullptr == now || nullptr != next)
					continue;
				ret = std::atomic_compare_exchange_weak(&(now->next), &emptypointer, n);
				//ret = now->next.compare_exchange_strong(emptypointer, n);
				if (!ret) {
					continue;
				}

				std::atomic_store(&tail, n);
			}
		}

		if (0 == len.load()) {
			//head.store(tail);
			std::atomic_store(&head, tail);
		}

		len++;
	}

public:

	Queue() : len(0), head(nullptr), tail(nullptr), popindex(0) {}
	~Queue() {
		/*
		struct Node* now = head.load();
		struct Node *next = nullptr;
		while (nullptr != now) {
			next = now->next.load();
			GiveBack(now);
			now = next;
		}
		*/

		len = 0;
	}

	size_t size() {
		return len.load();
	}

	bool empty() {
		return 0 == len.load();
	}

	void push(const T& t) {
		std::shared_ptr<struct Node> n = GetNode();
		n->next = kEmptyPointer;
		n->value = t;

		Push2Tail(n);
	}

	void push(T&& t) {
		std::shared_ptr<struct Node> n = GetNode();
		n->next = kEmptyPointer;
		n->value = std::move(t);

		Push2Tail(n);
	}

	T front() {
		assert(len.load() > 0);

		std::shared_ptr<struct Node> n = std::atomic_load(&head);
		assert(nullptr != n);
		return n->value;
	}

	T back() {
		assert(len.load() > 0);
		std::shared_ptr<struct Node> n = std::atomic_load(&tail);
		assert(nullptr != n);
		return n->value;
	}

	void pop() {
		assert(len.load() > 0);
		len--;
		std::shared_ptr<struct Node> lastone = std::atomic_load(&tail);
		assert(len.load() >= 0);

		std::shared_ptr<struct Node> now = nullptr;
		std::shared_ptr<struct Node> next = nullptr;
		std::shared_ptr<struct Node> emptypointer = kEmptyPointer;

		size_t index;
		bool ret = false;
		std::atomic_thread_fence(std::memory_order_acquire);
		while (!ret) {
			index = popindex.load();
			now = std::atomic_load(&head);
			if (index != popindex.load()) {
				continue;
			}

			if (nullptr == now)
				continue;

			next = std::atomic_load(&(now->next));
			if (index != popindex.load()) {
				continue;
			}

			ret = std::atomic_compare_exchange_weak(&head, &now, next);
			popindex++;
		}

		if (0 == len.load()) {
			std::atomic_compare_exchange_strong(&tail, &lastone, emptypointer);
		}

		std::atomic_thread_fence(std::memory_order_release);
		popindex++;
		GiveBack(now);
	}
};


#endif