

#include <iostream>


#include "avl.h"


using namespace avl;

void print_tree(Tree<int> &tree)
{

	tree.TraversePreorder();
	std::cout << std::endl;
	tree.TraverseInorder();
	std::cout << std::endl;
	std::cout << std::endl;
}

int main(void)
{

	Tree<int> tree;

	for (int i = 1; i < 11; i++) {
		tree.Add(i);
		print_tree(tree);
	}

	int searchvalue = 1;
	printf("find 1: %d\n", tree.Search(searchvalue));
	searchvalue = 11;
	printf("find 11: %d\n", tree.Search(searchvalue));
	int removevalue;
	removevalue = 4;
	tree.Remove(removevalue);
	print_tree(tree);
	removevalue = 5;
	tree.Remove(removevalue);
	print_tree(tree);
	removevalue = 6;
	tree.Remove(removevalue);
	print_tree(tree);
	removevalue = 7;
	tree.Remove(removevalue);
	print_tree(tree);
	removevalue = 8;
	tree.Remove(removevalue);
	print_tree(tree);
	removevalue = 9;
	tree.Remove(removevalue);
	print_tree(tree);
	removevalue = 2;
	tree.Remove(removevalue);
	print_tree(tree);


	Tree<int> tree1;
	int v;
	for (int i = 1; i < 11; i++) {
		v = 11 - i;
		tree1.Add(v);
		print_tree(tree1);
	}

	searchvalue = 1;
	printf("find 1: %d\n", tree1.Search(searchvalue));
	searchvalue = 11;
	printf("find 11: %d\n", tree1.Search(searchvalue));

	return 0;
}