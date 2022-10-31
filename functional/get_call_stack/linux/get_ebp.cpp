#include <iostream>
#include <iomanip>

void print_ebp_pointer()
{
	char *p = NULL;
	char *n = NULL;
	// window mov format: mov {dst}, {src}
	// linux mov format: mov {src}, {dst}
	// %0 output
	// %1.... input
	asm("mov %%rbp, %0"
		: "=r"(p)
		// : "r" (p) represent %1, input
		// : "r" (p) represent %2, input
	);
	while (p != NULL)
	{
		std::cout << "get ebp value: " << (void *)p << std::endl;
		n = *(char **)p;
		for (char* i = p; i < n; i++)
		{
			unsigned char v = *(unsigned char *)i;
			std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)v << " ";
			if (!((i + 1 - p) & 0xf))
			{
				std::cout << std::endl;
			}
		}

		std::cout << std::endl;
		std::cout << std::endl;
		p = n;
	}
}

int main()
{
	print_ebp_pointer();

	std::cout << "hello world" << std::endl;

	return 0;
}
