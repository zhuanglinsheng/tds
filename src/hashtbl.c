/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/hashtbl.h>
#include <tds/array.h>
#include <tds/bitarray.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __thashtbl_init_capacity   16
#define __thashtbl_load_threshold  0.7
#define __thashtbl_default_base    431


/* Core Hash algorithm
 */
uint64_t __tds_hashtbl_code_fn(const void *key, size_t keysize, uint64_t base);

/* Check the load factor and try to expand Hash-table
 * Return a bool indicating the success
 */
int __tds_hashtbl_try_expand(tds_hashtbl *tbl);

struct tds_hashtbl
{
	tds_bitarray *__markers;
	tds_array *__keys;
	tds_array *__values;
	size_t __usage;
#ifdef __tds_debug
	size_t __n_conflicts;
#endif
};

tds_hashtbl *tds_hashtbl_create(size_t key_size, size_t value_size)
{
	tds_hashtbl *tbl = (tds_hashtbl *) malloc(sizeof(tds_hashtbl));

	if (NULL == tbl) {
		printf("Error ... tds_hashtbl_create\n");
		return NULL;
	}
	if (NULL == (tbl->__keys = tds_array_create(key_size, __thashtbl_init_capacity))) {
		free(tbl);
		printf("Error ... tds_hashtbl_create\n");
		return NULL;
	}
	if (NULL == (tbl->__values = tds_array_create(value_size, __thashtbl_init_capacity))) {
		free(tbl->__keys);
		free(tbl);
		printf("Error ... tds_hashtbl_create\n");
		return NULL;
	}
	if (NULL == (tbl->__markers = tds_bitarray_create(__thashtbl_init_capacity))) {
		free(tbl->__keys);
		free(tbl->__values);
		free(tbl);
		printf("Error ... tds_hashtbl_create\n");
		return NULL;
	}
	tds_bitarray_init(tbl->__markers, 0);
	tbl->__usage = 0;
#ifdef __tds_debug
	tbl->__n_conflicts = 0;
#endif
	return tbl;
}

tds_hashtbl *tds_hashtbl_force_create(size_t key_size, size_t value_size)
{
	tds_hashtbl *tbl = tds_hashtbl_create(key_size, value_size);

	if (NULL == tbl) {
		printf("Error ... tds_hashtbl_force_create\n");
		exit(-1);
	}
	return tbl;
}

void tds_hashtbl_free(tds_hashtbl *tbl)
{
	assert(NULL != tbl);

	tds_bitarray_free(tbl->__markers);
	tds_array_free(tbl->__keys);
	tds_array_free(tbl->__values);
	free(tbl);
}

size_t tds_hashtbl_usage(const tds_hashtbl *tbl)
{
	assert(NULL != tbl);
	return tbl->__usage;
}

size_t tds_hashtbl_capacity(const tds_hashtbl *tbl)
{
	assert(NULL != tbl);
	return tds_array_capacity(tbl->__keys);
}

double tds_hashtbl_load_factor(const tds_hashtbl *tbl)
{
	assert(NULL != tbl);
	return ((double) tds_hashtbl_usage(tbl)) / ((double) tds_hashtbl_capacity(tbl));
}

void *tds_hashtbl_get(const tds_hashtbl *tbl, const void *key)
{
	int state = 0;
	size_t loc = __tds_hashtbl_get_loc(tbl, key, tds_hashtbl_capacity(tbl), &state);
	if (state)  /* the location is used */
		return tds_array_get(tbl->__values, loc);
	else        /* the location is free */
		return NULL;
}

int tds_hashtbl_set(tds_hashtbl *tbl, const void *key, const void *value)
{
	int state = 0;
	size_t loc = __tds_hashtbl_get_loc(tbl, key, tds_hashtbl_capacity(tbl), &state);

	if (!state) {  /* the location is originally free */
		tds_bitarray_set(tbl->__markers, loc, 1);
		tbl->__usage++;
	}
	tds_array_set(tbl->__keys, loc, key);
	tds_array_set(tbl->__values, loc, value);

	if(!__tds_hashtbl_try_expand(tbl)) {
		printf("Error ... tds_hashtbl_set\n");
		return 0;  /* failure */
	}
	return 1;  /* success */
}

void tds_hashtbl_force_set(tds_hashtbl *tbl, const void *key, const void *value)
{
	if (!tds_hashtbl_set(tbl, key, value)) {
		printf("Error ... tds_hashtbl_force_set\n");
		exit(-1);
	}
}

void tds_hashtbl_force_set_existing(tds_hashtbl *tbl, const void *key, const void *ele)
{
	int state = 0;
	size_t loc = __tds_hashtbl_get_loc(tbl, key, tds_hashtbl_capacity(tbl), &state);

	if (state)  /* find the key */
		tds_array_set(tbl->__values, loc, ele);
	else {
		printf("Error .. tds_hashtbl_force_set_existing\n");
		exit(-1);
	}
}

void tds_hashtbl_rm(tds_hashtbl *tbl, const void *key)
{
	int state = 0;
	size_t loc = __tds_hashtbl_get_loc(tbl, key, tds_hashtbl_capacity(tbl), &state);

	if (state) {
		tds_bitarray_set(tbl->__markers, loc, 0);
		tbl->__usage--;
	}
}

/******************************************************************************
 * Hash related
 ******************************************************************************/

uint64_t __tds_hashtbl_code_fn(const void *key, size_t keysize, uint64_t base)
{
	uint64_t code = 0;  /* hash code */
	size_t i;
	char *p = (char *) key;

	for (i = 0; i<keysize; i++) {
		code += (p[i] + 1);
		code *= base;
	}
	return code;
}

size_t __tds_hashtbl_get_loc(
#ifndef __tds_debug
		const
#endif
		tds_hashtbl *tbl, const void *key, size_t _new_capacity, int *state)
{
	size_t key_size = tds_array_elesize(tbl->__keys);
	size_t code = __tds_hashtbl_code_fn(key, key_size, __thashtbl_default_base);
	long explorer_1 = 0;
	long explorer_2 = 1;
	size_t loc = code % _new_capacity;
SEARCH:
	loc = (code + explorer_1 * explorer_1 * explorer_2) % _new_capacity;

	if (!tds_bitarray_get(tbl->__markers, loc)) {
		*state = 0;  /* loc is free */
		return loc;
	} else if (0 == memcmp(key, tds_array_get(tbl->__keys, loc), key_size)) {
		*state = 1;  /* loc is key */
		return loc;
	} else {
		if (explorer_2 > 0)
			explorer_1++;
		explorer_2 *= -1;
#ifdef __tds_debug
		tbl->__n_conflicts++;
#endif
		goto SEARCH;
	}
}

int __tds_hashtbl_try_expand(tds_hashtbl *tbl)
{
	size_t key_size;
	size_t value_size;
	size_t new_capacity;
	size_t idx;
	tds_bitarray *new_markers = NULL;
	tds_array *new_keys = NULL;
	tds_array *new_values = NULL;
	tds_hashtbl new_table;

	assert(NULL != tbl);

	if (tds_hashtbl_load_factor(tbl) < __thashtbl_load_threshold)
		return 1;  /* success */
	key_size = tds_array_elesize(tbl->__keys);
	value_size = tds_array_elesize(tbl->__values);
	new_capacity = 2 * tds_hashtbl_capacity(tbl);

	if (NULL == (new_markers = tds_bitarray_create(new_capacity))) {
		printf("Error .. __tds_hashtbl_try_expand\n");
		return 0;
	}
	if (NULL == (new_keys = tds_array_create(key_size, new_capacity))) {
		printf("Error .. __tds_hashtbl_try_expand\n");
		return 0;
	}
	if (NULL == (new_values = tds_array_create(value_size, new_capacity))) {
		printf("Error .. __tds_hashtbl_try_expand\n");
		return 0;
	}
	new_table.__keys = new_keys;
	new_table.__values = new_values;
	tds_bitarray_init(new_markers, 0);
	new_table.__markers = new_markers;
	new_table.__usage = 0;

	/* searching old hashtable, rehash keys */
	for (idx = 0; idx < tds_hashtbl_capacity(tbl); idx++) {
		if (!tds_bitarray_get(tbl->__markers, idx))  /* not used */
			continue;
		tds_hashtbl_set(&new_table, \
			tds_array_get(tbl->__keys, idx), tds_array_get(tbl->__values, idx));
	}
	tds_array_free(tbl->__keys);
	tds_array_free(tbl->__values);
	tds_bitarray_free(tbl->__markers);

	tbl->__keys = new_keys;
	tbl->__values = new_values;
	tbl->__markers = new_markers;
	return 1;
}

#ifdef __tds_debug
size_t __tds_hashtbl_get_n_conflicts(const tds_hashtbl *tbl)
{
	return tbl->__n_conflicts;
}
void __tds_hashtbl_restore_n_conflicts(tds_hashtbl *tbl)
{
	tbl->__n_conflicts = 0;
}
#endif
