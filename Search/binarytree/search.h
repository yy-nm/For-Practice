#ifndef _SEARCH_H_
#define _SEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

struct node {
	int value;
	struct node *l;
	struct node *r;
};

struct search_context {
	struct node *root;
};


int search_context_init(struct search_context *c);
int search_add(struct search_context *c, int data);
int search(struct search_context *c, int target);
int search_remove(struct search_context *c, int target);
int search_collect(struct search_context *c);

#ifdef __cplusplus
}
#endif


#endif // _SEARCH_H_
