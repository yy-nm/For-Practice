

#include "heap.h"


#include <vector>
#include <iostream>


using namespace binaryheap;


void printHeap(MaxBinaryHeap<int> &h)
{
	std::cout << std::endl;
	
	while (h.size() > 0) {
		std::cout << h.top() << std::endl;
		h.pop();
	}
}

void printHeap(MinBinaryHeap<int> &h)
{
	std::cout << std::endl;

	while (h.size() > 0) {
		std::cout << h.top() << std::endl;
		h.pop();
	}
}

int main()
{
	MaxBinaryHeap<int> h;

	std::vector<int> testdata = {
		1, 2, 3, 4, 5, 6, 7, 8, 9
	};


	for (size_t i = 0; i < testdata.size(); i++) {
		h.push(testdata[i]);
	}

	printHeap(h);

	testdata = {
		9, 8, 7, 6, 5, 4, 3, 2, 1
	};


	for (size_t i = 0; i < testdata.size(); i++) {
		h.push(testdata[i]);
	}

	printHeap(h);

	// min binary heap

	MinBinaryHeap<int> hh;

	testdata = {
		1, 2, 3, 4, 5, 6, 7, 8, 9
	};


	for (size_t i = 0; i < testdata.size(); i++) {
		hh.push(testdata[i]);
	}

	printHeap(hh);

	testdata = {
		9, 8, 7, 6, 5, 4, 3, 2, 1
	};


	for (size_t i = 0; i < testdata.size(); i++) {
		hh.push(testdata[i]);
	}

	printHeap(hh);

}