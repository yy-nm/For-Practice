

#include "avl.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>


static avl_tree_node_t* get_node()
{
	avl_tree_node_t *node = (avl_tree_node_t*)malloc(sizeof(avl_tree_node_t));
	assert(node);

	node->lchild = node->rchild = NULL;
	node->height = 0;

	return node;
}

static avl_tree_node_t* rotation_left_single(avl_tree_node_t *node)
{
	avl_tree_node_t *lchild = node->lchild;
	avl_tree_node_t *subroot = NULL;

	subroot = lchild;

	node->lchild = subroot->rchild;
	subroot->rchild = node;

	node->height = MAX(GET_LEFT_HEIGHT(node), GET_RIGHT_HEIGHT(node)) + 1;
	lchild->height = MAX(GET_LEFT_HEIGHT(lchild), GET_RIGHT_HEIGHT(lchild)) + 1;
	//subroot->height = MAX(GET_LEFT_HEIGHT(subroot), GET_RIGHT_HEIGHT(subroot)) + 1;

	return subroot;
}

static avl_tree_node_t* rotation_left_double(avl_tree_node_t *node)
{
	avl_tree_node_t *lchild = node->lchild;
	avl_tree_node_t *subroot = NULL;

	subroot = lchild->rchild;

	node->lchild = subroot->rchild;
	lchild->rchild = subroot->lchild;

	subroot->lchild = lchild;
	subroot->rchild = node;

	node->height = MAX(GET_LEFT_HEIGHT(node), GET_RIGHT_HEIGHT(node)) + 1;
	lchild->height = MAX(GET_LEFT_HEIGHT(lchild), GET_RIGHT_HEIGHT(lchild)) + 1;
	subroot->height = MAX(GET_LEFT_HEIGHT(subroot), GET_RIGHT_HEIGHT(subroot)) + 1;

	return subroot;
}

static avl_tree_node_t* rotation_right_single(avl_tree_node_t *node)
{
	avl_tree_node_t *rchild = node->rchild;
	avl_tree_node_t *subroot = NULL;

	subroot = rchild;

	node->rchild = subroot->lchild;
	subroot->lchild = node;

	node->height = MAX(GET_LEFT_HEIGHT(node), GET_RIGHT_HEIGHT(node)) + 1;
	rchild->height = MAX(GET_LEFT_HEIGHT(rchild), GET_RIGHT_HEIGHT(rchild)) + 1;
	//subroot->height = MAX(GET_LEFT_HEIGHT(subroot), GET_RIGHT_HEIGHT(subroot)) + 1;

	return subroot;
}

static avl_tree_node_t* rotation_right_double(avl_tree_node_t *node)
{
	avl_tree_node_t *rchild = node->rchild;
	avl_tree_node_t *subroot = NULL;

	subroot = rchild->lchild;

	node->rchild = subroot->lchild;
	rchild->lchild = subroot->rchild;

	subroot->lchild = node;
	subroot->rchild = rchild;

	node->height = MAX(GET_LEFT_HEIGHT(node), GET_RIGHT_HEIGHT(node)) + 1;
	rchild->height = MAX(GET_LEFT_HEIGHT(rchild), GET_RIGHT_HEIGHT(rchild)) + 1;
	subroot->height = MAX(GET_LEFT_HEIGHT(subroot), GET_RIGHT_HEIGHT(subroot)) + 1;

	return subroot;
}

static void free_subtree(avl_tree_node_t *node)
{
	if (!node)
		return;
	free_subtree(node->lchild);
	free_subtree(node->rchild);
	free(node);
}

static int search(avl_tree_node_t *node, NUMTYPE value)
{
	while (node)
	{
		if (node->value == value)
			return 1;
		else if (node->value < value)
			node = node->rchild;
		else
			node = node->lchild;
	}

	return 0;
}

static void check_rotation(avl_tree_t *tree, NUMTYPE value, avl_tree_node_t **parents, int len, bool once)
{
	// check need rotation
	// check unbalances node
	int height;
	int heightl;
	int heightr;
	avl_tree_node_t *subroot = NULL;
	avl_tree_node_t *parent = NULL;

	while (len > 0) {
		parent = parents[--len];
		height = parent->height;

		if (parent->lchild)
			heightl = parent->lchild->height;
		else
			heightl = -1;

		if (parent->rchild)
			heightr = parent->rchild->height;
		else
			heightr = -1;

		if (heightl <= heightr + 1 && heightl + 1 >= heightr) {
			parent->height = MAX(heightl, heightr) + 1;
		}
		else {
			if (heightl > heightr + 1) {
				if (value > parent->lchild->value && value < parent->value)
					subroot = rotation_left_double(parent);
				else
					subroot = rotation_left_single(parent);
			}
			else if (heightl + 1 < heightr) {
				if (value > parent->value && value < parent->rchild->value)
					subroot = rotation_right_double(parent);
				else
					subroot = rotation_right_single(parent);
			}

			if (len > 0) {
				parent = parents[len - 1];
				if (parent->value > subroot->value) {
					parent->lchild = subroot;
				}
				else {
					parent->rchild = subroot;
				}
			}
			else { // change root
				tree->root = subroot;
			}

			if (once)
				break;
		}
	}
}


int avl_tree_insert(avl_tree_t *tree, NUMTYPE value)
{
	if (!tree)
		return 1;

	if (!tree->root) {
		tree->root = get_node();
		tree->root->value = value;
		return 0;
	}

	avl_tree_node_t *parents[AVL_HEIGHT_MAX_SUPPORT];
	int parentindex = 0;
	avl_tree_node_t *parent = NULL;

	parent = tree->root;
	while (parent)
	{
		assert(parentindex < AVL_HEIGHT_MAX_SUPPORT);

		if (parent->value > value) {
			if (parent->lchild) {
				parents[parentindex++] = parent;
				parent = parent->lchild;
			}
			else {
				parent->lchild = get_node();
				parent->lchild->value = value;
				parent->height = MAX(1, parent->height);
				break;
			}
		}
		else if (parent->value < value) {
			if (parent->rchild) {
				parents[parentindex++] = parent;
				parent = parent->rchild;
			}
			else {
				parent->rchild = get_node();
				parent->rchild->value = value;
				parent->height = MAX(1, parent->height);
				break;
			}
		}
		else {
			return 0;
		}
	}

	check_rotation(tree, value, parents, parentindex, true);

	return 0;
}

int avl_tree_search(avl_tree_t *tree, NUMTYPE value)
{
	return search(tree ? tree->root : NULL, value);
}

int avl_tree_delete(avl_tree_t *tree, NUMTYPE value)
{
	if (!tree)
		return 1;

	avl_tree_node_t *parents[AVL_HEIGHT_MAX_SUPPORT];
	int parentindex = 0;

	avl_tree_node_t *node = tree->root;
	
	while (node)
	{
		assert(parentindex < AVL_HEIGHT_MAX_SUPPORT);
		parents[parentindex++] = node;

		if (node->value == value)
			break;
		else if (node->value > value)
			node = node->lchild;
		else
			node = node->rchild;
	}

	// not found
	if (!node)
		return 1;

	// get it
	avl_tree_node_t *needdeletenode = node;
	bool haschild = true;
	bool hasgrandchild = false;

	if (node->rchild) {
		node = node->rchild;
		while (node->lchild) {
			hasgrandchild = true;
			assert(parentindex < AVL_HEIGHT_MAX_SUPPORT);
			parents[parentindex++] = node;
			node = node->lchild;
		}
	}
	else if (node->lchild) {
		node = node->lchild;
		while (node->rchild) {
			hasgrandchild = true;
			assert(parentindex < AVL_HEIGHT_MAX_SUPPORT);
			parents[parentindex++] = node;
			node = node->rchild;
		}
	}
	else {
		haschild = false;
		if (parentindex == 1) {
			free_subtree(node);
			tree->root = NULL;
			return 0;
		}
	}

	parentindex--;
	if (!haschild) {
		parentindex--;
	}
	
	if (hasgrandchild) {
		if (parents[parentindex]->value > node->value) {
			parents[parentindex]->lchild = node->rchild;
			node->rchild = NULL;
		}
		else {
			parents[parentindex]->rchild = node->lchild;
			node->lchild = NULL;
		}
	}
	else {
		if (parents[parentindex]->value > node->value) {
			parents[parentindex]->lchild = node->lchild;
			node->lchild = NULL;
		}
		else {
			parents[parentindex]->rchild = node->rchild;
			node->rchild = NULL;
		}
	}
	
	needdeletenode->value = node->value;
	free_subtree(node);

	check_rotation(tree, needdeletenode->value, parents, parentindex + 1, false);

	return 0;
}

int avl_tree_free(avl_tree_t *tree)
{
	if (tree) {
		free_subtree(tree->root);
		tree->root = NULL;
	}

	return 0;
}


// travrse

static void _preorder(avl_tree_node_t *node)
{
	if (!node)
		return;
	printf("%d ", node->value);
	_preorder(node->lchild);
	_preorder(node->rchild);
}

static void _inorder(avl_tree_node_t *node)
{
	if (!node)
		return;
	_inorder(node->lchild);
	printf("%d ", node->value);
	_inorder(node->rchild);
}

static void _postorder(avl_tree_node_t *node)
{
	if (!node)
		return;
	_postorder(node->lchild);
	_postorder(node->rchild);
	printf("%d ", node->value);
}

void avl_tree_traverse_preorder(avl_tree_t *tree)
{
	_preorder(tree ? tree->root : NULL);
}

void avl_tree_traverse_inorder(avl_tree_t *tree)
{
	_inorder(tree ? tree->root : NULL);
}

void avl_tree_traverse_postorder(avl_tree_t *tree)
{
	_postorder(tree ? tree->root : NULL);
}
