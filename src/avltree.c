/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/avltree.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct avltree_node {
	struct avltree_node *__father;
	struct avltree_node *__child_l;
	struct avltree_node *__child_r;
	char __balance_factor;
	/* more `elesize` spaces for data storage */
};

#define __avltree_node_basic_size  sizeof(struct avltree_node)
#define __avltree_ABS(a)  (((a) > 0) ? (a) : (-(a)))

/*
 * `sl` mean "strickly less than"
 */
struct avltree_node *__avltree_node_create(size_t elesize, struct avltree_node *father, int sl)
{
	size_t total_size = elesize + __avltree_node_basic_size;
	struct avltree_node *node = (struct avltree_node *) malloc(total_size);

	if (NULL == node) {
		printf("Error ... __avltree_node_create\n");
		return NULL;
	}
	node->__father = father;
	node->__child_l = NULL;
	node->__child_r = NULL;
	node->__balance_factor = 0;

	if (NULL != father) {
		if (sl) {
			father->__child_l = node;
			father->__balance_factor -= 1;
		} else {
			father->__child_r = node;
			father->__balance_factor += 1;
		}
	}
	return node;
}

void *__avltree_node_data(const struct avltree_node *node)
{
	assert(NULL != node);
	return ((char *) node) + __avltree_node_basic_size;
}

void __avltree_node_free(struct avltree_node *node)
{
	free(node);
}


struct avltree {
	/* tree structure related */
	struct avltree_node *__root_node;
	struct avltree_node *__smallest_node;
	struct avltree_node *__largest_node;
	size_t __len;

	/* data related */
	size_t __elesize;

	/* buffer related */
	size_t __buffer_limit;
	size_t __buffer_len;
	struct avltree_node *__buffer_head;  /* a stack maintained by only `father` and `child_l` */
};

#define __tds_avltree_buffer_limit  16

avltree *avltree_create_gen(size_t elesize, size_t bufferlim)
{
	avltree *tree = NULL;

	assert(elesize > 0);
	assert(bufferlim > 0);

	if (NULL == (tree = (avltree *) malloc(sizeof(avltree)))) {
		printf("Error ... avltree_create\n");
		return NULL;
	}
	tree->__root_node = NULL;
	tree->__smallest_node = NULL;
	tree->__largest_node = NULL;
	tree->__len = 0;
	tree->__elesize = elesize;
	tree->__buffer_limit = bufferlim;
	tree->__buffer_len = 0;
	tree->__buffer_head = NULL;
	return tree;
}

avltree *avltree_create(size_t elesize)
{
	return avltree_create_gen(elesize, __tds_avltree_buffer_limit);
}


/* If there is room in buffer, then put the node to buffer
 * Otherwise, free the node
 */
void __avltree_try_free_node(avltree *tree, struct avltree_node *node)
{
	assert(NULL != tree);
	assert(NULL != node);

	if (tree->__buffer_len < tree->__buffer_limit) {
		node->__child_l = tree->__buffer_head;
		if (NULL != tree->__buffer_head)
			tree->__buffer_head->__father = node;
		tree->__buffer_head = node;
		tree->__buffer_len++;
	} else
		__avltree_node_free(node);
}

/* If there is node in buffer, then pick the node
 * Otherwise, create a new node
 */
struct avltree_node *__avltree_try_create_node(avltree *tree, struct avltree_node *father, int sl)
{
	struct avltree_node *the_node = NULL;

	if (tree->__buffer_len == 0)
		the_node = __avltree_node_create(tree->__elesize, father, sl);
	else {  /* if there is node in buffer stack */
		the_node = tree->__buffer_head;
		tree->__buffer_head = tree->__buffer_head->__child_l;
		if (NULL != tree->__buffer_head)
			tree->__buffer_head->__father = NULL;
		tree->__buffer_len--;
	}
	return the_node;
}


void avltree_free_buffer(avltree *tree)
{
	struct avltree_node *buffer_node = tree->__buffer_head;
	struct avltree_node *buffer_node_freed = NULL;
	assert(NULL != tree);
	while (NULL != buffer_node) {
		buffer_node_freed = buffer_node;
		buffer_node = buffer_node->__child_l;
		__avltree_node_free(buffer_node_freed);
	}
	tree->__buffer_head = NULL;
}

void avltree_free_tree(avltree *tree)
{
	avltree_iter *iter = avltree_iter_front(tree);
	avltree_iter *iter_freed = NULL;

	assert(NULL != tree);

	while (NULL != iter) {
		iter_freed = iter;
		iter = avltree_iter_next(iter);
		__avltree_try_free_node(tree, iter_freed);
	}
	tree->__len = 0;
	tree->__root_node = NULL;
	tree->__largest_node = NULL;
	tree->__smallest_node = NULL;
}

void avltree_free(avltree *tree)
{
	assert(NULL != tree);
	avltree_free_tree(tree);
	avltree_free_buffer(tree);
	free(tree);
}


size_t avltree_len(const avltree *tree)
{
	return tree->__len;
}

size_t avltree_elesize(const avltree *tree)
{
	return tree->__elesize;
}

void *avltree_smallest(const avltree *tree)
{
	assert(NULL != tree);
	return __avltree_node_data(tree->__smallest_node);
}

void *avltree_largest(const avltree *tree)
{
	assert(NULL != tree);
	return __avltree_node_data(tree->__largest_node);
}


avltree_iter *avltree_iter_front(const avltree *tree)
{
	assert(NULL != tree);
	return tree->__smallest_node;
}

avltree_iter *avltree_iter_back(const avltree *tree)
{
	assert(NULL != tree);
	return tree->__largest_node;
}

void *avltree_iter_data(avltree_iter *iter)
{
	assert(NULL != iter);
	return __avltree_node_data(iter);
}

avltree_iter *avltree_iter_next(avltree_iter *iter)
{
	avltree_iter *the_node = NULL;

	assert(NULL != iter);

	if (NULL != iter->__child_r) {
		the_node = iter->__child_r;
		while (NULL != the_node && NULL != the_node->__child_l)
			the_node = the_node->__child_l;
	} else if (NULL != iter->__father && iter == iter->__father->__child_l)
		the_node = iter->__father;
	return the_node;
}

avltree_iter *avltree_iter_prev(avltree_iter *iter)
{
	avltree_iter *the_node = NULL;

	assert(NULL != iter);

	if (NULL != iter->__child_l) {
		the_node = iter->__child_l;
		while (NULL != the_node && NULL != the_node->__child_r)
			the_node = the_node->__child_r;
	} else if (NULL != iter->__father && iter == iter->__father->__child_r)
		the_node = iter->__father;
	return the_node;
}


int avltree_append(avltree *tree, void *ele, __fn_cmp _f)
{
	struct avltree_node *node = NULL;
	struct avltree_node *node_father = NULL;
	int is_sl = 0;

	assert(NULL != tree);
	assert(NULL != ele);
	assert(NULL != _f);

	node = tree->__root_node;  /* initialization */

	while (NULL != node) {  /* find `node_father` and `left` */
		node_father = node;

		/* strictly less: `ele < node_father.data` */
		if (-1 == _f(ele, __avltree_node_data(node_father))) {
			node = node_father->__child_l;
			is_sl = 1;
		} else {
			node = node_father->__child_r;
			is_sl = 0;
		}
	}
	if (NULL == (node = __avltree_try_create_node(tree, node_father, is_sl)) ) {
		printf("Error ... avltree_append\n");
		return 0;  /* failure */
	}
	memcpy(__avltree_node_data(node), ele, tree->__elesize);

	if (0 == tree->__len) {  /* previously empty tree */
		tree->__root_node = node;
		tree->__smallest_node = node;
		tree->__largest_node = node;
	} else {
		void *smallest_dta = __avltree_node_data(tree->__smallest_node);
		void *largest_dta = __avltree_node_data(tree->__largest_node);

		if (1 == _f(smallest_dta, ele))  /* smallest > ele */
			tree->__smallest_node = node;
		if (1 == _f(ele, largest_dta))   /* ele > largest */
			tree->__largest_node = node;
	}
	tree->__len++;
	return 1;
}

void *avltree_get(avltree *tree, size_t loc)
{
	assert(NULL != tree);
	return NULL;
}

int avltree_delete(avltree *tree, size_t loc)
{
	assert(NULL != tree);
	return 0;
}
