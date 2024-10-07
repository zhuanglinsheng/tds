/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_AVLTREE_H__
#define __T_AVLTREE_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * AVL Tree
 *
 * Balanced Binary Tree.
 *****************************************************************************/

typedef struct avltree  avltree;
typedef struct avltree_node  avltree_iter;

/* Comparing function
 *
 * Assumption:
 * 	- return  1 if a > b
 * 	- return  0 if a = b
 * 	- return -1 if a < b
 */
typedef int __fn_cmp(void *_a, void *_b);

avltree *avltree_create(size_t elesize);
avltree *avltree_create_g(size_t elesize, size_t bufferlim);

void avltree_free(avltree *tree);
void avltree_free_buffer(avltree *tree);

size_t avltree_len(const avltree *tree);
int avltree_height(const avltree *tree);
size_t avltree_elesize(const avltree *tree);
void *avltree_smallest(const avltree *tree);
void *avltree_largest(const avltree *tree);
avltree_iter *avltree_root(const avltree *tree);

void *avltree_iter_data(avltree_iter *iter);
avltree_iter *avltree_iter_front(const avltree *tree);
avltree_iter *avltree_iter_back(const avltree *tree);
avltree_iter *avltree_iter_next(avltree_iter *iter);
avltree_iter *avltree_iter_prev(avltree_iter *iter);

int avltree_insert(avltree *tree, void *ele, __fn_cmp _f);

void *avltree_get(const avltree *tree, void *key, __fn_cmp _f);
avltree_iter *avltree_get_iter(const avltree *tree, void *key, __fn_cmp _f);
avltree_iter *avltree_get_iter_g(const avltree *tree, void *key, __fn_cmp _f, int *is_left);
int avltree_delete(avltree *tree, void *key, __fn_cmp _f);

#ifdef __cplusplus
}
#endif

#endif
