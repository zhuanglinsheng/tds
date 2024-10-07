#include <tds/avltree.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

int cmp_int(void *a, void *b)
{
	int *pa = (int *)a;
	int *pb = (int *)b;

	if (*pa > *pb)
		return 1;
	else if (*pa == *pb)
		return 0;
	else
		return -1;
}

void test(void)
{
	/* Creation
	 */
	avltree *tree = avltree_create_g(sizeof(int), 1);
	avltree_iter *iter = NULL;
	int data = 0;

	/* Inserting
	 */
	data = 9;
	avltree_insert(tree, &data, cmp_int);
	data = 7;
	avltree_insert(tree, &data, cmp_int);
	data = 11;
	avltree_insert(tree, &data, cmp_int);
	data = 12;
	avltree_insert(tree, &data, cmp_int);
	data = 6;
	avltree_insert(tree, &data, cmp_int);
	data = 8;
	avltree_insert(tree, &data, cmp_int);
	data = 10;
	avltree_insert(tree, &data, cmp_int);
	/*
	 *                 9
	 *               /   \
	 *              7     11
	 *             / \    / \
	 *            6   8 10   12
	 */
	assert(avltree_len(tree) == 7);
	assert(avltree_elesize(tree) == sizeof(int));
	assert(*(int *)(avltree_smallest(tree)) == 6);
	assert(*(int *)(avltree_largest(tree)) == 12);
	assert(3 == avltree_height(tree));
	data = 8;
	assert(data == *(int *)avltree_get(tree, &data, cmp_int));

	data = 6;
	iter = avltree_iter_front(tree);
	while (NULL != iter) {
		assert(*(int *)avltree_iter_data(iter) == data);
		printf("%i\n", *(int *)avltree_iter_data(iter));
		data++;
		iter = avltree_iter_next(iter);
	}
	data = 12;
	iter = avltree_iter_back(tree);
	while (NULL != iter) {
		assert(*(int *)avltree_iter_data(iter) == data);
		data--;
		iter = avltree_iter_prev(iter);
	}

	data = 8;
	avltree_delete(tree, &data, cmp_int);

	data = 9;
	avltree_delete(tree, &data, cmp_int);

	data = 6;
	iter = avltree_get_iter(tree, &data, cmp_int);
	printf("get the iterator of %i\n", *(int *)avltree_iter_data(iter));

	while (NULL != iter) {
		printf("next = %i\n", *(int *)avltree_iter_data(iter));
		iter = avltree_iter_next(iter);
	}
	avltree_free(tree);
}

int main(void)
{
	test();
	return 0;
}
