/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/avltree.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __avltree_buffer_limit  ((size_t)-1)
#define __avltree_node_basic_size  sizeof(struct avltree_node)

#define __avltree_ABS(a)  (((a) > 0) ? (a) : (-(a)))
#define __avltree_MAX(a, b) (((a) > (b)) ? (a) : (b))

struct avltree_node {
	struct avltree_node *__father;
	struct avltree_node *__child_l;
	struct avltree_node *__child_r;
	int __height;  /* `height` refers to the number of sub-tree layers */
	int __depth;   /* `depth` refers to the distance to the root of tree */
	/* more `elesize` spaces for data storage */
};

struct avltree {
	/* binary tree structure */
	struct avltree_node *__root_node;

	/* tree statistics */
	size_t __len;
	size_t __elesize;

	/* buffer */
	/* buffer is a stack maintained by only `father` and `child_l` */
	size_t __buffer_limit;
	size_t __buffer_len;
	struct avltree_node *__buffer_head;
};


/******************************************************************************
 * Part 1: Node level operations
 *
 * Based on only node information, we can only do LOCAL operations on the tree
 *****************************************************************************/

/* Return a `NULL` pointer on failure
 */
struct avltree_node *__node_create(size_t elesize)
{
	size_t total_size = elesize + __avltree_node_basic_size;
	struct avltree_node *node = (struct avltree_node *) malloc(total_size);

	if (NULL == node) {
		printf("Error ... __node_create\n");
		return NULL;
	}
	node->__father = NULL;
	node->__child_l = NULL;
	node->__child_r = NULL;
	node->__height = 1;
	node->__depth = 0;
	return node;
}

void __node_free(struct avltree_node *node)
{
	free(node);
}

void *__node_data(const struct avltree_node *node)
{
	assert(NULL != node);
	return ((char *) node) + __avltree_node_basic_size;
}

int __node_balace_factor(const struct avltree_node *node)
{
	int depth_l = 0;
	int depth_r = 0;
	assert(NULL != node);

	if (NULL != node->__child_l)
		depth_l = node->__child_l->__height;
	if (NULL != node->__child_r)
		depth_r = node->__child_r->__height;
	return depth_l - depth_r;
}

/* Local operation of binary tree:
 * Update the height of `node` and all upper nodes
 */
void __node_update_height_upwardly(struct avltree_node *node)
{
	int h_l = 0;
	int h_r = 0;
	assert(NULL != node);

	if (NULL != node->__child_l)
		h_l = node->__child_l->__height;
	if (NULL != node->__child_r)
		h_r = node->__child_r->__height;
	node->__height = 1 + __avltree_MAX(h_l, h_r);
	if (NULL != node->__father)
		__node_update_height_upwardly(node->__father);
}

/* Local operation of binary tree:
 * Update the depth of `node` and all children
 */
void __node_update_depth_downwardly(struct avltree_node *node)
{
	int depth = 0;
	struct avltree_node *the_node = node;
	assert(NULL != node);

	/* calculate depth */
	while (NULL != the_node->__father) {
		depth++;
		the_node = the_node->__father;
	}
	node->__depth = depth;

	/* update left child */
	the_node = node;
	while (NULL != the_node->__child_l) {
		the_node->__child_l->__depth = the_node->__depth + 1;
		the_node = the_node->__child_l;
	}
	/* update right child */
	the_node = node;
	while (NULL != the_node->__child_r) {
			the_node->__child_r->__depth = the_node->__depth + 1;
			the_node = the_node->__child_r;
	}
}


/******************************************************************************
 * Part 2: Binary tree related operations with buffer
 *
 * The tree structure, statistics and buffer are all adjusted, only without
 * rotation (re-balancing) operations
 *****************************************************************************/

struct avltree_node * __buffer_pop(avltree *tree)
{
	struct avltree_node *buffer_head = NULL;
	assert(NULL != tree);

	if (NULL != tree->__buffer_head) {
		buffer_head = tree->__buffer_head;
		tree->__buffer_head = buffer_head->__child_l;
		if (NULL != tree->__buffer_head)
			tree->__buffer_head->__father = NULL;
		tree->__buffer_len--;
	}
	return buffer_head;
}

/* Return a bool indicating success
 * If unsussessful, the `node` is unchanged
 */
int __buffer_try_append(avltree *tree, struct avltree_node *node, int force_buffer)
{
	assert(NULL != tree);
	assert(NULL != node);
	assert(tree->__buffer_limit > 0);

	if (tree->__buffer_len == tree->__buffer_limit) {
		if (force_buffer) {
			struct avltree_node *head = __buffer_pop(tree);
			/* head is not `NULL` since `buffer_len` > 0 */
			__node_free(head);
		} else
			return 0;  /* failure */
	}
	node->__father = NULL;
	node->__child_l = tree->__buffer_head;  /* could be NULL */
	if (NULL != tree->__buffer_head)
		tree->__buffer_head->__father = node;
	tree->__buffer_head = node;
	tree->__buffer_len++;
	return 1;
}

/* Remove `node` from the binary tree
 *
 * Warning: we assume that `node` is in `tree`
 * Don't use this function any elsewhere!!!
 */
void __bintree_rm_node(avltree *tree, struct avltree_node *node, int force_buffer)
{
	struct avltree_node *c_l = NULL;
	struct avltree_node *c_r = NULL;
	struct avltree_node *prev = NULL;
	struct avltree_node *next = NULL;
	struct avltree_node *father = NULL;
	int left = 0;  /* the node is the left child of its father */
	assert(NULL != tree);
	assert(NULL != node);

	c_l = node->__child_l;
	c_r = node->__child_r;
	prev = avltree_iter_prev(node);
	next = avltree_iter_next(node);
	if (NULL != (father = node->__father))
		left = (node == father->__child_l);
	/*
	 * Adjust the binary tree structure
	 */
	if (NULL == c_l && NULL == c_r) {
		if (NULL != father) {
			if (left)
				father->__child_l = NULL;
			else
				father->__child_r = NULL;
			__node_update_height_upwardly(father);
		} else  /* the node is a single root without children */
			tree->__root_node = NULL;
	} else if (NULL != c_l && NULL == c_r) {
		if (NULL != father) {
			if (left)
				father->__child_l = c_l;
			else
				father->__child_r = c_l;
			__node_update_height_upwardly(node->__father);
		} else  /* the node is the root with a single left child */
			tree->__root_node = c_l;
		c_l->__father = father;
		__node_update_depth_downwardly(c_l);
	} else if (NULL == c_l && NULL != c_r) {
		if (NULL != father) {
			if (left)
				father->__child_l = c_r;
			else
				father->__child_r = c_r;
			__node_update_height_upwardly(node->__father);
		} else  /* the node is the root with a single right child */
			tree->__root_node = c_r;
		c_r->__father = father;
		__node_update_depth_downwardly(c_r);
	} else {
		int select_prev = prev->__depth > next->__depth;
		struct avltree_node *selected = select_prev ? prev : next;
		__bintree_rm_node(tree, selected, 1);
		/* NOTE: recursion
		 * 	The `selected` is NOT being freed since it is buffered
		 * 	Hence, its data is still reachable
		 */
		memcpy(__node_data(node),__node_data(selected), tree->__elesize);
		return;
	}
	/* Adjust the tree statistics
	 */
	tree->__len--;
	/*
	 * Buffer or release the node
	 */
	if (0 == __buffer_try_append(tree, node, force_buffer)) {
		__node_free(node);
	}
}

/* Add a node to the binary tree as a child of `father`
 *
 * Warning: we assume `father` is either `NULL` or in `tree`
 * Don't use this function any elsewhere!!!
 */
struct avltree_node *__bintree_add_node_to(avltree *tree, struct avltree_node *father, int left)
{
	struct avltree_node *the_node = NULL;
	assert(NULL != tree);

	if (tree->__buffer_len == 0)
		the_node = __node_create(tree->__elesize);
	else  /* if there is node in buffer stack */
		the_node = __buffer_pop(tree);
	if (NULL == the_node)
		return NULL;
	the_node->__father = father;
	__node_update_depth_downwardly(the_node);
	/* child_l, child_r, height are unchanged */

	if (NULL != father) {
		if (left)
			father->__child_l = the_node;
		else
			father->__child_r = the_node;
		__node_update_height_upwardly(father);
	}
	if (0 == tree->__len) {  /* previously, the tree is empty */
		tree->__root_node = the_node;
	}
	tree->__len++;
	return the_node;
}

/* Warning: the input `iter` must be `tree->__root_node`
 * Don't use this function any elsewhere!!!
 */
void __bintree_clear(avltree *tree, struct avltree_node *iter, int force_buffer)
{
	if (NULL != iter) {
		if (NULL == iter->__child_l && NULL == iter->__child_r)
			__bintree_rm_node(tree, iter, force_buffer);
		else if (NULL != iter->__child_l && NULL == iter->__child_r) {
			__bintree_clear(tree, iter->__child_l, force_buffer);
			__bintree_rm_node(tree, iter, force_buffer);
		} else if (NULL == iter->__child_l && NULL != iter->__child_r) {
			__bintree_clear(tree, iter->__child_r, force_buffer);
			__bintree_rm_node(tree, iter, force_buffer);
		} else {
			__bintree_clear(tree, iter->__child_l, force_buffer);
			__bintree_clear(tree, iter->__child_r, force_buffer);
			__bintree_rm_node(tree, iter, force_buffer);
		}
	}
}


/******************************************************************************
 * Part 3: AVL tree related operations
 *
 *****************************************************************************/

avltree *avltree_create_g(size_t elesize, size_t bufferlim)
{
	avltree *tree = NULL;

	assert(elesize > 0);
	assert(bufferlim > 0);

	if (NULL == (tree = (avltree *) malloc(sizeof(avltree)))) {
		printf("Error ... avltree_create\n");
		return NULL;
	}
	tree->__root_node = NULL;
	tree->__len = 0;
	tree->__elesize = elesize;
	tree->__buffer_limit = bufferlim;
	tree->__buffer_len = 0;
	tree->__buffer_head = NULL;
	return tree;
}

avltree *avltree_create(size_t elesize)
{
	return avltree_create_g(elesize, __avltree_buffer_limit);
}

void avltree_free_buffer(avltree *tree)
{
	struct avltree_node *buffer_node = tree->__buffer_head;
	struct avltree_node *buffer_node_freed = NULL;
	assert(NULL != tree);
	while (NULL != buffer_node) {
		buffer_node_freed = buffer_node;
		buffer_node = buffer_node->__child_l;
		__node_free(buffer_node_freed);
	}
	tree->__buffer_head = NULL;
}

void avltree_free(avltree *tree)
{
	assert(NULL != tree);
	__bintree_clear(tree, tree->__root_node, 0);
	avltree_free_buffer(tree);
	free(tree);
}

size_t avltree_len(const avltree *tree)
{
	return tree->__len;
}

int avltree_height(const avltree *tree)
{
	return tree->__root_node->__height;
}

size_t avltree_elesize(const avltree *tree)
{
	return tree->__elesize;
}

avltree_iter *avltree_iter_front(const avltree *tree)
{
	avltree_iter *the_node = NULL;
	assert(NULL != tree);

	the_node = tree->__root_node;
	while (NULL != the_node && NULL != the_node->__child_l)
		the_node = the_node->__child_l;
	return the_node;
}

avltree_iter *avltree_iter_back(const avltree *tree)
{
	avltree_iter *the_node = NULL;
	assert(NULL != tree);

	the_node = tree->__root_node;
	while (NULL != the_node && NULL != the_node->__child_r)
		the_node = the_node->__child_r;
	return the_node;
}

void *avltree_smallest(const avltree *tree)
{
	avltree_iter *the_node = avltree_iter_front(tree);
	return __node_data(the_node);
}

void *avltree_largest(const avltree *tree)
{
	avltree_iter *the_node = avltree_iter_back(tree);
	return __node_data(the_node);
}

avltree_iter *avltree_root(const avltree *tree)
{
	return tree->__root_node;
}

void *avltree_iter_data(avltree_iter *iter)
{
	assert(NULL != iter);
	return __node_data(iter);
}

avltree_iter *avltree_iter_next(avltree_iter *iter)
{
	avltree_iter *the_node = NULL;
	assert(NULL != iter);

	if (NULL != iter->__child_r) {
	/*
	 * If `iter` has right child, then the node is the most left
	 *     child among all `iter`'s right child
	 */
		the_node = iter->__child_r;
		while (NULL != the_node && NULL != the_node->__child_l)
			the_node = the_node->__child_l;
		return the_node;
	} else {
	/*
	 * If `iter` doesn't have right child, then the node is the
	 *     father if it is the left child of its father. If not, find
	 *     father's father.
	 */
		the_node = iter;
	Loop:
		if (NULL != the_node->__father) {
			if (the_node == the_node->__father->__child_l)
				return the_node->__father;
			else {
				the_node = the_node->__father;
				goto Loop;
			}
		}
	}
	return NULL;  /* failure */
}

avltree_iter *avltree_iter_prev(avltree_iter *iter)
{
	avltree_iter *the_node = NULL;
	assert(NULL != iter);

	if (NULL != iter->__child_l) {
	/*
	 * If `iter` has left child, then the node is the most right
	 *     child among all `iter`'s left child
	 */
		the_node = iter->__child_l;
		while (NULL != the_node && NULL != the_node->__child_r)
			the_node = the_node->__child_r;
		return the_node;
	} else {
	/*
	 * If `iter` doesn't have left child, then the node is the
	 *     father if it is the right child of its father. If not, find
	 *     father's father.
	 */
		the_node = iter;
	Loop:
		if (NULL != the_node->__father) {
			if (the_node == the_node->__father->__child_r)
				return the_node->__father;
			else {
				the_node = the_node->__father;
				goto Loop;
			}
		}
	}
	return NULL;  /* failure */
}

avltree_iter *avltree_get_iter_g(const avltree *tree, void *key, __fn_cmp _f, int *is_left)
{
	struct avltree_node *node = NULL;
	assert(NULL != tree);
	assert(NULL != key);

	node = tree->__root_node;  /* initialization */

	while (NULL != node) {
		switch (_f(key, __node_data(node))) {
		case -1:  /* key < node */
			node = node->__child_l;
			*is_left = 1;
			break;
		case 0:  /* key = node */
			return node;
		case 1:  /* key > node */
			node = node->__child_r;
			*is_left = 0;
			break;
		}
	}
	return NULL;  /* failure */
}

avltree_iter *avltree_get_iter(const avltree *tree, void *key, __fn_cmp _f)
{
	int left = 0;
	return avltree_get_iter_g(tree, key, _f, &left);
}

void *avltree_get(const avltree *tree, void *key, __fn_cmp _f)
{
	struct avltree_node *the_node = NULL;

	if (NULL == (the_node = avltree_get_iter(tree, key, _f)))
		return NULL;
	else
		return __node_data(the_node);
}

int avltree_insert(avltree *tree, void *ele, __fn_cmp _f)
{
	struct avltree_node *node = NULL;
	struct avltree_node *node_father = NULL;
	int is_left = 0;

	assert(NULL != tree);
	assert(NULL != ele);
	assert(NULL != _f);

	node = tree->__root_node;  /* initialization */

	while (NULL != node) {  /* find `node_father` and `is_left` */
		node_father = node;

		/* strictly less: `ele < node_father.data` */
		if (-1 == _f(ele, __node_data(node_father))) {
			node = node_father->__child_l;
			is_left = 1;
		} else {
			node = node_father->__child_r;
			is_left = 0;
		}
	}
	if (NULL == (node = __bintree_add_node_to(tree, node_father, is_left))) {
		printf("Error ... avltree_insert\n");
		return 0;  /* failure */
	}
	memcpy(__node_data(node), ele, tree->__elesize);
	return 1;
}

/* The deleted node is not released but forced to be buffered
 */
int avltree_delete_g(avltree *tree, void *key, __fn_cmp _f, int force_buffer)
{
	struct avltree_node *the_node = NULL;
	int left = 0;

	assert(NULL != tree);
	assert(NULL != key);
	assert(NULL != _f);

	if (NULL == (the_node = avltree_get_iter_g(tree, key, _f, &left))) {
		printf("Error ... avltree_delete\n");
		return 0;  /* failure */
	}
	__bintree_rm_node(tree, the_node, force_buffer);
	/*
	 * Update tree statistics
	 */

	return 1;
}

int avltree_delete(avltree *tree, void *key, __fn_cmp _f)
{
	return avltree_delete_g(tree, key, _f, 0);
}
