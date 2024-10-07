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
	data = 6;
	avltree_insert(tree, &data, cmp_int);
	data = 7;
	avltree_insert(tree, &data, cmp_int);
	data = 8;
	avltree_insert(tree, &data, cmp_int);
	/*
	 *      7
	 *     / \
	 *    6   8
	 */
	assert(7 == *(int *) avltree_iter_data(avltree_root(tree)));

	data = 9;
	avltree_insert(tree, &data, cmp_int);
	data = 10;
	avltree_insert(tree, &data, cmp_int);
	/*
	 *       7                  7
	 *      / \                / \
	 *     6   8       =>     6   9
	 *          \                / \
	 *           9              8  10
	 *            \
	 *            10
	 */
	assert(3 == avltree_height(tree));
	data = 11;
	avltree_insert(tree, &data, cmp_int);
	/*
	 *       7                9
	 *      / \              / \
	 *     6   9     =>     7   10
	 *        / \          / \    \
	 *       8  10        6   8    11
	 *            \
	 *            11
	 */
	assert(3 == avltree_height(tree));
	data = 12;
	avltree_insert(tree, &data, cmp_int);
	/*
	 *       9                 9
	 *      / \              /   \
	 *     7  10       =>   7     11
	 *    / \   \          / \    / \
	 *   6   8  11        6   8  10 12
	 *            \
	 *            12
	 */
	assert(3 == avltree_height(tree));
	assert(9 == *(int *) avltree_iter_data(avltree_root(tree)));

	assert(avltree_len(tree) == 7);
	assert(avltree_elesize(tree) == sizeof(int));
	assert(*(int *)(avltree_smallest(tree)) == 6);
	assert(*(int *)(avltree_largest(tree)) == 12);

	/* test get */
	data = 8;
	assert(data == *(int *)avltree_get(tree, &data, cmp_int));

	/* test iteration forwardly */
	data = 6;
	iter = avltree_iter_front(tree);
	while (NULL != iter) {
		assert(*(int *)avltree_iter_data(iter) == data);
		data++;
		iter = avltree_iter_next(iter);
	}
	/* test iteration backwardly */
	data = 12;
	iter = avltree_iter_back(tree);
	while (NULL != iter) {
		assert(*(int *)avltree_iter_data(iter) == data);
		data--;
		iter = avltree_iter_prev(iter);
	}
	/* test delete */
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
