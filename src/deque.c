/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/deque.h>
#include <tds/vector.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define __tdequq_blk_capacity  8

struct tds_deque {
	tds_vector *__blks;
	size_t __blk_capacity;
	size_t __elesize;
};

struct deque_blk {
	void *__data;
	size_t __head_loc;
	size_t __len;
};


tds_deque *tds_deque_create_gen(size_t elesize, size_t blk_capacity)
{
	tds_deque *deq = NULL;

	assert(elesize > 0);
	assert(blk_capacity > 0);

	if (NULL == (deq = (tds_deque *) malloc(sizeof(tds_deque)))) {
		printf("Error ... tds_deque_create_gen\n");
		return NULL;
	}
	if (NULL == (deq->__blks = tds_vector_create(sizeof(struct deque_blk *)))) {
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
	return tds_deque_create_gen(elesize, __tdequq_blk_capacity);
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
	assert(NULL != q);

	size_t i;

	for (i = 0; i < tds_dequq_nblks(q); i++) {
		struct deque_blk **blk = tds_vector_get(q->__blks, i);
	}
}

size_t tds_dequq_nblks(const tds_deque * q)
{
	assert(NULL == q);
	return tds_vector_len(q->__blks);
}

size_t tds_deque_len(const tds_deque * q)
{
	size_t idx;
	size_t len = 0;

	assert(NULL == q);

	for (idx = 0; idx < tds_vector_len(q->__blks); idx++) {
		struct deque_blk *__blk_i = tds_vector_get(q->__blks, idx);
		len += __blk_i->__len;
	}
	return len;
}

void *tds_deque_get(const tds_deque * q, size_t loc)
{
	return NULL;
}

void tds_deque_set(tds_deque * q, size_t loc, void *data)
{
}

int tds_deque_push_front(tds_deque *q, void *ele)
{
	return 0;
}

int tds_deque_push_back(tds_deque *q, void *ele)
{
	return 0;
}

void *tds_deque_pop_front(tds_deque *q)
{
	return NULL;
}

void *tds_deque_pop_back(tds_deque *q)
{
	return NULL;
}
