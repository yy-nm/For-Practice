#ifndef _SEARCH_H_
#define _SEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int (*hash_func)(void *v, int len);
typedef int (*compare)(void *l, int llen, void *r, int rlen);

struct search_node {
	unsigned int hashvalue;
	int value;
	struct search_node *next;
};

struct search_context {
	int count;
	int cap;
	struct search_node **array;
	compare comparer;
	hash_func hash;
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
