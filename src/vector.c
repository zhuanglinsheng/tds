/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/vector.h>
#include <tds/array.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __tds_vector_init_len  8

struct tds_vector {
	tds_array *__data;
	size_t __len;
};


tds_vector *tds_vector_create_gen(size_t elesize, size_t capacity)
{
	tds_vector *vec = NULL;

	assert(elesize > 0);
	assert(capacity > 0);

	if (NULL == (vec = (tds_vector *)malloc(sizeof(tds_vector)))) {
		printf("Error ... tds_vector_create_gen\n");
		return NULL;
	}
	if (NULL == (vec->__data = tds_array_create(elesize, capacity))) {
		free(vec);
		printf("Error ... tds_vector_create_gen\n");
		return NULL;
	}
	vec->__len = 0;
	return vec;
}

tds_vector *tds_vector_create(size_t elesize)
{
	return tds_vector_create_gen(elesize, __tds_vector_init_len);
}

tds_vector *tds_vector_force_create(size_t elesize)
{
	tds_vector* re = tds_vector_create(elesize);

	if (NULL == re) {
		printf("Error .. tds_vector_force_create\n");
		exit(-1);
	}
	return re;
}

void tds_vector_free(tds_vector *vec)
{
	assert(NULL != vec);
	tds_array_free(vec->__data);
	free(vec);
}


size_t tds_vector_capacity(const tds_vector *vec)
{
	assert(NULL != vec);
	return tds_array_capacity(vec->__data);
}

size_t tds_vector_len(const tds_vector *vec)
{
	assert(NULL != vec);
	return vec->__len;
}

void *tds_vector_get(const tds_vector *vec, size_t loc)
{
	return tds_array_get(vec->__data, loc);
}

void tds_vector_set(tds_vector *vec, size_t loc, const void *ele)
{
	tds_array_set(vec->__data, loc, ele);
}


int tds_vector_pushback(tds_vector *vec, const void *ele)
{
	assert(NULL != vec);
	assert(NULL != ele);

	if (vec->__len < tds_vector_capacity(vec)) {
		tds_vector_set(vec, vec->__len, ele);
		vec->__len += 1;
		return 1;  /* success */
	} else {
		if (!tds_array_resize(vec->__data, 2 * tds_vector_capacity(vec))) {
			printf("Error ... tds_vector_pushback\n");
			return 0;  /* failure */
		}
		return tds_vector_pushback(vec, ele);
	}
}

void tds_vector_force_pushback(tds_vector *vec,const void *ele)
{
	if (!tds_vector_pushback(vec, ele)) {
		printf("Error ... tds_vector_force_pushback\n");
		exit(-1);
	}
}

void tds_vector_force_eat_vec(tds_vector *vec, tds_vector *v2)
{
	size_t idx = 0;

	assert(NULL != vec);
	assert(NULL != v2);

	for (; idx < tds_vector_len(v2); idx++) {
		void *ele_i = tds_vector_get(v2, idx);
		tds_vector_force_pushback(vec, ele_i);
	}
	tds_vector_clear(v2);
}

void *tds_vector_getback(const tds_vector *vec)
{
	assert(NULL != vec);
	return tds_vector_get(vec, vec->__len - 1);
}

void tds_vector_setback(tds_vector *vec, const void *ele)
{
	assert(NULL != vec);
	assert(NULL != ele);
	tds_vector_set(vec, vec->__len - 1, ele);
}

void *tds_vector_popback(tds_vector *vec)
{
	void *lastele;

	assert(NULL != vec);
	assert(vec->__len > 0);

	lastele = tds_vector_getback(vec);
	vec->__len--;
	return lastele;
}

void *tds_vector_delete(tds_vector *vec, size_t idx)
{
	void *ele = NULL;

	assert(NULL != vec);
	assert(idx < vec->__len);

	ele = tds_vector_get(vec, idx);

	for (; idx < vec->__len - 1; idx++) {
		void *next_ele = tds_vector_get(vec, idx + 1);
		tds_vector_set(vec, idx, next_ele);
	}
	(vec->__len)--;
	return ele;
}

void tds_vector_clear(tds_vector *vec)
{
	assert (NULL != vec);
	vec->__len = 0;
}
