/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_DEQUE_H__
#define __T_DEQUE_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Deque
 *
 * Deque (short for "double-ended queue") is a versatile data structure that
 * allows insertion and removal of elements from both ends (front and back).
 * It can be thought of as a hybrid between a stack (LIFO) and a queue (FIFO)
 *****************************************************************************/

typedef struct tds_deque  tds_deque;

tds_deque *tds_deque_create(size_t elesize);
tds_deque *tds_deque_create_g(size_t elesize, size_t blk_capacity, size_t buffer_lim);
tds_deque *tds_deque_force_create(size_t elesize);
void tds_deque_free(tds_deque *q);

size_t tds_deque_len(const tds_deque * q);
size_t tds_deque_nblks(const tds_deque * q);

void *tds_deque_get(const tds_deque * q, size_t loc);
void tds_deque_set(tds_deque * q, size_t loc, void *data);

int tds_deque_pushfront(tds_deque *q, void *ele);
int tds_deque_pushback(tds_deque *q, void *ele);
void *tds_deque_popfront(tds_deque *q);
void *tds_deque_popback(tds_deque *q);

#ifdef __cplusplus
}
#endif

#endif
