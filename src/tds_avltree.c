/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/avltree.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tds_avltree_buffer_limit  ((size_t)-1)
#define tds_avltreenode_basic_size  sizeof(struct tds_avltreenode)

struct tds_avltreenode {
	struct tds_avltreenode *__father;
	struct tds_avltreenode *__child_l;
	struct tds_avltreenode *__child_r;
	int __height;  /* `height` refers to the number of sub-tree layers */
	/* more `elesize` spaces for data storage */
};

struct tds_avltree {
	/* binary tree structure */
	struct tds_avltreenode *__root_node;

	/* tree statistics */
	size_t __len;
	size_t __elesize;

	/* buffer */
	/* buffer is a stack maintained by only `father` and `child_l` */
	size_t __buffer_limit;
	size_t __buffer_len;
	struct tds_avltreenode *__buffer_head;
};


/******************************************************************************
 * Part 1: Node level operations
 *
 * Based on only node information, we can only do LOCAL operations on the tree
 *****************************************************************************/

/* On failure, return a `NULL` pointer
 */
static struct tds_avltreenode *node_create(size_t elesize)
{
	size_t total_size = elesize + tds_avltreenode_basic_size;
	struct tds_avltreenode *node = (struct tds_avltreenode *) malloc(total_size);

	if (NULL == node) {
		printf("Error ... node_create\n");
		return NULL;
	}
	node->__father = NULL;
	node->__child_l = NULL;
	node->__child_r = NULL;
	node->__height = 1;
	return node;
}

static void node_free(struct tds_avltreenode *node)
{
	free(node);
}

static void *node_data(const struct tds_avltreenode *node)
{
	assert(NULL != node);
	return ((char *) node) + tds_avltreenode_basic_size;
}

/* Defined as "height of right" - "height of left"
 */
static int node_balace_factor(const struct tds_avltreenode *node)
{
	int depth_l = 0;
	int depth_r = 0;
	assert(NULL != node);

	if (NULL != node->__child_l)
		depth_l = node->__child_l->__height;
	if (NULL != node->__child_r)
		depth_r = node->__child_r->__height;
	return depth_r - depth_l;
}

/* Update the height of `node`, and all upper nodes
 *
 * Input: `mode`
 * 	- 0: update all upwards
 * 	- 1: lazy strategy
 * 	- 2: only update self
 */
static void node_update_height_upwardly(struct tds_avltreenode *node, int mode)
{
	int h_l = 0;
	int h_r = 0;
	int old_h = 0;
	int new_h = 0;
	assert(NULL != node);
	old_h = node->__height;

	if (NULL != node->__child_l)
		h_l = node->__child_l->__height;
	if (NULL != node->__child_r)
		h_r = node->__child_r->__height;
	new_h = 1 + tds_MAX(h_l, h_r);
	node->__height = new_h;

	if (NULL != node->__father
	&& ((0 == mode) || (1 == mode && old_h != new_h)))
		node_update_height_upwardly(node->__father, mode);
}

static int node_depth(const struct tds_avltreenode *node)
{
	int depth = 0;
	assert(NULL != node);

	if (NULL != node->__father) {
		depth++;
		node = node->__father;
	}
	return depth;
}

/******************************************************************************
 * Part 2: Binary tree related operations with buffer
 *
 * The tree structure, statistics and buffer are all adjusted, only without
 * rotation (re-balancing) operations
 *****************************************************************************/

static struct tds_avltreenode *buffer_pop(tds_avltree *tree)
{
	struct tds_avltreenode *buffer_head = NULL;
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
static int buffer_try_append( \
	tds_avltree *tree, struct tds_avltreenode *node, int force_buffer)
{
	assert(NULL != tree);
	assert(NULL != node);
	assert(tree->__buffer_limit > 0);

	if (tree->__buffer_len == tree->__buffer_limit) {
		if (force_buffer) {
			struct tds_avltreenode *head = buffer_pop(tree);
			/* head is not `NULL` since `buffer_len` > 0 */
			node_free(head);
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
static struct tds_avltreenode *binary_tree_rm_node( \
	tds_avltree *tree, struct tds_avltreenode *node, int force_buffer)
{
	struct tds_avltreenode *c_l = NULL;
	struct tds_avltreenode *c_r = NULL;
	struct tds_avltreenode *prev = NULL;
	struct tds_avltreenode *next = NULL;
	struct tds_avltreenode *father = NULL;
	int left = 0;  /* the node is the left child of its father */
	assert(NULL != tree);
	assert(NULL != node);

	c_l = node->__child_l;
	c_r = node->__child_r;
	prev = tds_avltreeiter_prev(node);
	next = tds_avltreeiter_next(node);
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
			node_update_height_upwardly(father, 0);
		} else  /* the node is a single root without children */
			tree->__root_node = NULL;
	} else if (NULL != c_l && NULL == c_r) {
		if (NULL != father) {
			if (left)
				father->__child_l = c_l;
			else
				father->__child_r = c_l;
			node_update_height_upwardly(node->__father, 0);
		} else  /* the node is the root with a single left child */
			tree->__root_node = c_l;
		c_l->__father = father;
	} else if (NULL == c_l && NULL != c_r) {
		if (NULL != father) {
			if (left)
				father->__child_l = c_r;
			else
				father->__child_r = c_r;
			node_update_height_upwardly(node->__father, 0);
		} else  /* the node is the root with a single right child */
			tree->__root_node = c_r;
		c_r->__father = father;
	} else {
		int select_prev = node_depth(prev) > node_depth(next);
		struct tds_avltreenode *selected = select_prev ? prev : next;
		father = binary_tree_rm_node(tree, selected, 1);
		/* NOTE: recursion
		 * 	The `selected` is NOT being freed since it is buffered
		 * 	Hence, its data is still reachable
		 */
		memcpy(node_data(node),node_data(selected), tree->__elesize);
		return father;
	}
	/* Adjust the tree statistics
	 */
	tree->__len--;
	/*
	 * Buffer or release the node
	 */
	if (0 == buffer_try_append(tree, node, force_buffer)) {
		node_free(node);
	}
	return father;
}

/* Add a node to the binary tree as a child of `father`
 *
 * Warning: we assume `father` is either `NULL` or in `tree`
 * Don't use this function any elsewhere!!!
 */
static struct tds_avltreenode *bintree_add_node_to( \
	tds_avltree *tree, struct tds_avltreenode *father, int left)
{
	struct tds_avltreenode *the_node = NULL;
	assert(NULL != tree);

	if (tree->__buffer_len == 0)
		the_node = node_create(tree->__elesize);
	else  /* if there is node in buffer stack */
		the_node = buffer_pop(tree);
	if (NULL == the_node)
		return NULL;
	the_node->__father = father;
	/* child_l, child_r, height are unchanged */

	if (NULL != father) {
		if (left)
			father->__child_l = the_node;
		else
			father->__child_r = the_node;
		node_update_height_upwardly(father, 1);
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
static void bintree_clear(tds_avltree *tree, struct tds_avltreenode *iter, int force_buffer)
{
	if (NULL != iter) {
		if (NULL == iter->__child_l && NULL == iter->__child_r)
			binary_tree_rm_node(tree, iter, force_buffer);
		else if (NULL != iter->__child_l && NULL == iter->__child_r) {
			bintree_clear(tree, iter->__child_l, force_buffer);
			binary_tree_rm_node(tree, iter, force_buffer);
		} else if (NULL == iter->__child_l && NULL != iter->__child_r) {
			bintree_clear(tree, iter->__child_r, force_buffer);
			binary_tree_rm_node(tree, iter, force_buffer);
		} else {
			bintree_clear(tree, iter->__child_l, force_buffer);
			bintree_clear(tree, iter->__child_r, force_buffer);
			binary_tree_rm_node(tree, iter, force_buffer);
		}
	}
}

static void bintree_clear_fast(tds_avltree *tree, struct tds_avltreenode *iter)
{
	if (NULL != iter) {
		if (NULL == iter->__child_l && NULL == iter->__child_r)
			node_free(iter);
		else if (NULL != iter->__child_l && NULL == iter->__child_r) {
			bintree_clear_fast(tree, iter->__child_l);
			node_free(iter);
		} else if (NULL == iter->__child_l && NULL != iter->__child_r) {
			bintree_clear_fast(tree, iter->__child_r);
			node_free(iter);
		} else {
			bintree_clear_fast(tree, iter->__child_l);
			bintree_clear_fast(tree, iter->__child_r);
			node_free(iter);
		}
	}
}

/******************************************************************************
 * Part 3: AVL tree related operations
 *
 *****************************************************************************/

tds_avltree *tds_avltree_create_g(size_t elesize, size_t bufferlim)
{
	tds_avltree *tree = NULL;

	assert(elesize > 0);
	assert(bufferlim > 0);

	if (NULL == (tree = (tds_avltree *) malloc(sizeof(tds_avltree)))) {
		printf("Error ... tds_avltree_create\n");
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

tds_avltree *tds_avltree_create(size_t elesize)
{
	return tds_avltree_create_g(elesize, tds_avltree_buffer_limit);
}

void tds_avltree_free_buffer(tds_avltree *tree)
{
	struct tds_avltreenode *buffer_node = tree->__buffer_head;
	struct tds_avltreenode *buffer_node_freed = NULL;
	assert(NULL != tree);
	while (NULL != buffer_node) {
		buffer_node_freed = buffer_node;
		buffer_node = buffer_node->__child_l;
		node_free(buffer_node_freed);
	}
	tree->__buffer_head = NULL;
}

void tds_avltree_free(tds_avltree *tree)
{
	assert(NULL != tree);
	bintree_clear_fast(tree, tree->__root_node);
	tds_avltree_free_buffer(tree);
	free(tree);
}

size_t tds_avltree_len(const tds_avltree *tree)
{
	return tree->__len;
}

int tds_avltree_height(const tds_avltree *tree)
{
	return tree->__root_node->__height;
}

size_t tds_avltree_elesize(const tds_avltree *tree)
{
	return tree->__elesize;
}

tds_avltreeiter *tds_avltreeiter_front(const tds_avltree *tree)
{
	tds_avltreeiter *the_node = NULL;
	assert(NULL != tree);

	the_node = tree->__root_node;
	while (NULL != the_node && NULL != the_node->__child_l)
		the_node = the_node->__child_l;
	return the_node;
}

tds_avltreeiter *tds_avltreeiter_back(const tds_avltree *tree)
{
	tds_avltreeiter *the_node = NULL;
	assert(NULL != tree);

	the_node = tree->__root_node;
	while (NULL != the_node && NULL != the_node->__child_r)
		the_node = the_node->__child_r;
	return the_node;
}

void *tds_avltree_smallest(const tds_avltree *tree)
{
	tds_avltreeiter *the_node = tds_avltreeiter_front(tree);
	return node_data(the_node);
}

void *tds_avltree_largest(const tds_avltree *tree)
{
	tds_avltreeiter *the_node = tds_avltreeiter_back(tree);
	return node_data(the_node);
}

tds_avltreeiter *tds_avltree_root(const tds_avltree *tree)
{
	return tree->__root_node;
}

void *tds_avltreeiter_data(tds_avltreeiter *iter)
{
	assert(NULL != iter);
	return node_data(iter);
}

tds_avltreeiter *tds_avltreeiter_next(tds_avltreeiter *iter)
{
	tds_avltreeiter *the_node = NULL;
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

tds_avltreeiter *tds_avltreeiter_prev(tds_avltreeiter *iter)
{
	tds_avltreeiter *the_node = NULL;
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

tds_avltreeiter *tds_avltreeiter_leftchild(const tds_avltreeiter *iter)
{
	assert(NULL != iter);
	return iter->__child_l;
}

tds_avltreeiter *tds_avltreeiter_rightchild(const tds_avltreeiter *iter)
{
	assert(NULL != iter);
	return iter->__child_r;
}

static tds_avltreeiter *avltree_getiter_g( \
	const tds_avltree *tree, void *key, tds_fcmp_t _f, int *is_left)
{
	struct tds_avltreenode *node = NULL;
	assert(NULL != tree);
	assert(NULL != key);

	node = tree->__root_node;  /* initialization */

	while (NULL != node) {
		switch (_f(key, node_data(node))) {
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

tds_avltreeiter *tds_avltree_getiter(const tds_avltree *tree, void *key, tds_fcmp_t _f)
{
	int left = 0;
	return avltree_getiter_g(tree, key, _f, &left);
}

void *tds_avltree_get(const tds_avltree *tree, void *key, tds_fcmp_t _f)
{
	struct tds_avltreenode *the_node = NULL;

	if (NULL == (the_node = tds_avltree_getiter(tree, key, _f)))
		return NULL;
	else
		return node_data(the_node);
}

/* Right rotation: Assumption: `node.child_l != NULL`
 *          |               |
 *         node            c_l
 *         / \             / \
 *       c_l  ..  =>      .  node
 *       / \                  / \
 *     .   c_lr            c_lr  ..
 */
static struct tds_avltreenode *avltree_rotation_r(struct tds_avltreenode *node)
{
	struct tds_avltreenode *c_l = NULL;
	struct tds_avltreenode *c_lr = NULL;
	assert(NULL != node);
	assert(NULL != node->__child_l);
	c_l = node->__child_l;
	c_lr = c_l->__child_r;  /* could be `NULL` */

	if (NULL != node->__father) {
		int left = (node == node->__father->__child_l);
		if (left)
			node->__father->__child_l = c_l;
		else
			node->__father->__child_r = c_l;
	}
	c_l->__father = node->__father;
	c_l->__child_r = node;
	node->__father = c_l;
	node->__child_l = c_lr;
	if (NULL != c_lr)
		c_lr->__father = node;
	node_update_height_upwardly(node, 2);
	node_update_height_upwardly(c_l, 2);
	if (NULL != c_l->__father)
		node_update_height_upwardly(c_l->__father, 1);
	return c_l;
}

/* Left rotation: Assumption: `node.child_r != NULL`
 *       |                  |
 *      node               c_r
 *      / \                / \
 *    ..   c_r    =>    node  .
 *         / \          / \
 *      c_rl  .       ..  c_rl
 */
static struct tds_avltreenode *avltree_rotation_l(struct tds_avltreenode *node)
{
	struct tds_avltreenode *c_r = NULL;
	struct tds_avltreenode *c_rl = NULL;
	assert(NULL != node);
	assert(NULL != node->__child_r);
	c_r = node->__child_r;
	c_rl = c_r->__child_l;  /* could be `NULL` */

	if (NULL != node->__father) {
		int left = (node == node->__father->__child_l);
		if (left)
			node->__father->__child_l = c_r;
		else
			node->__father->__child_r = c_r;
	}
	c_r->__father = node->__father;
	c_r->__child_l = node;
	node->__father = c_r;
	node->__child_r = c_rl;
	if (NULL != c_rl)
		c_rl->__father = node;
	node_update_height_upwardly(node, 2);
	node_update_height_upwardly(c_r, 2);
	if (NULL != c_r->__father)
		node_update_height_upwardly(c_r->__father, 1);
	return c_r;
}

/* Rebalance the `node` when it is unbalanced
 */
static struct tds_avltreenode *avltree_rebalance(struct tds_avltreenode *node)
{
	/* Case 1: RR type   Case 2: LL type   Case 3: LR type   Case 4: RL type
	 *       |                   |               |                 |
	 *      node (2)       (-2) node       (-2) node              node (2)
	 *        \                 /               /                   \
	 *        c_r (1)    (-1) c_l         (1) c_l                   c_r (-1)
	 *          \             /                 \                   /
	 *          c_rr       c_ll                 c_lr             c_rl
	 * Case 1: rota_l    Case 2: rota_r    Case 3: rota_r    Case 4: rota_l
	 */
	int bfac_node = 0;
	assert(NULL != node);
	bfac_node = node_balace_factor(node);

	if (bfac_node == 2)
		return avltree_rotation_l(node);
	if (bfac_node == -2)
		return avltree_rotation_r(node);
	return node;
}

int tds_avltree_insert(tds_avltree *tree, void *ele, tds_fcmp_t _f)
{
	struct tds_avltreenode *node = NULL;
	struct tds_avltreenode *node_father = NULL;
	int is_left = 0;

	assert(NULL != tree);
	assert(NULL != ele);
	assert(NULL != _f);

	node = tree->__root_node;  /* initialization */

	while (NULL != node) {  /* find `node_father` and `is_left` */
		node_father = node;

		/* strictly less: `ele < node_father.data` */
		if (-1 == _f(ele, node_data(node_father))) {
			node = node_father->__child_l;
			is_left = 1;
		} else {
			node = node_father->__child_r;
			is_left = 0;
		}
	}
	if (NULL == (node = bintree_add_node_to(tree, node_father, is_left))) {
		printf("Error ... tds_avltree_insert\n");
		return 0;  /* failure */
	}
	memcpy(node_data(node), ele, tree->__elesize);
	/*
	 * Re-balancing
	 * `node_father` is balanced, hence we start searching from its father
	 */
	while (NULL != node_father && NULL != node_father->__father) {
		struct tds_avltreenode *node_grandpa = node_father->__father;
		struct tds_avltreenode *tmprt = avltree_rebalance(node_grandpa);
		if (node_grandpa != tmprt) {  /* rotated */
			if (node_grandpa == tree->__root_node)
				tree->__root_node = tmprt;
			break;
		} else                                 /* unrotated */
			node_father = node_father->__father;
	}
	return 1;
}

/* The deleted node is not released but forced to be buffered
 */
int avltree_delete_g(tds_avltree *tree, void *key, tds_fcmp_t _f, int force_buffer)
{
	struct tds_avltreenode *node = NULL;
	struct tds_avltreenode *node_father = NULL;
	int left = 0;

	assert(NULL != tree);
	assert(NULL != key);
	assert(NULL != _f);

	if (NULL == (node = avltree_getiter_g(tree, key, _f, &left))) {
		printf("Error ... tds_avltree_delete\n");
		return 0;  /* failure */
	}
	node_father = binary_tree_rm_node(tree, node, force_buffer);
	/*
	 * Re-balancing
	 * We start searching from `node_father`, the father of deleted node
	 */
	while (NULL != node_father) {
		struct tds_avltreenode *tmprt = avltree_rebalance(node_father);
		if (node_father != tmprt) {  /* rotated */
			if (node_father == tree->__root_node)
				tree->__root_node = tmprt;
		} else                       /* unrotated */
			node_father = node_father->__father;
	}
	return 1;
}

int tds_avltree_delete(tds_avltree *tree, void *key, tds_fcmp_t _f)
{
	return avltree_delete_g(tree, key, _f, 0);
}
