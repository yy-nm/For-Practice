#include <stdio.h>

#include "traverseBinaryTree.h"

void printPreorderTree(Node *_node) {
	if (NULL == _node)
		return;
	printf("%d ", _node->value);
	printPreorderTree(_node->lchild);
	printPreorderTree(_node->rchild);
}

void printInorderTree(Node *_node) {
	if (NULL == _node)
		return;
	printInorderTree(_node->lchild);
	printf("%d ", _node->value);
	printInorderTree(_node->rchild);
}

void printPostorderTree(Node *_node) {
	if (NULL == _node)
		return;
	printPostorderTree(_node->lchild);
	printPostorderTree(_node->rchild);
	printf("%d ", _node->value);
}
