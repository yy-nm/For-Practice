// get_ebp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>

#ifdef WIN32

void *get_ebp_pointer() {
	char *p = NULL;
	char *n = NULL;
	__asm mov p, ebp; // same as linux: asm("mov %%rbp, %0;" : "=r" (p));
	//std::cout << "get ebp value: " << p << std::endl;
	while (p != NULL) {
		std::cout << "get ebp value: " << (void *)p << std::endl;
		n = *(char **)p;
		for (auto i = p; i < n; i++) {
			auto v = *(unsigned char *)i;
			std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)v << " ";
			if (!((i + 1 - p) & 0xf)) {
				std::cout << std::endl;
			}
		}

		std::cout << std::endl;
		std::cout << std::endl;
		p = n;
	}
	return p;
}

#else

//extern "C" void * __fastcall asm_func();
void *get_ebp_pointer() {
	//char *p = NULL;
	//char *n = NULL;
	////std::cout << "p addr: " << &p << std::endl;
	//p = (char*)asm_func();
	//while (p != NULL) {
	//	std::cout << "get ebp value: " << (void *)p << std::endl;
	//	n = *(char **)p;
	//	for (auto i = p; i < n; i++) {
	//		auto v = *(unsigned char *)i;
	//		std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)v << " ";
	//		if (!((i + 1 - p) & 0xf)) {
	//			std::cout << std::endl;
	//		}
	//	}

	//	std::cout << std::endl;
	//	std::cout << std::endl;
	//	p = n;
	//}
	//return p;

	std::cout << "rbp not support in x64 model" << std::endl;
	return NULL;
}

#endif


int main()
{
	void *v = NULL;
	//std::cout << "v addr: " << &v << std::endl;
	v = get_ebp_pointer();

	std::cout << "v: " << v << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
