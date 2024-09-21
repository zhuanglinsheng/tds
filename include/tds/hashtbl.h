/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_HASHTBL__
#define __T_HASHTBL__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tds_hashtbl  tds_hashtbl;

tds_hashtbl *tds_hashtbl_create(size_t keysize, size_t valuesize);
tds_hashtbl *tds_hashtbl_force_create(size_t keysize, size_t valuesize);
void tds_hashtbl_free(tds_hashtbl *arr);

size_t tds_hashtbl_usage(const tds_hashtbl *arr);
size_t tds_hashtbl_capacity(const tds_hashtbl *arr);
double tds_hashtbl_load_factor(const tds_hashtbl *arr);

/* Core Hash algorithm
 */
uint64_t __tds_hashtbl_code_fn(const void *key, size_t keysize, uint64_t base);

/* Calculate the unique location for a given `key` in Hash-table `tbl`
 *
 * Parameter Notes:
 * 	1. _new_capacity == the capacity of `tbl` if you do not expand Hash-table
 * 	2. _new_capacity != the capacity of `tbl` if you expand Hash-table (used for rehash keys)
 * 	3. state is an OUTPUT parameter:
 * 		3.1. state = 0 if the location is free
 * 		3.2. state = 1 if the location is used
 */
size_t __tds_hashtbl_get_loc(const tds_hashtbl *tbl, const void *key, size_t _new_capacity, int *state);

/* Check the load factor and try to expand Hash-table
 */
int __tds_hashtbl_try_expand(tds_hashtbl *tbl);

/* Get the value of an existing pair
 * Return a NULL pointer if the key is not found
 */
void *tds_hashtbl_get(const tds_hashtbl *tbl, const void *key);

/* Return a bool indicating success
 */
int tds_hashtbl_set(tds_hashtbl *tbl, const void *key, const void *value);

void tds_hashtbl_force_set(tds_hashtbl *tbl, const void *key, const void *value);

/* Set the value of an existing pair
 * 	return a bool indicating the key found
 */
void tds_hashtbl_force_set_existing(tds_hashtbl *tbl, const void *key, const void *ele);

void tds_hashtbl_rm(tds_hashtbl *tbl, const void *key);

#ifdef __cplusplus
}
#endif

#endif
