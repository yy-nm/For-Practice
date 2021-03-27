

#include "queue.h"


#include <atomic>
#include <thread>
#include <iostream>


struct TestCase
{
	Queue<int> q;
	std::atomic_int push_count;
	std::atomic_int pop_count;
};


void CurrencyPush(TestCase *tc, int count, int start)
{

	for (int i = 0; i < count; i++) {
		tc->q.push(i + start);
	}
	tc->push_count += count;
}

void CurrencyPop(TestCase *tc, int count)
{
	int value;
	int i = 0;
	while (count > i) {
		if (tc->q.pop(value)) {
			i++;
		}
	}
	tc->pop_count += count;
}

void test_1()
{
	TestCase tc;
	int threadcount = 5;
	int pushcountperthread = 1000000;
	//int threadcount = 3;
	//int pushcountperthread = 10000;
	int realcount = 0;
	int maxvalue = threadcount * pushcountperthread;

	/*
	tc.count = 0;
	// test case 1 multiple push

	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPush, &tc, pushcountperthread, i * pushcountperthread);
		t.detach();
	}

	while (tc.count.load() < threadcount * pushcountperthread) {

	}

	realcount = 0;
	while (tc.q.size() > 0) {
		//std::cout << tc.q.front() << std::endl;
		if (tc.q.front() >= maxvalue)
			std::cout << "error value: " << tc.q.front() << ", pos: " << tc.q.size() << std::endl;
		tc.q.pop();
		realcount++;
	}

//	*/

	tc.push_count = 0;
	// test case 2 multiple pop
	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPush, &tc, pushcountperthread, i * pushcountperthread);
		t.detach();
	}

	while (tc.push_count.load() < maxvalue) {

	}

//	/*
	tc.pop_count = 0;
	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPop, &tc, pushcountperthread);
		t.detach();
	}

	while (tc.pop_count.load() < maxvalue) {

	}
	//	*/

	std::cout << "test_1 OK!" << std::endl;
}


void test_2()
{
	TestCase tc;
	int threadcount = 5;
	int pushcountperthread = 1000000;
	int realcount = 0;
	int maxvalue = threadcount * pushcountperthread;

	tc.push_count = 0;
	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPush, &tc, pushcountperthread, i * pushcountperthread);
		t.detach();
	}

	tc.pop_count = 0;
	for (int i = 0; i < threadcount; i++) {
		std::thread t(CurrencyPop, &tc, pushcountperthread);
		t.detach();
	}

	while (tc.push_count.load() < maxvalue) {}
	std::cout << "test_2 push OK!" << std::endl;

	while (tc.pop_count.load() < maxvalue) {}
	std::cout << "test_2 pop OK!" << std::endl;

	std::cout << "test_2 OK!" << std::endl;
}


int main()
{

	test_1();
	test_2();

	return 0;
}