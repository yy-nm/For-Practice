

#include "stack.h"


#include <iostream>


int main()
{

	Stack<int> s;

	std::cout << s.empty() << std::endl;
	std::cout << s.size() << std::endl;

	for (int i = 0; i < 10; i++) {
		s.push(i);
	}

	std::cout << s.empty() << std::endl;
	std::cout << s.size() << std::endl;

	for (int i = 0; i < 10; i++) {
		std::cout << s.top() << std::endl;
		s.pop();
	}

	std::cout << s.empty() << std::endl;
	std::cout << s.size() << std::endl;

	return 0;
}