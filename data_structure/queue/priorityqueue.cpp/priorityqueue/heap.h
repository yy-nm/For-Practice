

#ifndef _HEAD_H_
#define _HEAD_H_


#include <cassert>

namespace binaryheap
{
	template<typename T>
	class MaxBinaryHeap
	{
	protected:
		T* narray;
		size_t cap;
		size_t len;

		static const size_t kStartCap = 4;

		static void GiveBack(T *arr) {
			if (nullptr != arr)
				delete[] arr;
		}

		static T* GetArray(size_t sz) {
			return new T[sz];
		}

		static size_t GetPropertyCap(size_t sz) {
			if (0 == sz)
				return kStartCap;
			else
				return sz << 1;
		}

		void CheckSize() {
			if (cap == len) {
				size_t ncap = GetPropertyCap(cap);
				T* arr = GetArray(ncap);
				for (int i = 0; i < len; i++) {
					arr[i] = std::move(narray[i]);
				}
				GiveBack(narray);
				narray = arr;
				cap = ncap;
			}
		}

		void CheckHeapFromBottom2Top() {
			size_t index = len - 1;
			size_t pindex;
			T temp;
			while (index > 0) {
				pindex = (index - 1) / 2;
				if (narray[index] > narray[pindex]) {
					temp = std::move(narray[pindex]);
					narray[pindex] = std::move(narray[index]);
					narray[index] = std::move(temp);

					index = pindex;
				}
				else {
					break;
				}
			}
		}

		void CheckHeapFromTop2Bottom(size_t index) {
			if (index >= len)
				return;

			size_t indexl = index * 2 + 1;
			size_t indexr = index * 2 + 2;
			bool left = false;
			bool right = false;


			if (indexl < len && narray[index] < narray[indexl]) {
				left = true;
			}

			if (indexr < len && ((left && narray[indexl] < narray[indexr]) || (!left && narray[index] < narray[indexr]))) {
				left = false;
				right = true;
			}

			T temp;
			if (left) {
				temp = std::move(narray[index]);
				narray[index] = std::move(narray[indexl]);
				narray[indexl] = std::move(temp);

				CheckHeapFromTop2Bottom(indexl);
			}
			else if (right) {
				temp = std::move(narray[index]);
				narray[index] = std::move(narray[indexr]);
				narray[indexr] = std::move(temp);

				CheckHeapFromTop2Bottom(indexr);
			}
		}

	public:
		MaxBinaryHeap() : narray(nullptr), cap(0), len(0) {}
		~MaxBinaryHeap() {
			GiveBack(narray);
			narray = nullptr;
			cap = len = 0;
		}


		T top() {
			assert(len > 0);
			return narray[0];
		}

		size_t size() {
			return len;
		}

		bool empty() {
			return 0 == len;
		}

		void push(const T& t) {
			CheckSize();
			narray[len] = t;
			len++;

			CheckHeapFromBottom2Top();
		}

		void push(T&& t) {
			CheckSize();
			narray[len] = std::move(t);
			len++;
			
			CheckHeapFromBottom2Top();
		}

		void pop() {
			assert(len > 0);
			narray[0] = std::move(narray[len - 1]);
			len--;

			CheckHeapFromTop2Bottom(0);
		}

	};

	template<typename T>
	class MinBinaryHeap
	{
	protected:
		T * narray;
		size_t cap;
		size_t len;

		static const size_t kStartCap = 4;

		static void GiveBack(T *arr) {
			if (nullptr != arr)
				delete[] arr;
		}

		static T* GetArray(size_t sz) {
			return new T[sz];
		}

		static size_t GetPropertyCap(size_t sz) {
			if (0 == sz)
				return kStartCap;
			else
				return sz << 1;
		}

		void CheckSize() {
			if (cap == len) {
				size_t ncap = GetPropertyCap(cap);
				T* arr = GetArray(ncap);
				for (int i = 0; i < len; i++) {
					arr[i] = std::move(narray[i]);
				}
				GiveBack(narray);
				narray = arr;
				cap = ncap;
			}
		}

		void CheckHeapFromBottom2Top() {
			size_t index = len - 1;
			size_t pindex;
			T temp;
			while (index > 0) {
				pindex = (index - 1) / 2;
				if (narray[index] < narray[pindex]) {
					temp = std::move(narray[pindex]);
					narray[pindex] = std::move(narray[index]);
					narray[index] = std::move(temp);

					index = pindex;
				}
				else {
					break;
				}
			}
		}

		void CheckHeapFromTop2Bottom(size_t index) {
			if (index >= len)
				return;

			size_t indexl = index * 2 + 1;
			size_t indexr = index * 2 + 2;
			bool left = false;
			bool right = false;


			if (indexl < len && narray[index] > narray[indexl]) {
				left = true;
			}

			if (indexr < len && ((left && narray[indexl] > narray[indexr]) || (!left && narray[index] > narray[indexr]))) {
				left = false;
				right = true;
			}

			T temp;
			if (left) {
				temp = std::move(narray[index]);
				narray[index] = std::move(narray[indexl]);
				narray[indexl] = std::move(temp);

				CheckHeapFromTop2Bottom(indexl);
			}
			else if (right) {
				temp = std::move(narray[index]);
				narray[index] = std::move(narray[indexr]);
				narray[indexr] = std::move(temp);

				CheckHeapFromTop2Bottom(indexr);
			}
		}

	public:
		MinBinaryHeap() : narray(nullptr), cap(0), len(0) {}
		~MinBinaryHeap() {
			GiveBack(narray);
			narray = nullptr;
			cap = len = 0;
		}


		T top() {
			assert(len > 0);
			return narray[0];
		}

		size_t size() {
			return len;
		}

		bool empty() {
			return 0 == len;
		}

		void push(const T& t) {
			CheckSize();
			narray[len] = t;
			len++;

			CheckHeapFromBottom2Top();
		}

		void push(T&& t) {
			CheckSize();
			narray[len] = std::move(t);
			len++;

			CheckHeapFromBottom2Top();
		}

		void pop() {
			assert(len > 0);
			narray[0] = std::move(narray[len - 1]);
			len--;

			CheckHeapFromTop2Bottom(0);
		}

	};
}


#endif