/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef TDS_LIST_H
#define TDS_LIST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Linked List
 *
 * Linked list is a linear data structure in which elements, called nodes,
 * are stored in separate memory locations and are connected through pointers.
 * Each node contains two main components: the data and a pointer to the next
 * node in the sequence
 *****************************************************************************/

typedef struct tds_linkedlist  tds_linkedlist;
typedef struct tds_linkedlist_node  tds_linkedlistiter;


tds_linkedlist *tds_linkedlist_create(size_t elesize);
tds_linkedlist *tds_linkedlist_create_g(size_t elesize, size_t buffer_limit);
void tds_linkedlist_free_buffer(tds_linkedlist *list);
void tds_linkedlist_free(tds_linkedlist *list);

/* Pre-allocate the nodes that you need.
 * These pre-allocated noded will be stored in buffer
 */
void tds_linkedlist_prealloc(tds_linkedlist *list, size_t n);

size_t tds_linkedlist_len(const tds_linkedlist *list);
size_t tds_linkedlist_bufferlen(const tds_linkedlist *list);

tds_linkedlistiter *tds_linkedlistiter_head(const tds_linkedlist *list);
tds_linkedlistiter *tds_linkedlistiter_tail(const tds_linkedlist *list);
tds_linkedlistiter *tds_linkedlistiter_prev(const tds_linkedlistiter *iter);
tds_linkedlistiter *tds_linkedlistiter_next(const tds_linkedlistiter *iter);

/* return the pointer to the data of `iter`
 * 	Users can read or write on this pointer, hence `get` and `set` are not provided
 */
void *tds_linkedlistiter_data(const tds_linkedlistiter *iter);

int tds_linkedlist_pushfront(tds_linkedlist *list, void *data);
int tds_linkedlist_pushback(tds_linkedlist *list, void *data);
void tds_linkedlist_popfront(tds_linkedlist *list);
void tds_linkedlist_popback(tds_linkedlist *list);

int tds_linkedlist_insert(tds_linkedlist *list, void *data, size_t n);
int tds_linkedlist_insert_before(tds_linkedlist *list, tds_linkedlistiter *iter, void *data);
int tds_linkedlist_insert_after(tds_linkedlist *list, tds_linkedlistiter *iter, void *data);

void tds_linkedlist_delete(tds_linkedlist *list, tds_linkedlistiter *iter);
void tds_linkedlist_delete2(tds_linkedlist *list, size_t loc);

#ifdef __cplusplus
}
#endif

#endif
