

#include "deque.h"


#include <iostream>


void PrintDeque(Deque<int> &q)
{
	std::cout << "deque count: " << q.size() << std::endl;
	std::cout << "deque is empty: " << q.empty() << std::endl;

	while (q.size() > 0) {
		std::cout << q.front() << std::endl;
		q.pop_front();
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

int main(void)
{

	Deque<int> q;

	for (int i = 0; i < 10; i++) {
		q.push_back(i);
	}
	
	q.insert(0, 11);
	q.insert(11, -1);

	PrintDeque(q);

	for (int i = 0; i < 10; i++) {
		q.push_back(100 + i);
	}

	PrintDeque(q);

	q.insert(0, 1);

	PrintDeque(q);

	q.insert(0, 1);
	q.insert(1, 2);
	q.erase(0);

	PrintDeque(q);

	q.insert(0, 1);
	q.erase(0);

	PrintDeque(q);

	return 0;
}