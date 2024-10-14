/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_AVLTREE_H__
#define __T_AVLTREE_H__

#include <stddef.h>
#include <tds.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * AVL Tree
 *
 * AVL Tree is a balanced binary tree
 *****************************************************************************/

typedef struct tds_avltree  tds_avltree;
typedef struct tds_avltreenode  tds_avltreeiter;

tds_avltree *tds_avltree_create(size_t elesize);
tds_avltree *tds_avltree_create_g(size_t elesize, size_t bufferlim);

void tds_avltree_free(tds_avltree *tree);
void tds_avltree_free_buffer(tds_avltree *tree);

size_t tds_avltree_len(const tds_avltree *tree);
int tds_avltree_height(const tds_avltree *tree);
size_t tds_avltree_elesize(const tds_avltree *tree);
void *tds_avltree_smallest(const tds_avltree *tree);
void *tds_avltree_largest(const tds_avltree *tree);

void *tds_avltreeiter_data(tds_avltreeiter *iter);
tds_avltreeiter *tds_avltreeiter_front(const tds_avltree *tree);
tds_avltreeiter *tds_avltreeiter_back(const tds_avltree *tree);
tds_avltreeiter *tds_avltreeiter_next(tds_avltreeiter *iter);
tds_avltreeiter *tds_avltreeiter_prev(tds_avltreeiter *iter);

tds_avltreeiter *tds_avltree_root(const tds_avltree *tree);
tds_avltreeiter *tds_avltreeiter_leftchild(const tds_avltreeiter *iter);
tds_avltreeiter *tds_avltreeiter_rightchild(const tds_avltreeiter *iter);

int tds_avltree_insert(tds_avltree *tree, void *ele, tds_fcmp_t _f);
int tds_avltree_delete(tds_avltree *tree, void *key, tds_fcmp_t _f);

void *tds_avltree_get(const tds_avltree *tree, void *key, tds_fcmp_t _f);
tds_avltreeiter *tds_avltree_getiter(const tds_avltree *tree, void *key, tds_fcmp_t _f);

#ifdef __cplusplus
}
#endif

#endif
