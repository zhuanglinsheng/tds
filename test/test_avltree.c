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

void test_1(void)
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
	 *     6           7
	 *      \    =>   / \
	 *       7       6   8
	 *        \
	 *         8
	 */
	assert(3 == avltree_len(tree));
	assert(2 == avltree_height(tree));
	assert(7 == *(int *) avltree_iter_data(avltree_root(tree)));
	assert(6 == *(int *) avltree_iter_data(__avltree_iter_leftchild(avltree_root(tree))));
	assert(8 == *(int *) avltree_iter_data(__avltree_iter_rightchild(avltree_root(tree))));

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
	 *       8   10       6   8    11
	 *             \
	 *              11
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
	data = 13;
	avltree_insert(tree, &data, cmp_int);
	/*
	 *         9
	 *       /   \
	 *      7     11
	 *     / \    / \
	 *    6   8  10  12
	 *                \
	 *                 13
	 */
	assert(4 == avltree_height(tree));
	assert(9 == *(int *) avltree_iter_data(avltree_root(tree)));

	assert(8 == avltree_len(tree));
	assert(sizeof(int) == avltree_elesize(tree));
	assert(6 == *(int *)(avltree_smallest(tree)));
	assert(13 == *(int *)(avltree_largest(tree)));

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
	data = 13;
	iter = avltree_iter_back(tree);
	while (NULL != iter) {
		assert(data == *(int *)avltree_iter_data(iter));
		data--;
		iter = avltree_iter_prev(iter);
	}
	/* test delete */
	data = 6;
	avltree_delete(tree, &data, cmp_int);
	/*
	 *         9
	 *       /   \
	 *      7     11
	 *       \    / \
	 *        8  10  12
	 *                \
	 *                 13
	 */
	assert(4 == avltree_height(tree));
	assert(9 == *(int *) avltree_iter_data(avltree_root(tree)));

	data = 7;
	avltree_delete(tree, &data, cmp_int);
	/*
	 *          9                  11
	 *         / \       =>       /  \
	 *        8   11             9    12
	 *            / \           / \     \
	 *           10  12        8  10     13
	 *                \
	 *                13
	 */
	assert(3 == avltree_height(tree));
	assert(11 == *(int *) avltree_iter_data(avltree_root(tree)));
	assert(9 == *(int *) avltree_iter_data(__avltree_iter_leftchild(avltree_root(tree))));
	assert(12 == *(int *) avltree_iter_data(__avltree_iter_rightchild(avltree_root(tree))));

	/* test free */
	avltree_free(tree);
}

void test_2(void)
{
	avltree *tree = avltree_create(sizeof(int));
	avltree_iter *iter = NULL;
	int data = 0;
	int __n = 100000;

	for(data = 0; data < __n; data++) {
		avltree_insert(tree, &data, cmp_int);
	}
	printf("height = %i\n", avltree_height(tree));
	printf("root = %i\n", *(int *) avltree_iter_data(avltree_root(tree)));

	/* test iteration */
	data = 0;
	iter = avltree_iter_front(tree);
	while (NULL != iter) {
		assert(data == *(int *) avltree_iter_data(iter));
		iter = avltree_iter_next(iter);
		data++;
	}

	/* search */
	data = 8241;
	assert(data == *(int *)avltree_get(tree, &data, cmp_int));

	avltree_free(tree);
}

int main(void)
{
	test_1();
	test_2();
	return 0;
}
