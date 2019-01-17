

#include "queue.h"


#include <atomic>
#include <thread>
#include <iostream>


struct TestCase
{
	Queue<int> q;
	std::atomic_int count;
};


void CurrencyPush(TestCase *tc, int count, int start)
{

	for (int i = 0; i < count; i++) {
		tc->q.push(i + start);
	}
	tc->count+= count;
}

void CurrencyPop(TestCase *tc, int count)
{
	for (int i = 0; i < count; i++) {
		tc->q.pop();
	}
	tc->count -= count;
}


int main()
{

	TestCase tc;
	int threadcount = 5;
	int pushcountperthread = 1000000;
	int realcount = 0;
	int maxvalue = threadcount * pushcountperthread;
	
//	/*
	tc.count = 0;
	// test case 1 multiple push

	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPush, &tc, pushcountperthread, i * pushcountperthread);
		t.detach();
	}

	while (tc.count.load() < threadcount * pushcountperthread) {

	}

	std::cout << tc.q.size() << std::endl;

	realcount = 0;
	while (tc.q.size() > 0) {
		//std::cout << tc.q.front() << std::endl;
		if (tc.q.front() >= maxvalue)
			std::cout << "error value: " << tc.q.front() << ", pos: " << tc.q.size() << std::endl;
		tc.q.pop();
		realcount++;
	}

	std::cout << realcount << std::endl;
//	*/

	tc.count = 0;
	// test case 2 multiple pop
	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPush, &tc, pushcountperthread, i * pushcountperthread);
		t.detach();
	}

	while (tc.count.load() < threadcount * pushcountperthread) {

	}

	std::cout << tc.q.size() << std::endl;
	
	/*
	realcount = 0;
	while (tc.q.size() > 0) {
		tc.q.pop();
		realcount++;
	}
	std::cout << realcount << std::endl;
//	*/

//	/*
	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPop, &tc, pushcountperthread);
		t.detach();
	}

	while (tc.count.load() != 0) {

	}
//	*/
	
	std::cout << "OK!" << std::endl;



	return 0;
}