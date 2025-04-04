/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/deque.h>
#include <tds/linkedlist.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define tds_dequq_default_blk_capacity  8
#define tds_dequq_default_buffer_limit  8

struct tds_deque_blk {
	size_t __head_loc;
	size_t __len;
	/* additional `elesize * blk_capacity` spaces for data storage */
};

#define tds_blk_basic_size  sizeof(struct tds_deque_blk)

struct tds_deque {
	tds_linkedlist *__blk_list;  /* created by linkedlist construction fn, containing blk pointers */

	size_t __blk_capacity;  /* the capacity of each block */
	size_t __elesize;
};

/******************************************************************************
 * Part 1: Block related
 *
 *****************************************************************************/

static struct tds_deque_blk *blk_create( \
		size_t elesize, size_t blk_capacity, int front_aligned)
{
	size_t total_size = tds_blk_basic_size + elesize * blk_capacity;
	struct tds_deque_blk *blk = NULL;

	assert(elesize > 0);
	assert(blk_capacity > 0);

	if (NULL == (blk = (struct tds_deque_blk *) malloc(total_size))) {
		printf("Error ... blk_create\n");
		return NULL;
	}
	blk->__head_loc = front_aligned ? 0 : blk_capacity;
	blk->__len = 0;
	return blk;
}

static void blk_free(struct tds_deque_blk *blk)
{
	free(blk);
}

static void *blk_data(const struct tds_deque_blk *blk)
{
	assert(NULL != blk);
	return ((char *) blk) + tds_blk_basic_size;
}

static void blk_push_front(struct tds_deque_blk *blk, void *data, size_t elesize)
{
	assert(NULL != blk);
	assert(NULL != data);

	assert(blk->__head_loc > 0);  /* key assumption */
	blk->__head_loc--;
	blk->__len++;
	memcpy(((char *) blk_data(blk)) + blk->__head_loc * elesize, data, elesize);
}

static void blk_push_back(struct tds_deque_blk *blk, void *data, size_t elesize, size_t blk_capacity)
{
	size_t loc = 0;

	assert(NULL != blk);
	assert(NULL != data);
	assert(elesize > 0);
	assert(blk_capacity > 0);

	loc = (blk->__head_loc) + (blk->__len);
	assert(loc < blk_capacity);  /* key assumption */

	memcpy(((char *) (blk_data(blk))) + loc * elesize, data, elesize);
	blk->__len++;
}

static void *blk_pop_front(struct tds_deque_blk *blk, size_t elesize)
{
	assert(NULL != blk);
	blk->__head_loc++;
	blk->__len--;
	return (char *)blk_data(blk) + (blk->__head_loc - 1) * elesize;
}

static void *blk_pop_back(struct tds_deque_blk *blk, size_t elesize)
{
	assert(NULL != blk);
	blk->__len--;
	return (char *)blk_data(blk) + (blk->__head_loc + blk->__len) * elesize;
}


/******************************************************************************
 * Part 2: Deque related
 *
 *****************************************************************************/

tds_deque *tds_deque_create_g(size_t elesize, size_t blk_capacity, size_t buffer_lim)
{
	tds_deque *deq = NULL;
	size_t real_blk_capacity = tds_dequq_default_blk_capacity;
	size_t real_buffer_limit = tds_dequq_default_buffer_limit;

	assert(elesize > 0);

	while (real_blk_capacity < blk_capacity)
		real_blk_capacity *= 2;
	while (real_buffer_limit < buffer_lim)
		real_buffer_limit *= 2;
	if (NULL == (deq = (tds_deque *) malloc(sizeof(tds_deque)))) {
		printf("Error ... tds_deque_create_g\n");
		return NULL;
	}
	if (NULL == (deq->__blk_list = tds_linkedlist_create_g(
					sizeof(struct tds_deque_blk *), real_buffer_limit))) {
		free(deq);
		printf("Error ... tds_deque_create_g\n");
		return NULL;
	}
	deq->__elesize = elesize;
	deq->__blk_capacity = real_blk_capacity;
	return deq;
}

tds_deque *tds_deque_create(size_t elesize)
{
	return tds_deque_create_g(elesize,\
			tds_dequq_default_blk_capacity,
			tds_dequq_default_buffer_limit);
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

void tds_deque_free(tds_deque *q)
{
	tds_linkedlistiter *iter;
	struct tds_deque_blk *blk;
	assert(NULL != q);

	iter = tds_linkedlistiter_head(q->__blk_list);
	while (NULL != iter) {
		blk = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter);
		blk_free(blk);
		iter = tds_linkedlistiter_next(iter);
	}
	tds_linkedlist_free(q->__blk_list);
	free(q);
}

size_t tds_deque_nblks(const tds_deque * q)
{
	assert(NULL != q);
	return tds_linkedlist_len(q->__blk_list);
}

static int deq_push_front_new_blk(tds_deque *q)
{
	struct tds_deque_blk *newblk = blk_create(q->__elesize, q->__blk_capacity, 0);

	if (NULL == newblk)
		return 0;  /* failure */
	if (0 == tds_linkedlist_pushfront(q->__blk_list, &newblk)) {
		blk_free(newblk);
		printf("Error ... deq_push_front_new_blk\n");
		return 0;  /* failure */
	}
	return 1;
}

static int deq_push_back_new_blk(tds_deque *q)
{
	struct tds_deque_blk *newblk = blk_create(q->__elesize, q->__blk_capacity, 1);

	if (NULL == newblk)
		return 0;  /* failure */
	if (0 == tds_linkedlist_pushback(q->__blk_list, &newblk)) {
		blk_free(newblk);
		printf("Error ... deq_push_back_new_blk\n");
		return 0;  /* failure */
	}
	return 1;
}

int tds_deque_pushfront(tds_deque *q, void *ele)
{
	tds_linkedlistiter *iter_front = NULL;
	struct tds_deque_blk *front_blk = NULL;

	assert(NULL != q);

	/* when blk list is empty, create a new block */
	if (0 == tds_linkedlist_len(q->__blk_list)
	 && 0 == deq_push_front_new_blk(q)) {
		printf("Error ... tds_deque_pushfront\n");
		return 0;
	}
	iter_front = tds_linkedlistiter_head(q->__blk_list);
	front_blk = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter_front);

	if (front_blk->__head_loc > 0) {
		/* when the front blk is not full */
		blk_push_front(front_blk, ele, q->__elesize);
		return 1;
	} else {
		/* when the front blk is full */
		if (0 == deq_push_front_new_blk(q)) {
			printf("Error ... tds_deque_pushfront\n");
			return 0;
		}
		return tds_deque_pushfront(q, ele);
	}
}

int tds_deque_pushback(tds_deque *q, void *ele)
{
	tds_linkedlistiter *iter_back = NULL;
	struct tds_deque_blk *back_blk = NULL;

	assert(NULL != q);

	/* when blk list is empty, create a new block */
	if (0 == tds_linkedlist_len(q->__blk_list) && 0 == deq_push_back_new_blk(q)) {
		printf("Error ... tds_deque_pushback\n");
		return 0;
	}
	iter_back = tds_linkedlistiter_tail(q->__blk_list);
	back_blk = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter_back);

	if (back_blk->__head_loc + back_blk->__len < q->__blk_capacity) {
		/* when the back blk is not full */
		blk_push_back(back_blk, ele, q->__elesize, q->__blk_capacity);
		return 1;
	} else {
		/* when the back blk is full */
		if (0 == deq_push_back_new_blk(q)) {
			printf("Error ... tds_deque_pushback\n");
			return 0;
		}
		return tds_deque_pushback(q, ele);
	}
	return 0;
}

void *tds_deque_popfront(tds_deque *q)
{
	tds_linkedlistiter *iter_front = NULL;
	struct tds_deque_blk *front_blk = NULL;

	assert(NULL != q);

	if (NULL == (iter_front = tds_linkedlistiter_head(q->__blk_list))) {
		printf("Error ... tds_deque_popfront\n");
		return NULL;
	}
	front_blk = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter_front);

	if (front_blk->__head_loc == q->__blk_capacity) {
		tds_linkedlist_popfront(q->__blk_list);
		return tds_deque_popfront(q);
	} else
		return blk_pop_front(front_blk, q->__elesize);
}

void *tds_deque_popback(tds_deque *q)
{
	tds_linkedlistiter *iter_back = NULL;
	struct tds_deque_blk *back_blk = NULL;

	assert(NULL != q);

	if (NULL == (iter_back = tds_linkedlistiter_tail(q->__blk_list))) {
		printf("Error ... tds_deque_popback\n");
		return NULL;
	}
	back_blk = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter_back);

	if (back_blk->__head_loc + back_blk->__len == 0) {
		tds_linkedlist_popback(q->__blk_list);
		return tds_deque_popback(q);
	} else
		return blk_pop_back(back_blk, q->__elesize);
}

size_t tds_deque_len(const tds_deque * q)
{
	tds_linkedlistiter *iter_head = NULL;
	tds_linkedlistiter *iter_tail = NULL;
	struct tds_deque_blk *blk_front = NULL;
	struct tds_deque_blk *blk_back = NULL;
	size_t nblks = 0;

	assert(NULL != q);
	nblks = tds_deque_nblks(q);

	iter_head = tds_linkedlistiter_head(q->__blk_list);
	iter_tail = tds_linkedlistiter_tail(q->__blk_list);
	blk_front = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter_head);
	blk_back = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter_tail);

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
	tds_linkedlistiter *iter;
	struct tds_deque_blk *blk;
	size_t idx = 0;

	assert(NULL != q);
	assert(loc < tds_deque_len(q));

	if (NULL == (iter = tds_linkedlistiter_head(q->__blk_list))) {
		printf("Error ... tds_deque_get\n");
		return NULL;  /* failure: loc out of index */
	}
	while (NULL != iter) {
		blk = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter);

		/* found the block */
		if (idx + blk->__len > loc) {
			char *p = blk_data(blk);
			return p + (blk->__head_loc + loc - idx) * q->__elesize;
		}
		idx += blk->__len;
		iter = tds_linkedlistiter_next(iter);
	}
	printf("Error ... tds_deque_get\n");
	return NULL;  /* failure: loc out of index */
}

void tds_deque_set(tds_deque * q, size_t loc, void *data)
{
	tds_linkedlistiter *iter;
	struct tds_deque_blk *blk;
	size_t idx = 0;

	assert(NULL != q);
	assert(loc < tds_deque_len(q));

	if (NULL == (iter = tds_linkedlistiter_head(q->__blk_list))) {
		printf("Error ... tds_deque_set\n");
		return;
	}
	while (NULL != iter) {
		blk = *(struct tds_deque_blk **) tds_linkedlistiter_data(iter);

		if (idx + blk->__len > loc) {
			char *p = (char *) blk_data(blk) + (blk->__head_loc + loc - idx) * q->__elesize;
			memcpy(p, data, q->__elesize);
			return;
		}
		idx += blk->__len;
		iter = tds_linkedlistiter_next(iter);
	}
}
