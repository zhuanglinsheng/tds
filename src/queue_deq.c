/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/queue_deq.h>
#include <tds/array.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __tds_queue_deq_init_len  8

tds_queue_deq *tds_queue_deq_create_g(size_t elesize, size_t capacity)
{
}

tds_queue_deq *tds_queue_deq_create(size_t elesize)
{
	return tds_queue_deq_create_g(elesize, __tds_queue_deq_init_len);
}

tds_queue_deq *tds_queue_deq_force_create(size_t elesize)
{

}

tds_queue_deq *tds_queue_deq_free(tds_queue_deq *q)
{
	assert(NULL != q);
}

size_t tds_queue_deq_len(const tds_queue_deq * q)
{

}

void *tds_queue_deq_get(const tds_queue_deq * q, size_t loc)
{

}

void *tds_queue_deq_getfirst(const tds_queue_deq * q)
{

}

void *tds_queue_deq_getlast(const tds_queue_deq * q)
{

}

void tds_queue_deq_set(tds_queue_deq * q, size_t loc, const void *ele)
{

}

void tds_queue_deq_setfirst(tds_queue_deq * q, const void *ele)
{

}

void tds_queue_deq_setlast(tds_queue_deq * q, const void *ele)
{

}

int tds_queue_deq_push(tds_queue_deq * q, const void *ele)
{

}

void *tququq_pop(tds_queue_deq * q)
{

}
