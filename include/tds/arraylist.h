/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_ARRAYLIST_H__
#define __T_ARRAYLIST_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Array List
 *
 * An array list is a resizable array that can automatically adjust its size
 * to accommodate added or removed elements
 *****************************************************************************/

typedef struct tds_arraylist  tds_arraylist;

/* On failure, return `NULL`
 * The initial capacity is defined in macro `__tds_arraylist_init_len` (8)
 */
tds_arraylist *tds_arraylist_create(size_t elesize);

/* On failure, return `NULL`
 * The initial capacity is at least `__tds_arraylist_init_len`, greater
 * 	or equal to the input `capacity`
 */
tds_arraylist *tds_arraylist_create_g(size_t elesize, size_t capacity);

/* On failure, the program is stopped
 */
tds_arraylist *tds_arraylist_force_create(size_t elesize);

/* On failure, the program is stopped
 */
tds_arraylist *tds_arraylist_force_create_g(size_t elesize, size_t capacity);

void tds_arraylist_free(tds_arraylist *list);

size_t tds_arraylist_capacity(const tds_arraylist *list);
size_t tds_arraylist_len(const tds_arraylist *list);

void *tds_arraylist_get(const tds_arraylist *list, size_t loc);
void tds_arraylist_set(tds_arraylist *list, size_t loc, const void *ele);

void *tds_arraylist_getback(const tds_arraylist *list);
void tds_arraylist_setback(tds_arraylist *list, const void *ele);

/* Shadow copy contents of the pointer `ele` into `list`
 * Return a bool indicating the success
 */
int tds_arraylist_pushback(tds_arraylist *list, const void *ele);

/* Shadow copy contents of the pointer `ele` into `list`
 * On failure, exit the program
 */
void tds_arraylist_force_pushback(tds_arraylist *list, const void *ele);

/* Move all elements in `l2` into `list`
 */
void tds_arraylist_force_eat_list(tds_arraylist *list, tds_arraylist *l2);

void *tds_arraylist_popback(tds_arraylist *list);
void *tds_arraylist_delete(tds_arraylist *list, size_t idx);
void tds_arraylist_clear(tds_arraylist *list);

#ifdef __cplusplus
}
#endif

#endif
