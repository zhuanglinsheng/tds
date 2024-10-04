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
	avltree *tree = avltree_create(sizeof(int));
	avltree_iter *iter = NULL;
	int data = 0;

	data = 9;
	avltree_append(tree, &data, cmp_int);
	data = 8;
	avltree_append(tree, &data, cmp_int);
	data = 10;
	avltree_append(tree, &data, cmp_int);
	data = 11;
	avltree_append(tree, &data, cmp_int);
	data = 7;
	avltree_append(tree, &data, cmp_int);
	/*
	 *                9
	 *               / \
	 *              8   10
	 *             /      \
	 *            7        11
	 */
	assert(avltree_len(tree) == 5);
	assert(avltree_elesize(tree) == sizeof(int));
	assert(*(int *)(avltree_smallest(tree)) == 7);
	assert(*(int *)(avltree_largest(tree)) == 11);

	data = 7;
	iter = avltree_iter_front(tree);
	while (NULL != iter) {
		assert(*(int *)avltree_iter_data(iter) == data);
		data++;
		iter = avltree_iter_next(iter);
	}
	data = 11;
	iter = avltree_iter_back(tree);
	while (NULL != iter) {
		assert(*(int *)avltree_iter_data(iter) == data);
		data--;
		iter = avltree_iter_prev(iter);
	}
}

int main(void)
{
	test();
	return 0;
}
