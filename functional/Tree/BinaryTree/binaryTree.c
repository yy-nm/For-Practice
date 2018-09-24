#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>

#include "binaryTree.h"


Node* createNode(int _value) {
	Node *pnode = (Node *) malloc(sizeof(Node));
	if (NULL == pnode) {
		perror("malloc fail");
		return NULL;
	}

	pnode->value = _value;
	pnode->lchild = NULL;
	pnode->rchild = NULL;

	return pnode;
}

Node* buildTree(int *_array, int _arrayLength, int _curIndex) {
	if (_curIndex >= _arrayLength || NULL == _array)
		return NULL;

	if (0 == _array[_curIndex])
		return NULL;

	Node *pnode = createNode(_array[_curIndex]);
	if (NULL == pnode)
		return NULL;
	pnode->lchild = buildTree(_array, _arrayLength
			, _curIndex * 2 + 1);
	pnode->rchild = buildTree(_array, _arrayLength
			, _curIndex * 2 + 2);

	return pnode;
}

void releaseTree(Node *_node) {
	if(NULL != _node->lchild)
		releaseTree(_node->lchild);
	if(NULL != _node->rchild)
		releaseTree(_node->rchild);
	free(_node);
}
