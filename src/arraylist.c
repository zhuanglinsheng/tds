/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/arraylist.h>
#include <tds/array.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __tds_arraylist_init_len  8

struct tds_arraylist {
	tds_array *__data;  /* created by array construction function */
	size_t __len;
};


tds_arraylist *tds_arraylist_create_gen(size_t elesize, size_t capacity)
{
	tds_arraylist *list = NULL;

	assert(elesize > 0);
	assert(capacity > 0);

	if (NULL == (list = (tds_arraylist *)malloc(sizeof(tds_arraylist)))) {
		printf("Error ... tds_arraylist_create_gen\n");
		return NULL;
	}
	if (NULL == (list->__data = tds_array_create(elesize, capacity))) {
		free(list);
		printf("Error ... tds_arraylist_create_gen\n");
		return NULL;
	}
	list->__len = 0;
	return list;
}

tds_arraylist *tds_arraylist_create(size_t elesize)
{
	return tds_arraylist_create_gen(elesize, __tds_arraylist_init_len);
}

tds_arraylist *tds_arraylist_force_create(size_t elesize)
{
	tds_arraylist* re = tds_arraylist_create(elesize);

	if (NULL == re) {
		printf("Error .. tds_arraylist_force_create\n");
		exit(-1);
	}
	return re;
}

void tds_arraylist_free(tds_arraylist *list)
{
	assert(NULL != list);
	tds_array_free(list->__data);
	free(list);
}


size_t tds_arraylist_capacity(const tds_arraylist *list)
{
	assert(NULL != list);
	return tds_array_capacity(list->__data);
}

size_t tds_arraylist_len(const tds_arraylist *list)
{
	assert(NULL != list);
	return list->__len;
}

void *tds_arraylist_get(const tds_arraylist *list, size_t loc)
{
	return tds_array_get(list->__data, loc);
}

void tds_arraylist_set(tds_arraylist *list, size_t loc, const void *ele)
{
	tds_array_set(list->__data, loc, ele);
}


int tds_arraylist_pushback(tds_arraylist *list, const void *ele)
{
	assert(NULL != list);
	assert(NULL != ele);

	if (list->__len < tds_arraylist_capacity(list)) {
		tds_arraylist_set(list, list->__len, ele);
		list->__len += 1;
		return 1;  /* success */
	} else {
		if (!tds_array_resize(list->__data, 2 * tds_arraylist_capacity(list))) {
			printf("Error ... tds_arraylist_pushback\n");
			return 0;  /* failure */
		}
		return tds_arraylist_pushback(list, ele);
	}
}

void tds_arraylist_force_pushback(tds_arraylist *list,const void *ele)
{
	if (!tds_arraylist_pushback(list, ele)) {
		printf("Error ... tds_arraylist_force_pushback\n");
		exit(-1);
	}
}

void tds_arraylist_force_eat_list(tds_arraylist *list, tds_arraylist *v2)
{
	size_t idx = 0;

	assert(NULL != list);
	assert(NULL != v2);

	for (; idx < tds_arraylist_len(v2); idx++) {
		void *ele_i = tds_arraylist_get(v2, idx);
		tds_arraylist_force_pushback(list, ele_i);
	}
	tds_arraylist_clear(v2);
}

void *tds_arraylist_getback(const tds_arraylist *list)
{
	assert(NULL != list);
	return tds_arraylist_get(list, list->__len - 1);
}

void tds_arraylist_setback(tds_arraylist *list, const void *ele)
{
	assert(NULL != list);
	assert(NULL != ele);
	tds_arraylist_set(list, list->__len - 1, ele);
}

void *tds_arraylist_popback(tds_arraylist *list)
{
	void *lastele;

	assert(NULL != list);
	assert(list->__len > 0);

	lastele = tds_arraylist_getback(list);
	list->__len--;
	return lastele;
}

void *tds_arraylist_delete(tds_arraylist *list, size_t idx)
{
	void *ele = NULL;

	assert(NULL != list);
	assert(idx < list->__len);

	ele = tds_arraylist_get(list, idx);

	for (; idx < list->__len - 1; idx++) {
		void *next_ele = tds_arraylist_get(list, idx + 1);
		tds_arraylist_set(list, idx, next_ele);
	}
	(list->__len)--;
	return ele;
}

void tds_arraylist_clear(tds_arraylist *list)
{
	assert (NULL != list);
	list->__len = 0;
}
