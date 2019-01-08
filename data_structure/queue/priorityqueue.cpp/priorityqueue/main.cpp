

#include "priorityqueue.h"


#include <vector>
#include <iostream>


using namespace priorityqueue;


void printPriority(MaxPriorityQueue<int, int> &q)
{
	std::cout << std::endl;
	while (q.size() > 0) {
		std::cout << q.top() << std::endl;
		q.pop();
	}
}

void printPriority(MinPriorityQueue<int, int> &q)
{
	std::cout << std::endl;
	while (q.size() > 0) {
		std::cout << q.top() << std::endl;
		q.pop();
	}
}

int main()
{
	std::vector<int> testcase;


	MaxPriorityQueue<int, int> q;

	testcase = {
		1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	for (int i = 0; i < testcase.size(); i++) {
		q.push(testcase[i], testcase[i]);
	}

	printPriority(q);

	testcase = {
		9, 8, 7, 6, 5, 4, 3, 2, 1, 0
	};

	for (int i = 0; i < testcase.size(); i++) {
		q.push(testcase[i], testcase[i]);
	}

	printPriority(q);


	MinPriorityQueue<int, int> qq;

	testcase = {
		1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	for (int i = 0; i < testcase.size(); i++) {
		qq.push(testcase[i], testcase[i]);
	}

	printPriority(qq);

	testcase = {
		9, 8, 7, 6, 5, 4, 3, 2, 1, 0
	};

	for (int i = 0; i < testcase.size(); i++) {
		qq.push(testcase[i], testcase[i]);
	}

	printPriority(qq);
	
}