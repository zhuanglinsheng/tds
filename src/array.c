/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/array.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tds_array {
	void *__data;  /* created by `malloc` */
	size_t __capacity;
	size_t __elesize;
};

tds_array *tds_array_create(size_t elesize, size_t capacity)
{
	tds_array *arr = NULL;

	if (NULL == (arr = (tds_array *) malloc(sizeof(tds_array)))) {
		printf("Error ... tds_array_create\n");
		return NULL;
	}
	if (NULL == (arr->__data = malloc(elesize * capacity))) {
		free(arr);
		printf("Error ... tds_array_create\n");
		return NULL;
	}
	memset(arr->__data, 0, elesize * capacity);  /* initialization */
	arr->__capacity = capacity;
	arr->__elesize = elesize;
	return arr;
}

tds_array *tds_array_force_create(size_t elesize, size_t capacity)
{
	tds_array *arr = tds_array_create(elesize, capacity);

	if (NULL == arr) {
		printf("Error ... tds_array_force_create\n");
		exit(-1);
	}
	return arr;
}

void tds_array_free(tds_array *arr)
{
	assert(NULL != arr);

	free(arr->__data);
	free(arr);
}

size_t tds_array_elesize(const tds_array *arr)
{
	assert(NULL != arr);

	return arr->__elesize;
}

size_t tds_array_capacity(const tds_array *arr)
{
	assert(NULL != arr);

	return arr->__capacity;
}

void *tds_array_get(const tds_array *arr, size_t loc)
{
	char *p;

	assert(NULL != arr);
	assert(loc < arr->__capacity);

	p = (char *)(arr->__data);
	return p + arr->__elesize * loc;
}

void tds_array_set(tds_array *arr, size_t loc, const void *ele)
{
	char *p = (char *)(arr->__data);

	assert(NULL != arr);
	assert(NULL != ele);
	assert(loc < arr->__capacity);

	memcpy(p + arr->__elesize * loc, ele, arr->__elesize);
}

int tds_array_resize(tds_array *arr, size_t new_capacity)
{
	void *new_data = NULL;

	assert(NULL != arr);

	if (new_capacity < arr->__capacity)
		return 1;  /* success */
	if (NULL == (new_data = realloc(arr->__data, 2 * arr->__elesize * arr->__capacity))) {
		printf("Error ... tds_array_resize\n");
		return 0;  /* failure */
	}
	arr->__data = new_data;
	arr->__capacity *= 2;
	return 1;
}

void tds_array_force_resize(tds_array *arr, size_t new_capacity)
{
	if (!tds_array_resize(arr, new_capacity)) {
		printf("Error ... tds_array_force_resize\n");
		exit(-1);
	}
}
