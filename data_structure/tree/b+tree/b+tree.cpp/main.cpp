

#include "b+tree.h"

#include <cstdio>

using namespace bplustree;


int main(void)
{
	BPlusTree<7> *tree = BPlusTree<7>::CreateEmptyTree();

	for (int i = 1; i < 111; i++) {
		tree->Insert(i);
		tree->TraverseTree();
	}

	int searchvalue = 1;
	std::cout << "find 1: " << tree->Search(searchvalue) << std::endl;
	searchvalue = 11;
	std::cout << "find 11: " << tree->Search(searchvalue) << std::endl;

	int removevalue;
	removevalue = 4;
	tree->Delete(removevalue);
	tree->TraverseTree();
	removevalue = 5;
	tree->Delete(removevalue);
	tree->TraverseTree();
	removevalue = 6;
	tree->Delete(removevalue);
	tree->TraverseTree();
	removevalue = 7;
	tree->Delete(removevalue);
	tree->TraverseTree();
	removevalue = 8;
	tree->Delete(removevalue);
	tree->TraverseTree();
	removevalue = 9;
	tree->Delete(removevalue);
	tree->TraverseTree();
	removevalue = 2;
	tree->Delete(removevalue);
	tree->TraverseTree();

	std::cout << std::endl;
	std::cout << std::endl;
	tree->TraverseData();
	std::cout << std::endl;

	tree->FreeTree();
	int v;
	for (int i = 1; i < 11; i++) {
		v = 11 - i;
		tree->Insert(v);
		tree->TraverseTree();
	}

	searchvalue = 1;
	std::cout << "find 1: " << tree->Search(searchvalue) << std::endl;
	searchvalue = 11;
	std::cout << "find 11: " << tree->Search(searchvalue) << std::endl;

	return 0;
}

