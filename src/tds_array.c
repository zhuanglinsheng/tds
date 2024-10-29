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
	size_t __capacity;
	size_t __elesize;
	/* `capacity * elesize` more spaces will be allocated for storage */
};

#define tds_array_basic_size  sizeof(tds_array)


tds_array *tds_array_create(size_t elesize, size_t capacity)
{
	tds_array *arr = NULL;
	size_t array_data_size = elesize * capacity;
	size_t array_total_size = tds_array_basic_size + array_data_size;

	if (NULL == (arr = (tds_array *) malloc(array_total_size))) {
		printf("Error ... tds_array_create\n");
		return NULL;
	}
	memset(tds_array_data(arr), 0, array_data_size);  /* initialization */
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
	free(arr);
}

void * tds_array_data(const tds_array *arr)
{
	return ((char *) arr) + tds_array_basic_size;
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

	p = (char *) tds_array_data(arr);
	return p + arr->__elesize * loc;
}

void tds_array_set(tds_array *arr, size_t loc, const void *ele)
{
	char *p = NULL;
	assert(NULL != arr);
	assert(NULL != ele);
	assert(loc < arr->__capacity);
	p = (char *) tds_array_data(arr);
	memcpy(p + arr->__elesize * loc, ele, arr->__elesize);
}

int tds_array_resize(tds_array **arr, size_t new_capacity)
{
	tds_array *new_arr = NULL;
	size_t new_arr_total_size = 0;
	size_t elesize = 0;
	assert(NULL != arr);
	assert(NULL != *arr);
	if (new_capacity < (*arr)->__capacity)
		return 1;  /* success, no need to realloc */
	elesize = (*arr)->__elesize;
	new_arr_total_size = tds_array_basic_size + elesize * new_capacity;
	if (NULL == (new_arr = realloc(*arr, new_arr_total_size))) {
		printf("Error ... tds_array_resize\n");
		return 0;  /* failure */
	}
	new_arr->__elesize = elesize;
	new_arr->__capacity = new_capacity;
	*arr = new_arr;
	return 1;
}

void tds_array_force_resize(tds_array **arr, size_t new_capacity)
{
	if (!tds_array_resize(arr, new_capacity)) {
		printf("Error ... tds_array_force_resize\n");
		exit(-1);
	}
}
