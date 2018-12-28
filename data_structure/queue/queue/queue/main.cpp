

#include <queue>
#include <vector>
#include <iostream>


#include "queue.h"


int main(void)
{
	//std::queue<int> q;
	//q.pop();

	//std::cout << q.front() << std::endl;



	Queue<int> qq;

	std::cout << qq.empty() << std::endl;
	std::cout << qq.size() << std::endl;

	std::cout << std::endl;

	for (int i = 0; i < 10; i++) {
		qq.push(11 - i);
	}

	while (qq.size() > 0) {
		std::cout << qq.front() << std::endl;
		qq.pop();
	}

	std::cout << std::endl;

	std::cout << qq.empty() << std::endl;
	std::cout << qq.size() << std::endl;

	return 0;
}
