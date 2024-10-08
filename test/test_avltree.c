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
	tds_avltree *tree = tds_avltree_create_g(sizeof(int), 1);
	tds_avltreeiter *iter = NULL;
	int data = 0;

	/* Inserting
	 */
	data = 6;
	tds_avltree_insert(tree, &data, cmp_int);
	data = 7;
	tds_avltree_insert(tree, &data, cmp_int);
	data = 8;
	tds_avltree_insert(tree, &data, cmp_int);
	/*
	 *     6           7
	 *      \    =>   / \
	 *       7       6   8
	 *        \
	 *         8
	 */
	assert(3 == tds_avltree_len(tree));
	assert(2 == tds_avltree_height(tree));
	assert(7 == *(int *) tds_avltreeiter_data(tds_avltree_root(tree)));
	assert(6 == *(int *) tds_avltreeiter_data(tds_avltreeiter_leftchild(tds_avltree_root(tree))));
	assert(8 == *(int *) tds_avltreeiter_data(tds_avltreeiter_rightchild(tds_avltree_root(tree))));

	data = 9;
	tds_avltree_insert(tree, &data, cmp_int);
	data = 10;
	tds_avltree_insert(tree, &data, cmp_int);
	/*
	 *       7                  7
	 *      / \                / \
	 *     6   8       =>     6   9
	 *          \                / \
	 *           9              8  10
	 *            \
	 *            10
	 */
	assert(3 == tds_avltree_height(tree));

	data = 11;
	tds_avltree_insert(tree, &data, cmp_int);
	/*
	 *       7                9
	 *      / \              / \
	 *     6   9     =>     7   10
	 *        / \          / \    \
	 *       8   10       6   8    11
	 *             \
	 *              11
	 */
	assert(3 == tds_avltree_height(tree));
	data = 12;
	tds_avltree_insert(tree, &data, cmp_int);
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
	tds_avltree_insert(tree, &data, cmp_int);
	/*
	 *         9
	 *       /   \
	 *      7     11
	 *     / \    / \
	 *    6   8  10  12
	 *                \
	 *                 13
	 */
	assert(4 == tds_avltree_height(tree));
	assert(9 == *(int *) tds_avltreeiter_data(tds_avltree_root(tree)));

	assert(8 == tds_avltree_len(tree));
	assert(sizeof(int) == tds_avltree_elesize(tree));
	assert(6 == *(int *)(tds_avltree_smallest(tree)));
	assert(13 == *(int *)(tds_avltree_largest(tree)));

	/* test get */
	data = 8;
	assert(data == *(int *)tds_avltree_get(tree, &data, cmp_int));

	/* test iteration forwardly */
	data = 6;
	iter = tds_avltreeiter_front(tree);
	while (NULL != iter) {
		assert(*(int *)tds_avltreeiter_data(iter) == data);
		data++;
		iter = tds_avltreeiter_next(iter);
	}
	/* test iteration backwardly */
	data = 13;
	iter = tds_avltreeiter_back(tree);
	while (NULL != iter) {
		assert(data == *(int *)tds_avltreeiter_data(iter));
		data--;
		iter = tds_avltreeiter_prev(iter);
	}
	/* test delete */
	data = 6;
	tds_avltree_delete(tree, &data, cmp_int);
	/*
	 *         9
	 *       /   \
	 *      7     11
	 *       \    / \
	 *        8  10  12
	 *                \
	 *                 13
	 */
	assert(4 == tds_avltree_height(tree));
	assert(9 == *(int *) tds_avltreeiter_data(tds_avltree_root(tree)));

	data = 7;
	tds_avltree_delete(tree, &data, cmp_int);
	/*
	 *          9                  11
	 *         / \       =>       /  \
	 *        8   11             9    12
	 *            / \           / \     \
	 *           10  12        8  10     13
	 *                \
	 *                13
	 */
	assert(3 == tds_avltree_height(tree));
	assert(11 == *(int *) tds_avltreeiter_data(tds_avltree_root(tree)));
	assert(9 == *(int *) tds_avltreeiter_data(tds_avltreeiter_leftchild(tds_avltree_root(tree))));
	assert(12 == *(int *) tds_avltreeiter_data(tds_avltreeiter_rightchild(tds_avltree_root(tree))));

	/* test free */
	tds_avltree_free(tree);
}

void test_2(void)
{
	tds_avltree *tree = tds_avltree_create(sizeof(int));
	tds_avltreeiter *iter = NULL;
	int data = 0;
	int __n = 100000;

	for(data = 0; data < __n; data++) {
		tds_avltree_insert(tree, &data, cmp_int);
	}
	printf("height = %i\n", tds_avltree_height(tree));
	printf("root = %i\n", *(int *) tds_avltreeiter_data(tds_avltree_root(tree)));

	/* test iteration */
	data = 0;
	iter = tds_avltreeiter_front(tree);
	while (NULL != iter) {
		assert(data == *(int *) tds_avltreeiter_data(iter));
		iter = tds_avltreeiter_next(iter);
		data++;
	}

	/* search */
	data = 8241;
	assert(data == *(int *)tds_avltree_get(tree, &data, cmp_int));

	tds_avltree_free(tree);
}

int main(void)
{
	test_1();
	test_2();
	return 0;
}
