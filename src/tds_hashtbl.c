/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
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
#define __thashtbl_load_threshold  0.75
#define __thashtbl_default_base    367

struct tds_hashtbl {
	tds_bitarray *__mark_delete;  /* created by bitarray construction function */
	tds_array *__pairs;           /* created by array construction function */
	size_t __keysize;
	size_t __usage;
};


/******************************************************************************
 * Part 1. Hash related
 ******************************************************************************/

/* Core Hash algorithm
 */
uint64_t __tds_hashtbl_code_fn(const void *key, size_t keysize, uint64_t base)
{
	uint64_t code = 0;  /* hash code */
	size_t i;
	char *p = (char *) key;

	for (i = 0; i < keysize; i++) {
		code += (p[i] + 1);
		code *= base;
	}
	return code;
}

/* Test results of 100,000,000 key-value pairs, counting the total conflictions
 *
 * Note:
 * Load factor is around 20% - 40%. See `test/test_hashtbl.c`
 *
 * | prim |  1,000,000 |  10,000,000 |  100,000,000 |
 * | ---- | ---------- | ----------- | ------------ |
 * |  13  |    254,677 |   1,810,393 |              |
 * |  17  |    282,561 |   1,825,637 |              |
 * |  19  |    322,992 |   3,751,867 |              |
 * |  23  |    568,471 |     891,815 |              |
 * |  29  |    175,504 |   2,481,449 |              |
 * |  31  |    689,364 |   3,956,252 |              |
 * |  37  |    390,750 |   2,075,701 |              |
 * |  41  |    282,088 |     858,399 |              |
 * |  43  |    535,560 |   1,313,095 |              |
 * |  47  |    288,020 |   2,274,344 |              |
 * |  53  |    339,329 |   2,137,211 |              |
 * |  59  |    278,367 |     957,101 |              |
 * |  61  |    195,273 |   2,164,114 |              |
 * |  67  |    349,482 |   4,487,439 |              |
 * |  71  |    258,043 |   1,015,314 |              |
 * |  73  |    819,659 |   2,520,886 |              |
 * |  79  |    250,145 |     832,997 |              |
 * |  83  |    132,799 |   1,127,806 |              |
 * |  89  |    156,396 |   1,631,691 |              |
 * |  97  |    538,955 |   3,089,063 |              |
 * | 101  |    297,211 |     961,811 |              |
 * | 103  |    189,988 |     918,970 |              |
 * | 107  |    333,492 |   1,340,713 |              |
 * | 109  |    743,002 |   1,513,836 |              |
 * | 113  |    333,293 |   2,051,708 |              |
 * | 127  | 14,386,873 |  47,066,592 |              |
 * | 131  |    185,299 |   2,110,784 |              |
 * | 137  |    229,833 |   1,324,296 |              |
 * | 139  |    288,627 |   1,456,837 |              |
 * | 149  |    542,476 |   1,843,235 |              |
 * | 151  |    208,122 |     827,972 |              |
 * | 157  |    227,202 |   1,832,950 |              |
 * | 163  |    292,412 |   1,171,459 |              |
 * | 167  |    291,104 |   1,448,417 |              |
 * | 173  |    289,708 |   1,714,152 |              |
 * | 179  |    280,400 |   2,496,113 |              |
 * | 181  |    428,376 |   3,247,992 |              |
 * | 191  |  2,541,637 |  15,165,678 |              |
 * | 193  |  2,672,284 |  14,806,009 |              |
 * | 197  |    147,151 |     870,027 |              |
 * | 199  |    505,277 |   1,789,141 |              |
 * | 211  |    893,109 |   2,066,069 |              |
 * | 223  |    535,430 |   2,830,929 |              |
 * | 227  |    196,969 |   1,114,904 |              |
 * | 229  |    248,974 |   1,657,697 |              |
 * | 233  |    268,220 |   1,383,649 |              |
 * | 239  |    313,925 |   2,150,900 |              |
 * | 241  |    401,029 |   1,680,264 |              |
 * | 251  |    363,454 |   1,473,134 |              |
 * | 257  | 22,719,070 | 464,954,344 |              |
 * | 263  |    634,143 |   2,419,988 |              |
 * | 269  |    227,019 |   1,817,864 |              |
 * | 271  |    218,656 |   2,984,116 |              |
 * | 277  |    326,324 |   1,093,931 |              |
 * | 281  |    564,831 |   1,396,687 |              |
 * | 283  |  1,184,666 |   2,537,050 |              |
 * | 293  |    295,100 |   1,622,604 |              |
 * | 307  |    311,510 |   1,104,937 |              |
 * | 311  |    854,688 |   1,151,211 |              |
 * | 313  |    185,325 |   3,609,561 |              |
 * | 317  |    347,620 |     828,801 |              |
 * | 331  |    258,504 |   1,967,779 |              |
 * | 337  |    304,634 |   1,371,804 |              |
 * | 347  |    340,222 |   1,958,599 |              |
 * | 349  |    210,856 |     848,136 |              |
 * | 353  |    477,158 |   2,062,634 |              |
 * | 359  |    264,153 |   2,554,342 |              |
 * | 367  |    122,435 |     641,442 |   17,126,217 |
 * | 373  |    329,602 |   1,196,194 |              |
 * | 379  |    319,964 |   1,227,605 |              |
 * | 383  | 14,635,708 |  48,194,127 |              |
 * | 389  |    296,223 |   1,838,127 |              |
 * | 397  |    832,162 |   2,808,716 |              |
 * | 401  |    651,589 |   1,586,482 |              |
 * | 409  |    349,236 |   1,809,507 |              |
 * | 419  |    326,571 |   3,044,083 |              |
 * | 421  |    736,152 |   2,117,233 |              |
 * | 431  |    213,512 |   1,867,724 |              |
 * | 433  |    516,930 |   1,621,813 |              |
 * | 439  |    215,614 |   1,668,819 |              |
 * | 443  |    591,346 |   4,027,463 |              |
 * | 449  |  2,443,250 |  15,437,502 |              |
 * | 457  |    386,981 |   2,904,237 |              |
 * | 461  |    587,353 |   1,354,406 |              |
 * | 463  |    330,964 |   2,262,520 |              |
 * | 467  |    228,730 |     621,561 |   24,177,312 |
 * | 479  |    806,742 |   3,220,566 |              |
 * | 487  |    393,394 |   2,060,740 |              |
 * | 491  |    298,330 |     864,481 |              |
 * | 499  |    452,754 |   7,978,167 |              |
 * | 503  |    318,007 |   1,514,110 |              |
 * | 509  |    304,624 |   1,586,846 |              |
 */

#ifdef __tds_debug
size_t __n_conflicts = 0;

size_t __tds_hashtbl_get_n_conflicts(const tds_hashtbl *tbl)
{
	return __n_conflicts;
}

void __tds_hashtbl_reset_n_conflicts(tds_hashtbl *tbl)
{
	__n_conflicts = 0;
}
#endif


/******************************************************************************
 * Part 2. Creation, Resize & Free
 ******************************************************************************/

tds_hashtbl *tds_hashtbl_create_g(size_t pairsize, size_t keysize, size_t init_capacity)
{
	size_t capacity = __thashtbl_init_capacity;
	tds_hashtbl *tbl = NULL;
	assert(keysize <= pairsize);
	while (capacity < init_capacity)
		capacity *= 2;
	if (NULL == (tbl = (tds_hashtbl *) malloc(sizeof(tds_hashtbl)))) {
		printf("Error ... tds_hashtbl_create_g\n");
		return NULL;
	}
	if (NULL == (tbl->__pairs = tds_array_create(pairsize, capacity))) {
		free(tbl);
		printf("Error ... tds_hashtbl_create_g\n");
		return NULL;
	}
	if (NULL == (tbl->__mark_delete = tds_bitarray_create(capacity))) {
		tds_array_free(tbl->__pairs);
		free(tbl);
		printf("Error ... tds_hashtbl_create_g\n");
		return NULL;
	}
	tbl->__keysize = keysize;
	tbl->__usage = 0;
	return tbl;
}

tds_hashtbl *tds_hashtbl_create(size_t pairsize, size_t keysize)
{
	return tds_hashtbl_create_g(pairsize, keysize, __thashtbl_init_capacity);
}

tds_hashtbl *tds_hashtbl_force_create_g(size_t pairsize, size_t keysize, size_t init_capacity)
{
	tds_hashtbl *tbl = NULL;

	if (NULL == (tbl = tds_hashtbl_create_g(pairsize, keysize, init_capacity))) {
		printf("Error ... tds_hashtbl_force_create_g\n");
		exit(-1);
	}
	return tbl;
}

tds_hashtbl *tds_hashtbl_force_create(size_t pairsize, size_t keysize)
{
	return tds_hashtbl_force_create_g(pairsize, keysize, __thashtbl_init_capacity);
}

static int __is_empty(const tds_hashtbl *tbl, size_t loc)
{
	size_t idx = 0;
	size_t keysize = tds_array_elesize(tbl->__pairs);
	unsigned char *pair = (unsigned char *) tds_array_get(tbl->__pairs, loc);

	for (idx = 0; idx < keysize; idx++) {
		if (pair[idx] != 0)
			return 0;
	}
	return 1;
}

int __tds_hashtbl_try_expand(tds_hashtbl *tbl)
{
	size_t pairsize = 0;
	size_t new_capacity = 0;
	size_t idx = 0;
	tds_bitarray *new_delete_marker = NULL;
	tds_array *new_pairs = NULL;
	tds_hashtbl new_table;

	assert(NULL != tbl);

	if (tds_hashtbl_load_factor(tbl) < __thashtbl_load_threshold)
		return 1;  /* success, no need to resize */
	pairsize = tds_array_elesize(tbl->__pairs);
	new_capacity = 2 * tds_hashtbl_capacity(tbl);

	if (NULL == (new_delete_marker = tds_bitarray_create(new_capacity))) {
		printf("Error .. __tds_hashtbl_try_expand\n");
		return 0;
	}
	if (NULL == (new_pairs = tds_array_create(pairsize, new_capacity))) {
		printf("Error .. __tds_hashtbl_try_expand\n");
		return 0;
	}
	tds_bitarray_init(new_delete_marker, 0);
	new_table.__pairs = new_pairs;
	new_table.__mark_delete = new_delete_marker;
	new_table.__keysize = tbl->__keysize;
	new_table.__usage = 0;

	/* searching old hashtable, rehash keys */
	for (idx = 0; idx < tds_hashtbl_capacity(tbl); idx++) {
		if (__is_empty(tbl, idx))  /* not used */
			continue;
		if (tds_bitarray_get(tbl->__mark_delete, idx))  /* deleted */
			continue;
		tds_hashtbl_set(&new_table, tds_array_get(tbl->__pairs, idx));
	}
	tds_array_free(tbl->__pairs);
	tds_bitarray_free(tbl->__mark_delete);

	tbl->__pairs = new_pairs;
	tbl->__mark_delete = new_delete_marker;
	return 1;
}

void tds_hashtbl_free(tds_hashtbl *tbl)
{
	assert(NULL != tbl);
	tds_bitarray_free(tbl->__mark_delete);
	tds_array_free(tbl->__pairs);
	free(tbl);
}


/******************************************************************************
 * Part 3. Statistics
 ******************************************************************************/

size_t tds_hashtbl_usage(const tds_hashtbl *tbl)
{
	assert(NULL != tbl);
	return tbl->__usage;
}

size_t tds_hashtbl_capacity(const tds_hashtbl *tbl)
{
	assert(NULL != tbl);
	return tds_array_capacity(tbl->__pairs);
}

double tds_hashtbl_load_factor(const tds_hashtbl *tbl)
{
	assert(NULL != tbl);
	return ((double) tds_hashtbl_usage(tbl)) / ((double) tds_hashtbl_capacity(tbl));
}


/******************************************************************************
 * Part 4. Search, Get, Set and Delete
 ******************************************************************************/

/* Return
 * 	location `loc` that is either free or used
 * 	state = 0 if loc is free
 * 	state = 1 if loc is used
 */
size_t tds_hashtbl_getloc( \
	const tds_hashtbl *tbl, const void *key, size_t _new_capacity, int *state)
{
	long explorer_1 = 0;
	long explorer_2 = 1;
	size_t keysize = 0, code = 0, loc = 0;
	assert(NULL != tbl);
	assert(NULL != key);
	assert(NULL != state);
	assert(_new_capacity >= tds_array_capacity(tbl->__pairs));
	keysize = tbl->__keysize;
	code = __tds_hashtbl_code_fn(key, keysize, __thashtbl_default_base);
	loc = code % _new_capacity;
SEARCH:
	loc = (code + explorer_1 * explorer_1 * explorer_2) % _new_capacity;

	if (__is_empty(tbl, loc)) {
		*state = 0;  /* loc is free for storage */
		return loc;
	} else if (0 == memcmp(key, tds_array_get(tbl->__pairs, loc), keysize)) {
		*state = 1;  /* loc is used */
		return loc;
	} else {
		if (explorer_2 > 0)
			explorer_1++;
		explorer_2 *= -1;
#ifdef __tds_debug
		__n_conflicts++;
#endif
		goto SEARCH;
	}
}

int tds_hashtbl_contains(const tds_hashtbl *tbl, const void *key, size_t *loc)
{
	size_t old_capacity = tds_hashtbl_capacity(tbl);
	int occupied = 0;
	*loc = tds_hashtbl_getloc(tbl, key, old_capacity, &occupied);
	if (!occupied)
		return 0;
	if (tds_bitarray_get(tbl->__mark_delete, *loc))
		return 0;
	return 1;
}

void *tds_hashtbl_get(const tds_hashtbl *tbl, const void *key)
{
	size_t old_capacity = tds_hashtbl_capacity(tbl);
	int occupied = 0;
	size_t loc = tds_hashtbl_getloc(tbl, key, old_capacity, &occupied);
	if (!occupied)  /* the location is free */
		return NULL;
	if (tds_bitarray_get(tbl->__mark_delete, loc))  /* the location has been deleted */
		return NULL;
	return tds_array_get(tbl->__pairs, loc);
}

int tds_hashtbl_set(tds_hashtbl *tbl, const void *pair)
{
	size_t old_capacity = tds_hashtbl_capacity(tbl);
	int occupied = 0;
	size_t loc = tds_hashtbl_getloc(tbl, pair, old_capacity, &occupied);
	if (!occupied)  /* the location is originally avaliable */
		tbl->__usage++;
	tds_array_set(tbl->__pairs, loc, pair);

	if(!__tds_hashtbl_try_expand(tbl)) {
		printf("Error ... tds_hashtbl_set\n");
		return 0;  /* failure */
	}
	return 1;  /* success */
}

void tds_hashtbl_force_set(tds_hashtbl *tbl, const void *pair)
{
	if (!tds_hashtbl_set(tbl, pair)) {
		printf("Error ... tds_hashtbl_force_set\n");
		exit(-1);
	}
}

int tds_hashtbl_rm(tds_hashtbl *tbl, const void *key)
{
	size_t loc = 0;

	if (!tds_hashtbl_contains(tbl, key, &loc))
		return 0;
	tds_bitarray_set(tbl->__mark_delete, loc, 1);
	return 1;
}
