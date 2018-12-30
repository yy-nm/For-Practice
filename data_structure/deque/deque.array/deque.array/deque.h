

#ifndef _DEQUE_H_
#define _DEQUE_H_


#include <cassert>


template<typename T>
class Deque
{
protected:

	T* valuearray;
	size_t len;
	size_t cap;

	size_t head;

	static void GiveBack(T* arr) {
		if (nullptr != arr) {
			delete[] arr;
		}
	}

	static T* GetNodes(size_t sz) {
		return new T[sz];
	}

	static size_t GetPropertyCap(size_t cap) {
		if (0 == cap)
			return kBeginCap;
		else
			return cap << 1;
	}

	static const int kBeginCap = 4;

	void CheckCap() {
		if (len == cap) {
			size_t oldcap = cap;
			cap = GetPropertyCap(cap);
			T* narr = GetNodes(cap);

			for (int i = 0; i < len; i++) {
				narr[i] = std::move(valuearray[(head + i) % oldcap]);
			}

			head = 0;
			GiveBack(valuearray);
			valuearray = narr;
		}
	}

	void EmptyThePosition(size_t pos) {
		CheckCap();
		if (0 == len) {

		}
		else if (0 == pos) {
			if (0 == head) {
				head = cap - 1;
			}
			else {
				head--;
			}
		}
		else {
			for (size_t i = len; i > pos; i--) {
				valuearray[(head + i) % cap] = std::move(valuearray[(head + i - 1) % cap]);
			}
		}
	}
public:

	Deque() : valuearray(nullptr), len(0), cap(0), head(0) {}
	~Deque() {
		if (nullptr != valuearray) {
			GiveBack(valuearray);
			valuearray = nullptr;
		}

		len = cap = 0;
		head = 0;
	}


	T front() {
		assert(len > 0);
		return valuearray[head];
	}

	T back() {
		assert(len > 0);
		return valuearray[(head +len) % cap];
	}

	bool empty() {
		return 0 == len;
	}

	size_t size() {
		return len;
	}

	size_t max_size() {
		return cap;
	}

	void clear() {
		len = cap = 0;
		head = 0;
	}

	void insert(size_t pos, const T& t) {
		assert(pos >= 0 && pos <= len);
		EmptyThePosition(pos);
		valuearray[(head + pos) % cap] = t;
		len++;
	}
	
	void insert(size_t pos, T&& t) {
		EmptyThePosition(pos);
		valuearray[(head + pos) % cap] = std::move(t);
		len++;
	}

	void erase(size_t pos) {
		assert(pos >= 0 && pos < len);

		for (size_t i = pos; i < len - 1; i++) {
			valuearray[(head + i) % cap] = valuearray[(head + i + 1) % cap];
		}

		len--;
	}

	void push_back(const T& t) {
		CheckCap();
		valuearray[(head + len) % cap] = t;
		len++;
	}

	void push_back(T&& t) {
		CheckCap();
		valuearray[(head + len) % cap] = std::move(t);
		len++;
	}

	void pop_back() {
		assert(len > 0);
		len--;
	}

	void push_front(const T& t) {
		EmptyThePosition(0);
		valuearray[head] = t;
		len++;
	}

	void push_front(T&& t) {
		EmptyThePosition(0);
		valuearray[head] = std::move(t);
		len++;
	}

	void pop_front() {
		assert(len > 0);
		head++;
		head %= cap;
		len--;
	}
};

#endif