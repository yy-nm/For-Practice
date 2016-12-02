#ifndef _SEARCH_H_
#define _SEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef unsigned int (*hash_func)(void *v, int len);
typedef int (*compare)(void *l, int llen, void *r, int rlen);



#ifndef USE_SELF_ITEM_TYPE
typedef void search_item_t;
#endif

#ifndef MALLOC
#define MALLOC(sz) malloc(sz)
#endif

#ifndef FREE
#define FREE(p) free(p)
#endif



struct search_node {
	unsigned int hashvalue;
	int sz;
	search_item_t *value;
	struct search_node *next;
};

struct search_context {
	int count;
	int cap;
	struct search_node **array;
	compare comparer;
	hash_func hash;
};

unsigned int hash_func_SDBM(void *v, int len);
unsigned int hash_func_DJB(void *v, int len);
unsigned int hash_func_DEK(void *v, int len);


int search_context_init(struct search_context *c);
int search_context_sethash(struct search_context *c, hash_func func);
int search_context_setcomparer(struct search_context *c, compare func);

int search_add(struct search_context *c, search_item_t *data, int sz);
int search(struct search_context *c, search_item_t *target, int sz);
int search_remove(struct search_context *c, search_item_t *target, int sz);
int search_collect(struct search_context *c);

#ifdef __cplusplus
}
#endif


#endif // _SEARCH_H_
