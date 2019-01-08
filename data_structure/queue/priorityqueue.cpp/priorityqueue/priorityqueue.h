

#ifndef _PRIORITYQUEUE_H_
#define _PRIORITYQUEUE_H_


#include "heap.h"


namespace priorityqueue
{
	template<typename CompareT, typename DataT>
	class MaxPriorityQueue
	{
	protected:
		struct Item {
			CompareT priority;
			DataT data;

			bool operator< (const Item& r) {
				return priority < r.priority;
			}

			bool operator> (const Item& r) {
				return priority > r.priority;
			}

			Item() {}

			Item(Item&& other) {
				priority = std::move(other.priority);
				data = std::move(other.data);
			}

			Item(const Item& other) {
				priority = other.priority;
				data = other.data;
			}

			Item& operator=(Item&& other) {
				if (this != &other) {
					priority = std::move(other.priority);
					data = std::move(other.data);
				}

				return *this;
			}

			Item& operator=(const Item& other) {
				priority = other.priority;
				data = other.data;

				return *this;
			}
		};

		binaryheap::MaxBinaryHeap<Item> heap;
	public:

		MaxPriorityQueue() {}
		~MaxPriorityQueue() {}

		bool empty() {
			return heap.empty();
		}

		size_t size() {
			return heap.size();
		}

		void push(const CompareT& priority, const DataT& data) {
			Item item;
			item.priority = priority;
			item.data = data;

			heap.push(std::move(item));
		}

		void push(const CompareT& priority, DataT&& data) {
			Item item;
			item.priority = priority;
			item.data = std::move(data);

			heap.push(std::move(item));
		}

		void push(CompareT&& priority, DataT&& data) {
			Item item;
			item.priority = std::move(priority);
			item.data = std::move(data);

			heap.push(std::move(item));
		}

		void pop() {
			heap.pop();
		}

		DataT top() {
			Item item = heap.top();
			return item.data;
		}
	};

	template<typename CompareT, typename DataT>
	class MinPriorityQueue
	{
	protected:
		struct Item {
			CompareT priority;
			DataT data;

			bool operator< (const Item& r) {
				return priority < r.priority;
			}

			bool operator> (const Item& r) {
				return priority > r.priority;
			}

			Item() {}

			Item(Item&& other) {
				priority = std::move(other.priority);
				data = std::move(other.data);
			}

			Item(const Item& other) {
				priority = other.priority;
				data = other.data;
			}

			Item& operator=(Item&& other) {
				if (this != &other) {
					priority = std::move(other.priority);
					data = std::move(other.data);
				}

				return *this;
			}

			Item& operator=(const Item& other) {
				priority = other.priority;
				data = other.data;

				return *this;
			}
		};

		binaryheap::MinBinaryHeap<Item> heap;
	public:

		MinPriorityQueue() {}
		~MinPriorityQueue() {}

		bool empty() {
			return heap.empty();
		}

		size_t size() {
			return heap.size();
		}

		void push(const CompareT& priority, const DataT& data) {
			Item item;
			item.priority = priority;
			item.data = data;

			heap.push(std::move(item));
		}

		void push(const CompareT& priority, DataT&& data) {
			Item item;
			item.priority = priority;
			item.data = std::move(data);

			heap.push(std::move(item));
		}

		void push(CompareT&& priority, DataT&& data) {
			Item item;
			item.priority = std::move(priority);
			item.data = std::move(data);

			heap.push(std::move(item));
		}

		void pop() {
			heap.pop();
		}

		DataT top() {
			Item item = heap.top();
			return item.data;
		}
	};
}

#endif
