

#ifndef _REDBLACK_H_
#define _REDBLACK_H_


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


#define REDBLACK_HEIGHT_MAX_SUPPORT 255

typedef size_t NUMTYPE;

#define RED true
#define BLACK false

typedef struct rb_tree_node {
	struct rb_tree_node *lchild;
	struct rb_tree_node *rchild;
	struct rb_tree_node *parent;
	bool color;
	
	NUMTYPE key;
} rb_tree_node_t;

typedef struct rb_tree {
	rb_tree_node_t *root;
} rb_tree_t;


// functions
void rb_tree_insert(rb_tree_t *tree, NUMTYPE key);
bool rb_tree_search(rb_tree_t *tree, NUMTYPE key);
void rb_tree_delete(rb_tree_t *tree, NUMTYPE key);
void rb_tree_free(rb_tree_t *tree);

void rb_tree_traverse_preorder(rb_tree_t *tree);
void rb_tree_traverse_inorder(rb_tree_t *tree);
void rb_tree_traverse_postorder(rb_tree_t *tree);

#endif
