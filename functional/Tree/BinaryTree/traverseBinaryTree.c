#include <stdio.h>
#include <stdbool.h>

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

typedef struct _dequeue {
	struct dequeue_node {
		Node *value;
		struct dequeue_node *next;
		struct dequeue_node *pre;
	};

	struct dequeue_node *head;
	struct dequeue_node *tail;
	int count;
} dequeue;


struct dequeue_node* create_dequeue_node() {
	struct dequeue_node *node = (struct dequeue_node *)malloc(sizeof(struct dequeue_node));
	node->next = NULL;
	node->pre = NULL;
	node->value = NULL;
}

void release_dequeue_node(struct dequeue_node* node) {
	if (node) {
		free(node);
	}
}

void dequeue_init(dequeue *q) {
	if (!q)
		return;
	q->head = NULL;
	q->tail = NULL;
	q->count = 0;
}

void dequeue_release(dequeue *q) {
	if (!q)
		return;
	struct dequeue_node* node;
	while (q->head != NULL) {
		node = q->head;
		q->head = q->head->next;
		release_dequeue_node(node);
	}

	q->head = NULL;
	q->tail = NULL;
	q->count = 0;
}

void dequeue_push(dequeue *q, Node *node)
{
	if (!q)
		return;

	struct dequeue_node *n = create_dequeue_node();
	n->value = node;

	if (q->count == 0) {
		q->head = q->tail = n;
	}
	else {
		n->pre = q->tail;
		q->tail->next = n;
		q->tail = q->tail->next;
	}

	q->count++;
}

void dequeue_pushFront(dequeue *q, Node *node)
{
	if (!q)
		return;

	struct dequeue_node *n = create_dequeue_node();
	n->value = node;

	if (q->count == 0) {
		q->head = q->tail = n;
	}
	else {
		n->next = q->head;
		q->head->pre = n;
		q->head = q->head->pre;
	}

	q->count++;
}

Node * dequeue_pop(dequeue *q)
{
	if (!q)
		return NULL;
	if (!q->tail)
		return NULL;

	struct dequeue_node *node = q->tail;
	Node *n = node->value;
	q->tail = q->tail->pre;
	if (q->tail)
		q->tail->next = NULL;
	q->count--;
	if (q->count == 0) {
		q->head = NULL;
	}

	release_dequeue_node(node);
	return n;
}

Node* dequeue_popFront(dequeue *q)
{
	if (!q)
		return NULL;
	if (!q->head)
		return NULL;

	struct dequeue_node *node = q->head;
	Node *n = node->value;
	q->head = q->head->next;
	if (q->head)
		q->head->pre = NULL;
	q->count--;
	if (q->count == 0) {
		q->tail = NULL;
	}

	release_dequeue_node(node);
	return n;
}


void printPreorderTreeWithoutRecursion(Node *_node)
{
	if (NULL == _node)
		return;
	dequeue q;
	dequeue_init(&q);

	dequeue_push(&q, _node);

	Node * n;
	while (q.count > 0) {
		n = dequeue_pop(&q);
		printf("%d ", n->value);
		if (n->rchild)
			dequeue_push(&q, n->rchild);
		if (n->lchild)
			dequeue_push(&q, n->lchild);
	}
}

void printInorderTreeWithoutRecursion(Node *_node)
{
	if (NULL == _node)
		return;
	//printf("no-implements");
	dequeue q;
	dequeue_init(&q);
	dequeue_push(&q, _node);
	dequeue q_cutleftnode;
	dequeue_init(&q_cutleftnode);

	Node * n;
	while (q.count > 0 || q_cutleftnode.count > 0) {
		n = dequeue_pop(&q);
		if (n) {
			while (n->lchild) {
				dequeue_push(&q_cutleftnode, n);
				n = n->lchild;
			}
		} else {
			n = dequeue_pop(&q_cutleftnode);
		}
		
		printf("%d ", n->value);
		if (n->rchild)
			dequeue_push(&q, n->rchild);
	}
}

void printPostorderTreeWithoutRecursion(Node *_node)
{
	if (NULL == _node)
		return;
	//printf("no-implements");
	dequeue q;
	dequeue_init(&q);
	dequeue_push(&q, _node);
	dequeue q_out;
	dequeue_init(&q_out);
	Node * n;
	
	while (q.count > 0) {
		n = dequeue_pop(&q);
		if (n->lchild) {
			dequeue_push(&q, n->lchild);
		}
		if (n->rchild) {
			dequeue_push(&q, n->rchild);
		}
		
		dequeue_push(&q_out, n);
	}

	while (q_out.count > 0) {
		n = dequeue_pop(&q_out);
		printf("%d ", n->value);
	}
}

