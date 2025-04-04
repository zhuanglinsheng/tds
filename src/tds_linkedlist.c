/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/linkedlist.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define tds_linkedlist_buffer_limit  ((size_t)-1)
#define tds_linkedlist_node_basic_size  sizeof(struct tds_linkedlist_node)

struct tds_linkedlist_node {
	struct tds_linkedlist_node *__prev;
	struct tds_linkedlist_node *__next;
	/* there will be `elesize` more spaces after the
	 * `tds_linkedlist_node` for the storage of data */
};

struct tds_linkedlist {
	size_t __elesize;
	size_t __len;
	struct tds_linkedlist_node *__head;
	struct tds_linkedlist_node *__tail;

	/* buffer stack */
	size_t __buffer_limit;
	struct tds_linkedlist_node *__buffer_head;  /* only pointer to next is valid */
};


/******************************************************************************
 * Part 1. Node related
 ******************************************************************************/

/* On success, return a linked list node whose pointer to `data` is valid
 * On failure, return `NULL` pointer
 */
static struct tds_linkedlist_node *linkedlistnode_create(size_t elesize)
{
	struct tds_linkedlist_node *node = NULL;

	if (NULL == (node = malloc(elesize + tds_linkedlist_node_basic_size))) {
		printf("Error ... linkedlistnode_create\n");
		return NULL;
	}
	return node;
}

static void *linkedlistnode_data(const struct tds_linkedlist_node *node)
{
	return ((char *) node) + tds_linkedlist_node_basic_size;
}


/******************************************************************************
 * Part 2. Buffer related
 ******************************************************************************/

/* If there is space, then store node to buffer
 * Otherwise, free the node
 */
static void store_linkedlistnode_to_buffer(struct tds_linkedlist_node *node, tds_linkedlist *list)
{
	if (tds_linkedlist_bufferlen(list) < list->__buffer_limit) {
		node->__next = list->__buffer_head;  /* maintain the pointer to next, can be NULL */
		list->__buffer_head = node;
	} else
		free(node);
}

/* First, checking the buffer stack.
 * If buffer stack is empty, try to create new node.
 * 	On failure, return `NULL` pointer.
 * If there is node in buffer, pop that node from buffer stack and return that node.
 */
static struct tds_linkedlist_node *get_linkedlistnode_for_appending(tds_linkedlist *list)
{
	struct tds_linkedlist_node *node = NULL;

	assert(NULL != list);

	if (NULL == list->__buffer_head) {
		if (NULL == (node = linkedlistnode_create(list->__elesize)))
			printf("Error ... get_linkedlistnode_for_appending\n");
	} else {
		node = list->__buffer_head;
		list->__buffer_head = node->__next;  /* pointer to next is valid */
	}
	return node;
}


/******************************************************************************
 * Part 3. List creation & free
 ******************************************************************************/

tds_linkedlist *tds_linkedlist_create_g(size_t elesize, size_t buffer_limit)
{
	tds_linkedlist *list = NULL;

	assert(elesize > 0);

	if (NULL == (list = (tds_linkedlist *) malloc(sizeof(tds_linkedlist)))) {
		printf("Error ... tds_linkedlist_create_g\n");
		return NULL;
	}
	list->__head = NULL;
	list->__tail = NULL;
	list->__elesize = elesize;
	list->__len = 0;
	list->__buffer_head = NULL;
	list->__buffer_limit = buffer_limit;
	return list;
}

tds_linkedlist *tds_linkedlist_create(size_t elesize)
{
	return tds_linkedlist_create_g(elesize, tds_linkedlist_buffer_limit);
}

void tds_linkedlist_prealloc(tds_linkedlist *list, size_t n)
{
	size_t idx = 0;
	assert(n <= list->__buffer_limit);

	for (idx = 0; idx < n; idx++) {
		struct tds_linkedlist_node *node = NULL;

		if (NULL == (node = linkedlistnode_create(list->__elesize))) {
			printf("Error ... tds_linkedlist_prealloc\n");
			break;
		}
		store_linkedlistnode_to_buffer(node, list);
	}
}

void tds_linkedlist_free_buffer(tds_linkedlist *list)
{
	assert(NULL != list);

	while (NULL != list->__buffer_head) {
		struct tds_linkedlist_node *node = list->__buffer_head;
		list->__buffer_head = node->__next;
		free(node);  /* free the whole block */
	}
}

void tds_linkedlist_free(tds_linkedlist *list)
{
	struct tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	tds_linkedlist_free_buffer(list);
	node = list->__head;

	while (NULL != node) {
		struct tds_linkedlist_node *tmp = node->__next;
		free(node);
		node = tmp;
	}
	free(list);
}


/******************************************************************************
 * Part 4. Statistics
 ******************************************************************************/

size_t tds_linkedlist_len(const tds_linkedlist *list)
{
	return list->__len;
}

size_t tds_linkedlist_bufferlen(const tds_linkedlist *list)
{
	size_t len = 0;
	struct tds_linkedlist_node *node = NULL;

	assert(NULL != list);

	node = list->__buffer_head;
	while (NULL != node) {
		node = node->__next;
		len++;
	}
	return len;
}


/******************************************************************************
 * Part 5. Iteration
 ******************************************************************************/

tds_linkedlistiter *tds_linkedlistiter_head(const tds_linkedlist *list)
{
	return list->__head;
}

tds_linkedlistiter *tds_linkedlistiter_tail(const tds_linkedlist *list)
{
	return list->__tail;
}

tds_linkedlistiter *tds_linkedlistiter_prev(const tds_linkedlistiter *iter)
{
	return iter->__prev;
}

tds_linkedlistiter *tds_linkedlistiter_next(const tds_linkedlistiter *iter)
{
	return iter->__next;
}

void *tds_linkedlistiter_data(const tds_linkedlistiter *iter)
{
	return linkedlistnode_data(iter);
}


/******************************************************************************
 * Part 6. Change List
 ******************************************************************************/

int tds_linkedlist_pushfront(tds_linkedlist *list, void *data)
{
	struct tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	assert(NULL != data);

	if (NULL == (node = get_linkedlistnode_for_appending(list))) {
		printf("Error ... tds_linkedlist_pushfront\n");
		return 0;  /* failure */
	}
	memcpy(linkedlistnode_data(node), data, list->__elesize);
	node->__prev = NULL;
	node->__next = list->__head;

	if (NULL != node->__next)
		node->__next->__prev = node;

	if (list->__len == 0) {
		list->__head = node;
		list->__tail = node;
	} else
		list->__head = node;
	list->__len++;
	return 1;
}

int tds_linkedlist_pushback(tds_linkedlist *list, void *data)
{
	struct tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	assert(NULL != data);

	if (NULL == (node = get_linkedlistnode_for_appending(list))) {
		printf("Error ... tds_linkedlist_pushback\n");
		return 0;  /* failure */
	}
	memcpy(linkedlistnode_data(node), data, list->__elesize);
	node->__next = NULL;
	node->__prev = list->__tail;
	if (NULL != node->__prev)
		node->__prev->__next = node;

	if (list->__len == 0) {
		list->__head = node;
		list->__tail = node;
	} else
		list->__tail = node;
	list->__len++;
	return 1;
}


void tds_linkedlist_popfront(tds_linkedlist *list)
{
	struct tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	assert(list->__len > 0);

	node = list->__head;
	list->__len--;

	if (list->__len == 0) {
		list->__head = NULL;
		list->__tail = NULL;
	} else {
		list->__head = node->__next;  /* set the head pointer, cannot be NULL */
		list->__head->__prev = NULL;
	}
	store_linkedlistnode_to_buffer(node, list);
}

void tds_linkedlist_popback(tds_linkedlist *list)
{
	struct tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	assert(list->__len > 0);

	node = list->__tail;
	list->__len--;

	if (list->__len == 0) {
		list->__head = NULL;
		list->__tail = NULL;
	} else {
		list->__tail = node->__prev;  /* set the last pointer, cannot be NULL */
		list->__tail->__next = NULL;
	}
	store_linkedlistnode_to_buffer(node, list);
}

int tds_linkedlist_insert(tds_linkedlist *list, void *data, size_t n)
{
	struct tds_linkedlist_node *node = NULL;
	struct tds_linkedlist_node *node_post = NULL;  /* currenty indexed by n */
	size_t idx = 0;

	assert(NULL != list);
	assert(NULL != data);
	assert(n <= list->__len);

	if (n == 0)
		return tds_linkedlist_pushfront(list, data);
	if (n == list->__len)
		return tds_linkedlist_pushback(list, data);
	/* Now 1 <= n <= len - 1 */

	node_post = list->__head;

	while (idx < n) {
		node_post = node_post->__next;
		idx++;
	}
	if (NULL == (node = get_linkedlistnode_for_appending(list))) {
		printf("Error ... tds_linkedlist_insert\n");
		return 0;  /* failure */
	}
	memcpy(linkedlistnode_data(node), data, list->__elesize);
	node->__prev = node_post->__prev;
	node->__next = node_post;

	if (NULL != node->__prev)
		node->__prev->__next = node;
	if (NULL != node->__next)
		node->__next->__prev = node;

	if (list->__len == 0) {
		list->__head = node;
		list->__tail = node;
	} else {
		if (n == list->__len)
			list->__tail = node;
		if (n == 0)
			list->__head = node;
	}
	list->__len++;
	return 1;
}

int tds_linkedlist_insert_before(tds_linkedlist *list, tds_linkedlistiter *iter, void *data)
{
	struct tds_linkedlist_node *node_new = NULL;

	assert(NULL != list);
	assert(NULL != iter);
	assert(NULL != data);

	if (NULL == (node_new = linkedlistnode_create(list->__elesize))) {
		printf("Error ... tds_linkedlist_insert_before\n");
		return 0;  /* failure */
	}
	node_new->__prev = iter->__prev;
	node_new->__next = iter;
	memcpy(linkedlistnode_data(node_new), data, list->__elesize);

	if (NULL != iter->__prev)
		iter->__prev->__next = node_new;
	else
		list->__head = node_new;

	iter->__prev = node_new;
	list->__len++;
	return 1;
}

int tds_linkedlist_insert_after(tds_linkedlist *list, tds_linkedlistiter *iter, void *data)
{
	struct tds_linkedlist_node *node_new = NULL;

	assert(NULL != list);
	assert(NULL != iter);
	assert(NULL != data);

	if (NULL == (node_new = linkedlistnode_create(list->__elesize))) {
		printf("Error ... tds_linkedlist_insert_before\n");
		return 0;  /* failure */
	}
	node_new->__prev = iter;
	node_new->__next = iter->__next;
	memcpy(linkedlistnode_data(node_new), data, list->__elesize);

	if (NULL != iter->__next)
		iter->__next->__prev = node_new;
	else
		list->__tail = node_new;

	iter->__next = node_new;
	list->__len++;
	return 1;
}

void tds_linkedlist_delete(tds_linkedlist *list, tds_linkedlistiter *iter)
{
	assert(NULL != list);
	assert(NULL != iter);

	list->__len--;

	if (list->__len == 0) {
		list->__head = NULL;
		list->__tail = NULL;
	} else {
		if (iter == list->__head)
			list->__head = iter->__next;
		if (iter == list->__tail)
			list->__tail = iter->__prev;
	}
	if (NULL != iter->__next)
		iter->__next->__prev = iter->__prev;
	if (NULL != iter->__prev)
		iter->__prev->__next = iter->__next;
	store_linkedlistnode_to_buffer(iter, list);
}

void tds_linkedlist_delete2(tds_linkedlist *list, size_t loc)
{
	struct tds_linkedlist_node *node = NULL;
	size_t idx = 0;

	assert(NULL != list);
	assert(list->__len > 0);
	assert(loc < list->__len);

	node = list->__head;

	while (idx < loc) {
		node = node->__next;
		idx++;
	}
	list->__len--;

	if (list->__len == 0) {
		list->__head = NULL;
		list->__tail = NULL;
	} else {
		if (node == list->__head)
			list->__head = node->__next;
		if (node == list->__tail)
			list->__tail = node->__prev;
	}
	if (NULL != node->__next)
		node->__next->__prev = node->__prev;
	if (NULL != node->__prev)
		node->__prev->__next = node->__next;
	store_linkedlistnode_to_buffer(node, list);
}
