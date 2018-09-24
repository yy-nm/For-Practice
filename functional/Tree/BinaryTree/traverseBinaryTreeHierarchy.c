#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

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
	Queue *q2lastone;
	
	q1.next = NULL;
	q2.next = NULL;

	q1.next = createQueueNode(_root);
	q2lastone = &q2;

	while(NULL !=  q1.next)
	{
		if(NULL != q1.next->node->lchild) {
			qIndex = createQueueNode(q1.next->node->lchild);
			if (NULL != qIndex) {
				qIndex->next = q2lastone->next;
				q2lastone->next = qIndex;
				q2lastone = q2lastone->next;
			}
		}

		if(NULL != q1.next->node->rchild) {
			qIndex = createQueueNode(q1.next->node->rchild);
			if (NULL != qIndex) {
				qIndex->next = q2lastone->next;
				q2lastone->next = qIndex;
				q2lastone = q2lastone->next;
			}
		}

		qIndex = q1.next;
		printf("%d ", qIndex->node->value);
		q1.next = qIndex->next;
		free(qIndex);

		if (NULL == q1.next) {
			q1.next = q2.next;
			q2.next = NULL;
			q2lastone = &q2;
			printf("\n");
		}
	}
}

void printTreeInHerarchyByRecursiving(Queue *_q) {
	Queue q;
	Queue *qlastone = NULL;
	Queue *qIndex = NULL;

	q.next = NULL;
	qlastone = &q;

	if (NULL == _q)
		return ;

	while(NULL != _q) {
		if (NULL != _q->node->lchild) {
			qIndex = createQueueNode(_q->node->lchild);
			if (NULL != qIndex) {
				qIndex->next = qlastone->next;
				qlastone->next = qIndex;
				qlastone = qlastone->next;
			}
		}
		if (NULL != _q->node->rchild) {
			qIndex = createQueueNode(_q->node->rchild);
			if (NULL != qIndex) {
				qIndex->next = qlastone->next;
				qlastone->next = qIndex;
				qlastone = qlastone->next;
			}
		}
		printf("%d ", _q->node->value);
		qIndex = _q;
		_q = _q->next;

		free(qIndex);
	}
	printf("\n");
	printTreeInHerarchyByRecursiving(q.next);
}

	

void printTreeInHerarchyByRecursion(Node *_root) {
	Queue *q = createQueueNode(_root);
	if(NULL != q)
		printTreeInHerarchyByRecursiving(q);
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
	printPreorderTreeWithoutRecursion(root);
	printf("\n");
	printInorderTreeWithoutRecursion(root);
	printf("\n");
	printPostorderTreeWithoutRecursion(root);
	printf("\n");

	printTreeInHerarchy(root);
	printf("\n\n");
	printTreeInHerarchyByRecursion(root);

	releaseTree(root);
	return 0;
}
