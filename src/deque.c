/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/deque.h>
#include <tds/arraylist.h>
#include <tds/linkedlist.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define __tdequq_blk_capacity      8
#define __tdequq_blk_buffer_limit  8

struct tds_deque {
	tds_linkedlist *__blks;  /* created by linkedlist construction function*/
	size_t __blk_capacity;
	size_t __elesize;
};

struct tds_deque_blk {
	size_t __head_loc;
	size_t __len;
	void *__data;
	/* additional `elesize * blk_capacity` spaces for data storage */
};


tds_deque *tds_deque_create_gen(size_t elesize, size_t blk_capacity, size_t blks_buffer_lim)
{
	tds_deque *deq = NULL;
	size_t data_size = elesize * blk_capacity;
	size_t blk_basic_size = sizeof(struct tds_deque_blk);
	size_t blk_total_size = blk_basic_size + data_size;

	assert(elesize > 0);
	assert(blk_capacity > 0);

	if (NULL == (deq = (tds_deque *) malloc(sizeof(tds_deque)))) {
		printf("Error ... tds_deque_create_gen\n");
		return NULL;
	}
	if (NULL == (deq->__blks = tds_linkedlist_create_gen(blk_total_size, blks_buffer_lim))) {
		free(deq);
		printf("Error ... tds_deque_create_gen\n");
		return NULL;
	}
	deq->__blk_capacity = blk_capacity;
	deq->__elesize = elesize;
	return deq;
}

tds_deque *tds_deque_create(size_t elesize)
{
	return tds_deque_create_gen(elesize, __tdequq_blk_capacity, __tdequq_blk_buffer_limit);
}

tds_deque *tds_deque_force_create(size_t elesize)
{
	tds_deque *deq = tds_deque_create(elesize);

	if (NULL == deq) {
		printf("Error ... tds_deque_force_create\n");
		exit(-1);
	}
	return deq;
}

tds_deque *tds_deque_free(tds_deque *q)
{
	assert(NULL == q);
}

size_t tds_dequq_nblks(const tds_deque * q)
{
	assert(NULL == q);
}

size_t tds_deque_len(const tds_deque * q)
{
	assert(NULL == q);
}

void *tds_deque_get(const tds_deque * q, size_t loc)
{
	assert(NULL == q);
	return NULL;
}

void tds_deque_set(tds_deque * q, size_t loc, void *data)
{
	assert(NULL == q);
}

int tds_deque_push_front(tds_deque *q, void *ele)
{
	assert(NULL == q);
	return 0;
}

int tds_deque_push_back(tds_deque *q, void *ele)
{
	assert(NULL == q);
	return 0;
}

void *tds_deque_pop_front(tds_deque *q)
{
	assert(NULL == q);
	return NULL;
}

void *tds_deque_pop_back(tds_deque *q)
{
	assert(NULL == q);
	return NULL;
}
