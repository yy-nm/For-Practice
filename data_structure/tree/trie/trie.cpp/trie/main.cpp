

#include "trie.h"


#include <string>
#include <vector>
#include <iostream>


int main(void)
{
	std::vector<std::string> insertwords = {
		"hello, world",
		"hello, world1",
		"helloworld",
		"hello world",
	};

	std::vector<std::string> deletewords = {
		"hello",
		"helloworld",
	};

	std::vector<std::string> searchwords = {
		"hello, world1",
		"hello",
	};

	trie::Trie t;

	for (auto it = insertwords.begin(); it < insertwords.end(); it++) {
		t.Insert(*it);
	}

	for (auto it = deletewords.begin(); it < deletewords.end(); it++) {
		t.Delete(*it);
	}

	for (auto it = searchwords.begin(); it < searchwords.end(); it++) {
		std::cout << "search " << *it << " result: " << t.Search(*it) << std::endl;
	}

	t.PrintAllString();

	return 0;
}
