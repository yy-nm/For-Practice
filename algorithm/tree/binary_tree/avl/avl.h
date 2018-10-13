

#ifndef _AVL_H_
#define _AVL_H_


#include <stdint.h>
#include <stdlib.h>


#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define GET_LEFT_HEIGHT(x) ((x)->lchild ? (x)->lchild->height : -1)
#define GET_RIGHT_HEIGHT(x) ((x)->rchild ? (x)->rchild->height : -1)

#define AVL_HEIGHT_MAX_SUPPORT 255

// types
typedef size_t NUMTYPE;

typedef struct avl_tree_node {
	struct avl_tree_node *lchild;
	struct avl_tree_node *rchild;
	int height;

	NUMTYPE value;
} avl_tree_node_t;

typedef struct avl_tree {
	avl_tree_node_t *root;
} avl_tree_t;


// functions
int avl_tree_insert(avl_tree_t *tree, NUMTYPE value);
int avl_tree_search(avl_tree_t *tree, NUMTYPE value);
int avl_tree_delete(avl_tree_t *tree, NUMTYPE value);
int avl_tree_free(avl_tree_t *tree);

void avl_tree_traverse_preorder(avl_tree_t *tree);
void avl_tree_traverse_inorder(avl_tree_t *tree);
void avl_tree_traverse_postorder(avl_tree_t *tree);


#endif