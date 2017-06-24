#ifndef BINARYTREE_H
#define BINARYTREE_H

typedef struct _node {
	int value;
	struct _node *lchild;
	struct _node *rchild;
} Node;


inline Node* createNode(int _value);
Node* buildTree(int *_array, int _arrayLength, int _curIndex);
void releaseTree(Node *_node);


#endif
