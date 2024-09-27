/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_QUEUE_H__
#define __T_QUEUE_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tds_queue_deq  tds_queue_deq;

tds_queue_deq *tds_queue_deq_create_gen(size_t elesize, size_t capacity);
tds_queue_deq *tds_queue_deq_create(size_t elesize);
tds_queue_deq *tds_queue_deq_force_create(size_t elesize);
tds_queue_deq *tds_queue_deq_free(tds_queue_deq *q);

size_t tds_queue_deq_len(const tds_queue_deq * q);

void *tds_queue_deq_get(const tds_queue_deq * q, size_t loc);
void *tds_queue_deq_getfirst(const tds_queue_deq * q);
void *tds_queue_deq_getlast(const tds_queue_deq * q);

void tds_queue_deq_set(tds_queue_deq * q, size_t loc, const void *ele);
void tds_queue_deq_setfirst(tds_queue_deq * q, const void *ele);
void tds_queue_deq_setlast(tds_queue_deq * q, const void *ele);

int tds_queue_deq_push(tds_queue_deq * q, const void *ele);
void *tququq_pop(tds_queue_deq * q);

#ifdef __cplusplus
}
#endif

#endif
