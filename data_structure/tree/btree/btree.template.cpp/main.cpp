

#include <iostream>
#include <stdio.h>


#include "btree.h"


using namespace btree;


int main(void)
{

	BTree<3> *tree = BTree<3>::CreateEmptyTree();

	for (int i = 1; i < 11; i++) {
		tree->Insert(i);
		tree->TraverseTree();
	}

	int searchvalue = 1;
	printf("find 1: %d\n", tree->Search(searchvalue));
	searchvalue = 11;
	printf("find 11: %d\n", tree->Search(searchvalue));
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


	tree->FreeTree();
	int v;
	for (int i = 1; i < 11; i++) {
		v = 11 - i;
		tree->Insert(v);
		tree->TraverseTree();
	}

	searchvalue = 1;
	printf("find 1: %d\n", tree->Search(searchvalue));
	searchvalue = 11;
	printf("find 11: %d\n", tree->Search(searchvalue));

	return 0;
}