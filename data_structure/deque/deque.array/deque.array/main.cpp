

#include "deque.h"


#include <iostream>


void PrintDeque(Deque<int> &q)
{
	std::cout << "deque count: " << q.size() << std::endl;
	std::cout << "deque cap: " << q.max_size() << std::endl;
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

	PrintDeque(q);

	for (int i = 0; i < 10; i++) {
		q.push_front(i);
	}

	PrintDeque(q);


	for (int i = 0; i < 10; i++) {
		q.push_back(i);
	}
	for (int i = 0; i < 10; i++) {
		q.push_front(i + 100);
	}

	PrintDeque(q);

	for (int i = 0; i < 20; i++) {
		if (i % 2 == 0)
			q.push_front(i);
		else
			q.push_back(i);
	}
	PrintDeque(q);

	for (int i = 0; i < 20; i++) {
		if (i % 2 == 0)
			q.push_front(i);
		else
			q.push_back(i);
	}

	q.pop_back();
	q.pop_front();
	PrintDeque(q);

	for (int i = 0; i < 20; i++) {
		if (i % 2 == 0)
			q.push_front(i);
		else
			q.push_back(i);
	}

	q.pop_back();
	q.pop_front();
	q.pop_front();
	q.pop_front();
	q.pop_front();
	q.pop_front();
	PrintDeque(q);

	for (int i = 0; i < 20; i++) {
		if (i % 2 == 0)
			q.push_front(i);
		else
			q.push_back(i);
	}

	q.insert(10, 100);
	q.insert(0, 101);
	q.insert(22, 102);

	PrintDeque(q);

	for (int i = 0; i < 20; i++) {
		if (i % 2 == 0)
			q.push_front(i);
		else
			q.push_back(i);
	}

	q.insert(10, 100);
	q.insert(0, 101);
	q.insert(22, 102);

	q.erase(0);

	q.erase(9);

	q.erase(19);

	PrintDeque(q);

	return 0;
}