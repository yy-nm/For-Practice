
#include <stdio.h>
#include <string.h>

#include "search.h"


int len_test_source = 10;
int test_source[] = { 1, 2, 3, 10, 4, 6, 1000, 8, 9, 10 };

int len_test_search = 5;
int test_search_array[] = { 3, 10, 4, 10000, 1 };
int test_search_result[] = { 0, 0, 0, 1, 0 };

int len_test_delete = 10;
int test_search_delete_array[] = { 1, 2, 3, 10, 4, 6, 1000, 8, 9, 10 };
int test_search_delete_result[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int len_test_search_again = 5;
int test_search_again_array[] = { 3, 10, 4, 10000, 1 };
int test_search_again_result[] = { 1, 1, 1, 1, 1 };


// str search
int len_test_str_source = 10;
char* test_str_source[] = { "1", "2", "3", "10", "4", "6", "10001", "8", "9", "10" };

int len_test_str_search = 5;
char* test_str_search_array[] = { "3", "10", "4", "10000", "1" };
int test_str_search_result[] = { 0, 0, 0, 1, 0 };

int len_test_str_delete = 10;
char* test_str_search_delete_array[] = { "1", "2", "3", "10", "4", "6", "1000", "8", "9", "10" };
int test_str_search_delete_result[] = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 };

int len_test_str_search_again = 5;
char* test_str_search_again_array[] = { "3", "10", "4", "10000", "1" };
int test_str_search_again_result[] = { 1, 1, 1, 1, 1 };


int main(int argc, char *argv[])
{
	int i;
	struct search_context c;
	search_context_init(&c);

	for (i = 0; i < len_test_source; i++) {
		search_add(&c, &test_source[i], sizeof(test_source[i]));
	}

	for (i = 0; i < len_test_search; i++) {
		if (search(&c, &test_search_array[i], sizeof(test_search_array[i])) != test_search_result[i]) {
			printf("search error: i: %d\n", i);
			return -1;
		}
	}

	for (i = 0; i< len_test_delete; i++) {
		if (search_remove(&c, &test_search_delete_array[i], sizeof(test_search_delete_array[i])) != test_search_delete_result[i]) {
			printf("delete error: i: %d\n", i);
			return -2;
		}
	}

	for (i = 0; i < len_test_search_again; i++) {
		if (search(&c, &test_search_again_array[i], sizeof(test_search_again_array[i])) != test_search_again_result[i]) {
			printf("search again error: i: %d\n", i);
			return -1;
		}
	}

	search_collect(&c);
	printf("int part test pass\n");

	search_context_init(&c);

	for (i = 0; i < len_test_str_source; i++) {
		search_add(&c, test_str_source[i], strlen(test_str_source[i]));
	}

	for (i = 0; i < len_test_str_search; i++) {
		if (search(&c, test_str_search_array[i], strlen(test_str_search_array[i])) != test_str_search_result[i]) {
			printf("search error: i: %d\n", i);
			return -1;
		}
	}

	for (i = 0; i< len_test_str_delete; i++) {
		if (search_remove(&c, test_str_search_delete_array[i], strlen(test_str_search_delete_array[i])) != test_str_search_delete_result[i]) {
			printf("delete error: i: %d\n", i);
			return -2;
		}
	}

	for (i = 0; i < len_test_str_search_again; i++) {
		if (search(&c, test_str_search_again_array[i], strlen(test_str_search_again_array[i])) != test_str_search_again_result[i]) {
			printf("search again error: i: %d\n", i);
			return -1;
		}
	}

	search_collect(&c);
	printf("str part test pass\n");

	printf("all test pass\n");
	return 0;
}
