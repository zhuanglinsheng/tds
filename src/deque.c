/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/deque.h>
#include <tds/linkedlist.h>
#include <tds/bitarray.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct tds_deque_blk {
	size_t __head_loc;
	size_t __len;
	/* additional `elesize * blk_capacity` spaces for data storage */
};

#define __blk_basic_size  sizeof(struct tds_deque_blk)

struct tds_deque_blk *__blk_create(size_t elesize, size_t blk_capacity, int front_aligned)
{
	size_t total_size = __blk_basic_size + elesize * blk_capacity;
	struct tds_deque_blk *blk = NULL;

	assert(elesize > 0);
	assert(blk_capacity > 0);

	if (NULL == (blk = (struct tds_deque_blk *) malloc(total_size))) {
		printf("Error ... __blk_create\n");
		return NULL;
	}
	blk->__head_loc = front_aligned ? 0 : blk_capacity;
	blk->__len = 0;
	return blk;
}

void __blk_free(struct tds_deque_blk *blk)
{
	free(blk);
}

void *__blk_data(const struct tds_deque_blk *blk)
{
	assert(NULL != blk);
	return ((char *) blk) + __blk_basic_size;
}

void __blk_push_front(struct tds_deque_blk *blk, void *data, size_t elesize)
{
	assert(NULL != blk);
	assert(NULL != data);

	assert(blk->__head_loc > 0);  /* key assumption */
	blk->__head_loc--;
	blk->__len++;
	memcpy(((char *) __blk_data(blk)) + blk->__head_loc * elesize, data, elesize);
}

void __blk_push_back(struct tds_deque_blk *blk, void *data, size_t elesize, size_t blk_capacity)
{
	size_t loc = 0;

	assert(NULL != blk);
	assert(NULL != data);
	assert(elesize > 0);
	assert(blk_capacity > 0);

	loc = (blk->__head_loc) + (blk->__len);
	assert(loc < blk_capacity);  /* key assumption */

	memcpy(((char *) (__blk_data(blk))) + loc * elesize, data, elesize);
	blk->__len++;
}

void *__blk_pop_front(struct tds_deque_blk *blk, size_t elesize)
{
	assert(NULL != blk);
	blk->__head_loc++;
	blk->__len--;
	return (char *)__blk_data(blk) + (blk->__head_loc - 1) * elesize;
}

void *__blk_pop_back(struct tds_deque_blk *blk, size_t elesize)
{
	assert(NULL != blk);
	blk->__len--;
	return (char *)__blk_data(blk) + (blk->__head_loc + blk->__len) * elesize;
}


#define __tds_dequq_default_blk_capacity  8
#define __tds_dequq_default_buffer_limit  8

struct tds_deque {
	tds_linkedlist *__blk_list;  /* created by linkedlist construction fn, containing blk pointers */
	tds_bitarray *__blk_aligned;

	size_t __blk_capacity;  /* the capacity of each block */
	size_t __elesize;
};

tds_deque *tds_deque_create_gen(size_t elesize, size_t blk_capacity, size_t buffer_lim)
{
	tds_deque *deq = NULL;
	size_t real_blk_capacity = __tds_dequq_default_blk_capacity;
	size_t real_buffer_limit = __tds_dequq_default_buffer_limit;

	assert(elesize > 0);

	while (real_blk_capacity < blk_capacity)
		real_blk_capacity *= 2;
	while (real_buffer_limit < buffer_lim)
		real_buffer_limit *= 2;
	if (NULL == (deq = (tds_deque *) malloc(sizeof(tds_deque)))) {
		printf("Error ... tds_deque_create_gen\n");
		return NULL;
	}
	if (NULL == (deq->__blk_list = tds_linkedlist_create_gen(
					sizeof(struct tds_deque_blk *), real_buffer_limit))) {
		free(deq);
		printf("Error ... tds_deque_create_gen\n");
		return NULL;
	}
	deq->__elesize = elesize;
	deq->__blk_capacity = real_blk_capacity;
	return deq;
}

tds_deque *tds_deque_create(size_t elesize)
{
	return tds_deque_create_gen(elesize,\
			__tds_dequq_default_blk_capacity,
			__tds_dequq_default_buffer_limit);
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
	tds_linkedlist_node *iter;
	struct tds_deque_blk *blk;
	assert(NULL != q);

	iter = tds_linkedlist_head(q->__blk_list);
	while (NULL != iter) {
		blk = *(struct tds_deque_blk **) tds_linkedlist_node_data(iter);
		__blk_free(blk);
		iter = tds_linkedlist_node_next(iter);
	}
	tds_linkedlist_free(q->__blk_list);
	free(q);
}

size_t tds_deque_nblks(const tds_deque * q)
{
	assert(NULL != q);
	return tds_linkedlist_len(q->__blk_list);
}

static int __deq_push_front_new_blk(tds_deque *q)
{
	struct tds_deque_blk *newblk = __blk_create(q->__elesize, q->__blk_capacity, 0);

	if (NULL == newblk)
		return 0;  /* failure */
	if (0 == tds_linkedlist_push_front(q->__blk_list, &newblk)) {
		__blk_free(newblk);
		printf("Error ... __deq_push_front_new_blk\n");
		return 0;  /* failure */
	}
	return 1;
}

static int __deq_push_back_new_blk(tds_deque *q)
{
	struct tds_deque_blk *newblk = __blk_create(q->__elesize, q->__blk_capacity, 1);

	if (NULL == newblk)
		return 0;  /* failure */
	if (0 == tds_linkedlist_push_back(q->__blk_list, &newblk)) {
		__blk_free(newblk);
		printf("Error ... __deq_push_back_new_blk\n");
		return 0;  /* failure */
	}
	return 1;
}

int tds_deque_push_front(tds_deque *q, void *ele)
{
	tds_linkedlist_node *front_node = NULL;
	struct tds_deque_blk *front_blk = NULL;

	assert(NULL != q);

	/* when blk list is empty, create a new block */
	if (0 == tds_linkedlist_len(q->__blk_list)
	 && 0 == __deq_push_front_new_blk(q)) {
		printf("Error ... tds_deque_push_front\n");
		return 0;
	}
	front_node = tds_linkedlist_head(q->__blk_list);
	front_blk = *(struct tds_deque_blk **) tds_linkedlist_node_data(front_node);

	if (front_blk->__head_loc > 0) {
		/* when the front blk is not full */
		__blk_push_front(front_blk, ele, q->__elesize);
		return 1;
	} else {
		/* when the front blk is full */
		if (0 == __deq_push_front_new_blk(q)) {
			printf("Error ... tds_deque_push_front\n");
			return 0;
		}
		return tds_deque_push_front(q, ele);
	}
}

int tds_deque_push_back(tds_deque *q, void *ele)
{
	tds_linkedlist_node *back_node = NULL;
	struct tds_deque_blk *back_blk = NULL;

	assert(NULL != q);

	/* when blk list is empty, create a new block */
	if (0 == tds_linkedlist_len(q->__blk_list) && 0 == __deq_push_back_new_blk(q)) {
		printf("Error ... tds_deque_push_back\n");
		return 0;
	}
	back_node = tds_linkedlist_tail(q->__blk_list);
	back_blk = *(struct tds_deque_blk **) tds_linkedlist_node_data(back_node);

	if (back_blk->__head_loc + back_blk->__len < q->__blk_capacity) {
		/* when the back blk is not full */
		__blk_push_back(back_blk, ele, q->__elesize, q->__blk_capacity);
		return 1;
	} else {
		/* when the back blk is full */
		if (0 == __deq_push_back_new_blk(q)) {
			printf("Error ... tds_deque_push_back\n");
			return 0;
		}
		return tds_deque_push_back(q, ele);
	}
	return 0;
}

void *tds_deque_pop_front(tds_deque *q)
{
	tds_linkedlist_node *front_node = NULL;
	struct tds_deque_blk *front_blk = NULL;

	assert(NULL != q);

	if (NULL == (front_node = tds_linkedlist_head(q->__blk_list))) {
		printf("Error ... tds_deque_pop_front\n");
		return NULL;
	}
	front_blk = *(struct tds_deque_blk **) tds_linkedlist_node_data(front_node);

	if (front_blk->__head_loc == q->__blk_capacity) {
		tds_linkedlist_pop_front(q->__blk_list);
		return tds_deque_pop_front(q);
	} else
		return __blk_pop_front(front_blk, q->__elesize);
}

void *tds_deque_pop_back(tds_deque *q)
{
	tds_linkedlist_node *back_node = NULL;
	struct tds_deque_blk *back_blk = NULL;

	assert(NULL != q);

	if (NULL == (back_node = tds_linkedlist_tail(q->__blk_list))) {
		printf("Error ... tds_deque_pop_back\n");
		return NULL;
	}
	back_blk = *(struct tds_deque_blk **) tds_linkedlist_node_data(back_node);

	if (back_blk->__head_loc + back_blk->__len == 0) {
		tds_linkedlist_pop_back(q->__blk_list);
		return tds_deque_pop_back(q);
	} else
		return __blk_pop_back(back_blk, q->__elesize);
}

size_t tds_deque_len(const tds_deque * q)
{
	tds_linkedlist_node *iter_head = NULL;
	tds_linkedlist_node *iter_tail = NULL;
	struct tds_deque_blk *blk_front = NULL;
	struct tds_deque_blk *blk_back = NULL;
	size_t nblks = 0;

	assert(NULL != q);
	nblks = tds_deque_nblks(q);

	iter_head = tds_linkedlist_head(q->__blk_list);
	iter_tail = tds_linkedlist_tail(q->__blk_list);
	blk_front = *(struct tds_deque_blk **) tds_linkedlist_node_data(iter_head);
	blk_back = *(struct tds_deque_blk **) tds_linkedlist_node_data(iter_tail);

	switch (nblks) {
	case 0:
		return 0;
	case 1:
		return blk_front->__len;
	default:
		return blk_front->__len + blk_back->__len + (nblks - 2) * q->__blk_capacity;
	}
}

void *tds_deque_get(const tds_deque * q, size_t loc)
{
	tds_linkedlist_node *iter;
	struct tds_deque_blk *blk;
	size_t idx = 0;

	assert(NULL != q);
	assert(loc < tds_deque_len(q));

	if (NULL == (iter = tds_linkedlist_head(q->__blk_list))) {
		printf("Error ... tds_deque_get\n");
		return NULL;
	}
	while (NULL != iter) {
		blk = *(struct tds_deque_blk **) tds_linkedlist_node_data(iter);

		if (idx + blk->__len > loc)
			return __blk_data(blk) + (blk->__head_loc + loc - idx) * q->__elesize;
		idx += blk->__len;
		iter = tds_linkedlist_node_next(iter);
	}
}

void tds_deque_set(tds_deque * q, size_t loc, void *data)
{
	tds_linkedlist_node *iter;
	struct tds_deque_blk *blk;
	size_t idx = 0;

	assert(NULL != q);
	assert(loc < tds_deque_len(q));

	if (NULL == (iter = tds_linkedlist_head(q->__blk_list))) {
		printf("Error ... tds_deque_set\n");
		return;
	}
	while (NULL != iter) {
		blk = *(struct tds_deque_blk **) tds_linkedlist_node_data(iter);

		if (idx + blk->__len > loc) {
			char *p = (char *) __blk_data(blk) + (blk->__head_loc + loc - idx) * q->__elesize;
			memcpy(p, data, q->__elesize);
			return;
		}
		idx += blk->__len;
		iter = tds_linkedlist_node_next(iter);
	}
}
