/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/linkedlist.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define __tds_linkedlist_buffer_limit  8

struct tds_linkedlist_node
{
	struct tds_linkedlist_node *__prev;
	struct tds_linkedlist_node *__next;
	void *__data;
	/* there will be `elesize` more spaces after the `tds_linkedlist_node` for the storage of data */
};

struct tds_linkedlist
{
	tds_linkedlist_node *__head;
	tds_linkedlist_node *__tail;
	size_t __elesize;
	size_t __len;

	/* buffer stack */
	tds_linkedlist_node *__buffer_head;  /* only pointer to next is valid */
	size_t __buffer_limit;
};


/* On success, return a linked list node whose pointer to `data` is valid
 * On failure, return `NULL` pointer
 */
static tds_linkedlist_node *__create_linkedlist_node(size_t elesize)
{
	void *_blk_space = NULL;
	tds_linkedlist_node *node = NULL;

	size_t basic_size = sizeof(tds_linkedlist_node);

	if (NULL == (_blk_space = malloc(basic_size + elesize))) {
		printf("Error ... __create_linkedlist_node\n");
		return NULL;
	}
	node = (tds_linkedlist_node *)_blk_space;
	node->__data = (char *)_blk_space + basic_size;
	return node;
}

/* If there is space, then store node to buffer
 * Otherwise, free the node
 */
static void __store_node_to_linkedlist_buffer(tds_linkedlist_node *node, tds_linkedlist *list)
{
	if (tds_linkedlist_buffer_len(list) < list->__buffer_limit) {
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
static tds_linkedlist_node *__get_linkedlist_node_for_appending(tds_linkedlist *list)
{
	tds_linkedlist_node *node = NULL;

	assert(NULL != list);

	if (NULL == list->__buffer_head) {
		if (NULL == (node = __create_linkedlist_node(list->__elesize)))
			printf("Error ... __get_linkedlist_node_for_appending\n");
	} else {
		node = list->__buffer_head;
		list->__buffer_head = node->__next;  /* pointer to next is valid */
	}
	return node;
}

tds_linkedlist *tds_linkedlist_create_gen(size_t elesize, size_t buffer_limit)
{
	tds_linkedlist *list = NULL;

	assert(elesize > 0);

	if (NULL == (list = (tds_linkedlist *) malloc(sizeof(tds_linkedlist)))) {
		printf("Error ... tds_linkedlist_create_gen\n");
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
	return tds_linkedlist_create_gen(elesize, __tds_linkedlist_buffer_limit);
}

void tds_linkedlist_free_buffer(tds_linkedlist *list)
{
	assert(NULL != list);

	while (NULL != list->__buffer_head) {
		tds_linkedlist_node *node = list->__buffer_head;
		list->__buffer_head = node->__next;
		free(node);  /* free the whole block */
	}
}

void tds_linkedlist_free(tds_linkedlist *list)
{
	tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	tds_linkedlist_free_buffer(list);
	node = list->__head;

	while (NULL != node) {
		tds_linkedlist_node *tmp = node->__next;
		free(node);
		node = tmp;
	}
	free(list);
}


size_t tds_linkedlist_len(const tds_linkedlist *list)
{
	return list->__len;
}

size_t tds_linkedlist_buffer_len(const tds_linkedlist *list)
{
	size_t len = 0;
	tds_linkedlist_node *node = NULL;

	assert(NULL != list);

	node = list->__buffer_head;
	while (NULL != node) {
		node = node->__next;
		len++;
	}
	return len;
}


tds_linkedlist_node *tds_linkedlist_head(const tds_linkedlist *list)
{
	return list->__head;
}

tds_linkedlist_node *tds_linkedlist_tail(const tds_linkedlist *list)
{
	return list->__tail;
}

tds_linkedlist_node *tds_linkedlist_node_previous(const tds_linkedlist_node *iter)
{
	return iter->__prev;
}

tds_linkedlist_node *tds_linkedlist_node_next(const tds_linkedlist_node *iter)
{
	return iter->__next;
}

void *tds_linkedlist_node_data(const tds_linkedlist_node *iter)
{
	return iter->__data;
}


int tds_linkedlist_push_front(tds_linkedlist *list, void *data)
{
	tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	assert(NULL != data);

	if (NULL == (node = __get_linkedlist_node_for_appending(list))) {
		printf("Error ... tds_linkedlist_push_front\n");
		return 0;  /* failure */
	}
	memcpy(node->__data, data, list->__elesize);  /* node->__data is a valid pointer to a free space */
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

int tds_linkedlist_push_back(tds_linkedlist *list, void *data)
{
	tds_linkedlist_node *node = NULL;

	assert(NULL != list);
	assert(NULL != data);

	if (NULL == (node = __get_linkedlist_node_for_appending(list))) {
		printf("Error ... tds_linkedlist_push_back\n");
		return 0;  /* failure */
	}
	memcpy(node->__data, data, list->__elesize);  /* node->__data is a valid pointer to a free space */
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

int tds_linkedlist_insert(tds_linkedlist *list, void *data, size_t n)
{
	tds_linkedlist_node *node = NULL;
	tds_linkedlist_node *node_post = NULL;  /* currenty indexed by n */
	size_t idx = 0;

	assert(NULL != list);
	assert(NULL != data);
	assert(n <= list->__len);

	if (n == 0)
		return tds_linkedlist_push_front(list, data);
	if (n == list->__len)
		return tds_linkedlist_push_back(list, data);
	/* Now 1 <= n <= len - 1 */

	node_post = list->__head;

	while (idx < n) {
		node_post = node_post->__next;
		idx++;
	}
	if (NULL == (node = __get_linkedlist_node_for_appending(list))) {
		printf("Error ... tds_linkedlist_insert\n");
		return 0;  /* failure */
	}
	memcpy(node->__data, data, list->__elesize);  /* node->__data is a valid pointer to a free space */
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


void tds_linkedlist_pop_front(tds_linkedlist *list)
{
	tds_linkedlist_node *node = NULL;

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
	__store_node_to_linkedlist_buffer(node, list);
}

void tds_linkedlist_pop_back(tds_linkedlist *list)
{
	tds_linkedlist_node *node = NULL;

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
	__store_node_to_linkedlist_buffer(node, list);
}

void tds_linkedlist_delete(tds_linkedlist *list, size_t n)
{
	tds_linkedlist_node *node = NULL;
	size_t idx = 0;

	assert(NULL != list);
	assert(list->__len > 0);
	assert(n < list->__len);

	node = list->__head;

	while (idx < n) {
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
	__store_node_to_linkedlist_buffer(node, list);
}
