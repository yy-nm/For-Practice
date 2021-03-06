



#include <stdio.h>


#include "redblack.h"


void print_tree(rb_tree_t *tree)
{
	if (tree) {
		rb_tree_traverse_preorder(tree);
		printf("\n");
		rb_tree_traverse_inorder(tree);
		printf("\n");
		printf("\n");
	}
}

int main(void)
{

	rb_tree_t tree;
	tree.root = NULL;

	for (int i = 1; i < 11; i++) {
		rb_tree_insert(&tree, i);
		print_tree(&tree);
	}
	printf("find 1: %d\n", rb_tree_search(&tree, 1));
	printf("find 11: %d\n", rb_tree_search(&tree, 11));
	rb_tree_delete(&tree, 4);
	print_tree(&tree);
	rb_tree_delete(&tree, 5);
	print_tree(&tree);
	rb_tree_delete(&tree, 6);
	print_tree(&tree);
	rb_tree_delete(&tree, 7);
	print_tree(&tree);
	rb_tree_delete(&tree, 8);
	print_tree(&tree);
	rb_tree_delete(&tree, 9);
	print_tree(&tree);
	rb_tree_delete(&tree, 2);
	print_tree(&tree);
	rb_tree_free(&tree);

	for (int i = 1; i < 11; i++) {
		rb_tree_insert(&tree, 11 - i);
		print_tree(&tree);
	}

	printf("find 1: %d\n", rb_tree_search(&tree, 1));
	printf("find 11: %d\n", rb_tree_search(&tree, 11));
	rb_tree_free(&tree);

	return 0;
}