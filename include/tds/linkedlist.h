/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_LIST_H__
#define __T_LIST_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Linked List
 *
 *
 *****************************************************************************/
typedef struct tds_linkedlist  tds_linkedlist;

/* Linked list iterator
 */
typedef struct tds_linkedlist_node  tds_linkedlist_iter;

tds_linkedlist *tds_linkedlist_create_gen(size_t elesize, size_t buffer_limit);
tds_linkedlist *tds_linkedlist_create(size_t elesize);

void tds_linkedlist_free_buffer(tds_linkedlist *list);
void tds_linkedlist_free(tds_linkedlist *list);

size_t tds_linkedlist_len(const tds_linkedlist *list);
size_t tds_linkedlist_buffer_len(const tds_linkedlist *list);

tds_linkedlist_iter *tds_linkedlist_iter_head(const tds_linkedlist *list);
tds_linkedlist_iter *tds_linkedlist_iter_tail(const tds_linkedlist *list);
tds_linkedlist_iter *tds_linkedlist_iter_prev(const tds_linkedlist_iter *iter);
tds_linkedlist_iter *tds_linkedlist_iter_next(const tds_linkedlist_iter *iter);

/* return the pointer to the data of `iter`
 * 	Users can read or write on this pointer, hence `get` and `set` are not provided
 */
void *tds_linkedlist_iter_data(const tds_linkedlist_iter *iter);

int tds_linkedlist_insert_before(tds_linkedlist *list, tds_linkedlist_iter *iter, void *data);
int tds_linkedlist_insert_after(tds_linkedlist *list, tds_linkedlist_iter *iter, void *data);
void tds_linkedlist_delete_node(tds_linkedlist *list, tds_linkedlist_iter *iter);

int tds_linkedlist_push_front(tds_linkedlist *list, void *data);
int tds_linkedlist_push_back(tds_linkedlist *list, void *data);

void tds_linkedlist_pop_front(tds_linkedlist *list);
void tds_linkedlist_pop_back(tds_linkedlist *list);

int tds_linkedlist_insert(tds_linkedlist *list, void *data, size_t n);
void tds_linkedlist_delete(tds_linkedlist *list, size_t n);

#ifdef __cplusplus
}
#endif

#endif
