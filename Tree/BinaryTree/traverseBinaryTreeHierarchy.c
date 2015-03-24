#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "binaryTree.h"
#include "traverseBinaryTree.h"


typedef struct _queue {
	Node *node;
	struct _queue *next;
} Queue;


inline Queue* createQueueNode(Node *_node) {
	if (NULL == _node)
		return NULL;
	Queue *q = (Queue *) malloc(sizeof(Queue));
	if (NULL == q) {
		printf("malloc QueueNode fail!\n");
		return NULL;
	}
	q->next = NULL;
	q->node = _node;

	return q;
}

void printTreeInHerarchy(Node *_root) {
	Queue q1;
	Queue q2;
	Queue *qIndex;
	
	q1.next = NULL;
	q2.next = NULL;

	q1.next = createQueueNode(_root);

	while(NULL !=  q1.next)
	{
		if(NULL != q1.next->node->lchild) {
			qIndex = createQueueNode(q1.next->node->lchild);
			if (NULL != qIndex) {
				qIndex->next = q2.next;
				q2.next = qIndex;
			}
		}

		if(NULL != q1.next->node->rchild) {
			qIndex = createQueueNode(q1.next->node->rchild);
			if (NULL != qIndex) {
				qIndex->next = q2.next;
				q2.next = qIndex;
			}
		}

		qIndex = q1.next;
		printf("%d ", qIndex->node->value);
		q1.next = qIndex->next;
		free(qIndex);

		if (NULL == q1.next) {
			q1.next = q2.next;
			q2.next = NULL;
			printf("\n");
		}
	}
}


int main(int argc, char **args) {
	int tree[] = { 1, 2, 3, 0, 5, 6, 7, 8 };
	Node *root = buildTree(tree, sizeof(tree) / sizeof(int), 0);

	printInorderTree(root);
	printf("\n");
	printPreorderTree(root);
	printf("\n");
	printPostorderTree(root);
	printf("\n");

	printTreeInHerarchy(root);

	releaseTree(root);
	return 0;
}
