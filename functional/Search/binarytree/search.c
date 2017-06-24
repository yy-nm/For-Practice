

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int search_context_init(struct search_context *c)
{
	if (!c)
		return -1;
	c->root = NULL;
	return 0;
}

static int _insert(struct search_context *c, int data)
{
	if (NULL == c->root) {
		c->root = (struct node *) malloc(sizeof(struct node));
		c->root->value = data;
		c->root->l = c->root->r = NULL;
		return 0;
	}

	struct node *n = c->root;
	while (n) {
		if (n->value > data) {
			if (n->l)
				n = n->l;
			else {
				n->l = (struct node *) malloc(sizeof(struct node));
				n->l->value = data;
				n->l->l = n->l->r = NULL;
				return 0;
			}
		} else {
			if (n->r)
				n = n->r;
			else {
				n->r = (struct node *) malloc(sizeof(struct node));
				n->r->value = data;
				n->r->l = n->r->r = NULL;
				return 0;
			}
		}
	}

	return 1;
}

int search_add(struct search_context *c, int data)
{
	if (!c)
		return -1;
	return _insert(c, data);
}

static int _search(struct node *root, int target, struct node **node_target, struct node **node_targetparent)
{
	if (NULL == root)
		return 1;

	struct node *nt = root;
	struct node *ntp = NULL;
	if (node_target)
		*node_target = NULL;
	if (node_targetparent)
		*node_targetparent = NULL;

	while (nt) {
		if (nt->value == target) {
			if (node_target)
				*node_target = nt;
			if (node_targetparent)
				*node_targetparent = ntp;
			return 0;
		}
		else if (nt->value > target) {
			ntp = nt;
			nt = nt->l;
		} else {
			ntp = nt;
			nt = nt->r;
		}
	}

	return 1;
}

int search(struct search_context *c, int target)
{
	if (!c)
		return -1;

	return _search(c->root, target, NULL, NULL);
}

// if have both child use right subtree leftest node replace root
static void _remove_node_in_tree(struct search_context *c, struct node *root, struct node *parent)
{
	if (!root)
		return;
	int leftorrightornull = 0;
	struct node *tmp = NULL;
	if (parent) {
		if (parent->l == root)
			leftorrightornull = -1;
		else if (parent->r == root)
			leftorrightornull = 1;
	}
	if (!root->l && !root->r) {
		free(root);
		root = NULL;
	} else if (!root->l) {
		tmp = root->r;
		free(root);
		root = tmp;
	} else if (!root->r) {
		tmp = root->l;
		free(root);
		root = tmp;
	} else {
		struct node *p = NULL;
		tmp = root->r;
		while (tmp->l) {
			p = tmp;
			tmp = tmp->l;
		}
		if (p) {
			p->l = tmp->r;
			tmp->r = root->r;
		}
		tmp->l = root->l;
		free(root);
		root = tmp;
	}

	if (-1 == leftorrightornull)
		parent->l = root;
	else if (1 == leftorrightornull)
		parent->r = root;
	else
		c->root = root;
}

int search_remove(struct search_context *c, int target)
{
	if (!c)
		return -1;

	struct node *nt, *ntp;
	int ret = _search(c->root, target, &nt, &ntp);

	if (ret)
		return ret;

	_remove_node_in_tree(c, nt, ntp);
	return 0;
}

static void _remove_whole_subtree(struct node *root)
{
	if (!root)
		return;
	if (root->l) {
		_remove_whole_subtree(root->l);
		root->l = NULL;
	}

	if (root->r) {
		_remove_whole_subtree(root->r);
		root->r = NULL;
	}
	free(root);
}

int search_collect(struct search_context *c)
{
	if (!c)
		return -1;

	_remove_whole_subtree(c->root);
	c->root = NULL;
	return 0;
}
