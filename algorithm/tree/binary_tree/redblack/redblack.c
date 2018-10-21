

#include "redblack.h"

#include <assert.h>


#define CHECK_NODE_RED(node) ((node) && (node)->color == RED)
#define GET_PARENT(node) ((node) ? (node)->parent : NULL)


static rb_tree_node_t* get_node()
{
	rb_tree_node_t *node = (rb_tree_node_t*)malloc(sizeof(rb_tree_node_t));
	assert(node);

	node->lchild = node->rchild = NULL;
	node->parent = NULL;
	node->color = RED;

	return node;
}

void right_rotate(rb_tree_t * tree, rb_tree_node_t *x)
{
	rb_tree_node_t *y = x->lchild;
	x->lchild = y->rchild;
	if (y->rchild)
		y->rchild->parent = x;

	y->parent = x->parent;
	if (!x->parent) {
		tree->root = y;
	}
	else if (x->parent->lchild == x) {
		x->parent->lchild = y;
	}
	else if (x->parent->rchild == x) {
		x->parent->rchild = x;
	}
	y->rchild = x;
	x->parent = y;
}

void left_rotate(rb_tree_t * tree, rb_tree_node_t *x)
{
	rb_tree_node_t *y = x->rchild;

	x->rchild = y->lchild;
	if (y->lchild)
		y->lchild->parent = x;

	y->parent = x->parent;

	if (!x->parent) {
		tree->root = y;
	}
	else if (x->parent->lchild == x) {
		x->parent->lchild = y;
	}
	else if (x->parent->rchild == x) {
		x->parent->rchild = y;
	}

	y->lchild = x;
	x->parent = y;
}

void rb_insert_fixup(rb_tree_t * tree, rb_tree_node_t *z)
{
	rb_tree_node_t *parent;
	rb_tree_node_t *pparent;
	rb_tree_node_t *y;

	while (z) {
		parent = z->parent;
		if (!parent || parent->color == BLACK)
			break;
		pparent = parent->parent;
		if (!pparent)
			break;
		
		if (parent == pparent->lchild) {
			y = pparent->rchild;
			if (CHECK_NODE_RED(y)) {
				parent->color = BLACK;
				y->color = BLACK;
				pparent->color = RED;
				z = pparent;
			}
			else {
				if (z == parent->rchild) {
					z = parent;
					left_rotate(tree, z);
				}
				
				z->parent->color = BLACK;
				if (z->parent->parent) {
					z->parent->parent->color = RED;
					right_rotate(tree, z->parent->parent);
				}
			}
		}
		else if (parent == pparent->rchild) {
			y = pparent->lchild;
			if (CHECK_NODE_RED(y)) {
				parent->color = BLACK;
				y->color = BLACK;
				pparent->color = RED;
				z = pparent;
			}
			else {
				if (z == parent->lchild) {
					z = parent;
					right_rotate(tree, z);
				}

				z->parent->color = BLACK;
				if (z->parent->parent) {
					z->parent->parent->color = RED;
					left_rotate(tree, z->parent->parent);
				}
			}
		}
		else {
			break;
		}
	}
}

void rb_tree_insert(rb_tree_t * tree, NUMTYPE key)
{
	if (!tree)
		return;
	if (!tree->root) {
		tree->root = get_node();
		tree->root->key = key;
		tree->root->color = BLACK;
		return;
	}

	rb_tree_node_t *parent = NULL;
	rb_tree_node_t *node = NULL;

	parent = tree->root;
	while (parent)
	{
		if (parent->key > key) {
			if (parent->lchild) {
				parent = parent->lchild;
			}
			else {
				node = get_node();
				node->key = key;
				node->parent = parent;
				parent->lchild = node;
				break;
			}
		}
		else if (parent->key < key) {
			if (parent->rchild) {
				parent = parent->rchild;
			}
			else {
				node = get_node();
				node->key = key;
				node->parent = parent;
				parent->rchild = node;
				break;
			}
		}
		else {
			return;
		}
	}

	rb_insert_fixup(tree, node);
	tree->root->color = BLACK;
}

bool rb_tree_search(rb_tree_t *tree, NUMTYPE key)
{
	if (NULL == tree)
		return false;

	rb_tree_node_t *node = tree->root;

	while (node) {
		if (node->key > key)
			node = node->lchild;
		else if (node->key < key)
			node = node->rchild;
		else
			return true;
	}

	return false;
}

static void _transplant(rb_tree_t *tree, rb_tree_node_t *u, rb_tree_node_t *v)
{
	if (!u->parent)
		tree->root = v;
	else if (u == u->parent->lchild)
		u->parent->lchild = v;
	else if (u == u->parent->rchild)
		u->parent->rchild = v;

	if (v)
		v->parent = u->parent;
}

static rb_tree_node_t * _get_min_one(rb_tree_node_t *node)
{
	if (!node)
		return NULL;

	while (node->lchild)
	{
		node = node->lchild;
	}

	return node;
}

static rb_tree_node_t * _get_max_one(rb_tree_node_t *node)
{
	if (!node)
		return NULL;

	while (node->rchild)
	{
		node = node->rchild;
	}

	return node;
}

static void _clear_tmp(rb_tree_node_t *node)
{
	if (!node || !node->parent)
		return;

	if (node == node->parent->lchild)
		node->parent->lchild = NULL;
	else if (node == node->parent->rchild)
		node->parent->rchild = NULL;

}

static void rb_delete_fixup(rb_tree_t *tree, rb_tree_node_t *x, bool use_tmp)
{
	rb_tree_node_t *w;
	while (x != tree->root && x->color == BLACK) {
		if (x == x->parent->lchild) {
			w = x->parent->rchild;
			if (w && w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				left_rotate(tree, x->parent);
				w = x->parent->rchild;
			}
			if (w && ((w->lchild && w->lchild->color == BLACK) || !w->lchild) && ((w->rchild && w->rchild->color == BLACK) || !w->rchild)) {
				w->color = RED;

				if (use_tmp) {
					use_tmp = false;
					_clear_tmp(x);
				}
				x = x->parent;
			}
			else {
				if (w) {
					if ((w->rchild && w->rchild->color == BLACK) || !w->rchild) {
						if (w->lchild)
							w->lchild->color = BLACK;
						w->color = RED;
						right_rotate(tree, w);
						w = x->parent->rchild;
					}

					w->color = x->parent->color;
					if (w->rchild)
						w->rchild->color = BLACK;
				}

				x->parent->color = BLACK;
				left_rotate(tree, x->parent);

				if (use_tmp) {
					use_tmp = false;
					_clear_tmp(x);
				}

				x = tree->root;
			}
		}
		else {
			w = x->parent->lchild;
			if (w && w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				right_rotate(tree, x->parent);
				w = x->parent->lchild;
			}
			if (w && ((w->lchild && w->lchild->color == BLACK) || !w->lchild) && ((w->rchild && w->rchild->color == BLACK) || !w->rchild)) {
				w->color = RED;
				
				if (use_tmp) {
					use_tmp = false;
					_clear_tmp(x);
				}
				x = x->parent;
			}
			else {
				if (w) {
					if ((w->lchild && w->lchild->color == BLACK) || !w->lchild) {
						if (w->rchild)
							w->rchild->color = BLACK;
						w->color = RED;
						left_rotate(tree, w);
						w = x->parent->lchild;
					}

					w->color = x->parent->color;
					if (w->lchild)
						w->lchild->color = BLACK;
				}
				x->parent->color = BLACK;
				right_rotate(tree, x->parent);

				if (use_tmp) {
					use_tmp = false;
					_clear_tmp(x);
				}

				x = tree->root;
			}
		}
	}

	if (x)
		x->color = BLACK;
}

static void _remove(rb_tree_t *tree, rb_tree_node_t *z)
{
	rb_tree_node_t *y = z;
	bool y_color = y->color;

	rb_tree_node_t *x = NULL;

	rb_tree_node_t tmp;
	tmp.color = BLACK;
	tmp.lchild = NULL;
	tmp.rchild = NULL;
	tmp.parent = NULL;
	bool use_tmp = false;

	if (!z->lchild) {
		x = z->rchild;
		if (!x) {
			x = &tmp;
			use_tmp = true;
		}
		_transplant(tree, z, x);
	}
	else if (!z->rchild) {
		x = z->lchild;
		if (!x) {
			x = &tmp;
			use_tmp = true;
		}
		_transplant(tree, z, x);
	}
	else {
		//y = _get_min_one(z->rchild);
		y = _get_max_one(z->lchild);
		y_color = y->color;

		//x = y->rchild;
		x = y->lchild;
		if (!x) {
			x = &tmp;
			use_tmp = true;
		}
		if (y->parent == z) {
			if (x)
				x->parent = y;
		}
		else {
			_transplant(tree, y, x);
			//y->rchild = z->rchild;
			//y->rchild->parent = y;
			y->lchild = z->lchild;
			y->lchild->parent = y;
		}

		_transplant(tree, z, y);
		//y->lchild = z->lchild;
		//y->lchild->parent = y;
		y->rchild = z->rchild;
		y->rchild->parent = y;
		y->color = z->color;
	}

	if (y_color == BLACK)
		rb_delete_fixup(tree, x, use_tmp);
	else if (use_tmp) {
		_clear_tmp(x);
	}
}

void rb_tree_delete(rb_tree_t *tree, NUMTYPE key)
{
	if (NULL == tree)
		return;

	rb_tree_node_t *node = tree->root;

	// find it
	while (node) {
		if (node->key > key)
			node = node->lchild;
		else if (node->key < key)
			node = node->rchild;
		else
			break;;
	}

	if (!node)
		return;

	// remove it
	_remove(tree, node);
	free(node);
	node = NULL;
}


static void free_subtree(rb_tree_node_t *node)
{
	if (!node)
		return;
	free_subtree(node->lchild);
	free_subtree(node->rchild);
	free(node);
}

void rb_tree_free(rb_tree_t *tree)
{
	if (tree) {
		free_subtree(tree->root);
		tree->root = NULL;
	}
}


static void _preorder(rb_tree_node_t *node)
{
	if (!node)
		return;
	printf("%d(%d) ", node->key, node->color ? 1 : 0);
	_preorder(node->lchild);
	_preorder(node->rchild);
}

static void _inorder(rb_tree_node_t *node)
{
	if (!node)
		return;
	_inorder(node->lchild);
	printf("%d(%d) ", node->key, node->color ? 1 : 0);
	_inorder(node->rchild);
}

static void _postorder(rb_tree_node_t *node)
{
	if (!node)
		return;
	_postorder(node->lchild);
	_postorder(node->rchild);
	printf("%d(%d) ", node->key, node->color ? 1 : 0);
}

void rb_tree_traverse_preorder(rb_tree_t *tree)
{
	_preorder(tree ? tree->root : NULL);
}

void rb_tree_traverse_inorder(rb_tree_t *tree)
{
	_inorder(tree ? tree->root : NULL);
}

void rb_tree_traverse_postorder(rb_tree_t *tree)
{
	_postorder(tree ? tree->root : NULL);
}