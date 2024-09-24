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

#define __tds_debug

typedef struct tds_hashtbl  tds_hashtbl;

tds_hashtbl *tds_hashtbl_create_gen(size_t keysize, size_t valuesize, size_t init_capacity);
tds_hashtbl *tds_hashtbl_force_create_gen(size_t keysize, size_t valuesize, size_t init_capacity);
tds_hashtbl *tds_hashtbl_create(size_t keysize, size_t valuesize);
tds_hashtbl *tds_hashtbl_force_create(size_t keysize, size_t valuesize);

void tds_hashtbl_free(tds_hashtbl *arr);

size_t tds_hashtbl_usage(const tds_hashtbl *arr);
size_t tds_hashtbl_capacity(const tds_hashtbl *arr);
double tds_hashtbl_load_factor(const tds_hashtbl *arr);

/* Get the value of an existing pair
 * Return a NULL pointer if the key is not found
 */
void *tds_hashtbl_get(const tds_hashtbl *tbl, const void *key);

/* Return a bool indicating success
 *
 * Note
 * 	- the `key` can be existing or new
 */
int tds_hashtbl_set(tds_hashtbl *tbl, const void *key, const void *value);

/* On failure, exit the program
 */
void tds_hashtbl_force_set(tds_hashtbl *tbl, const void *key, const void *value);

/* Return a boolean
 */
int tds_hashtbl_contains(const tds_hashtbl *tbl, const void *key, size_t *loc);

/* Remove an element from the `tbl`
 * Return a boolean indicating success.
 */
int tds_hashtbl_rm(tds_hashtbl *tbl, const void *key);

/* Calculate the unique location for a given `key` in Hash-table `tbl`
 *
 * Note
 * 	1. `_new_capacity` can be different from the current capacity of `tbl`
 * 		- `_new_capacity` == the capacity of `tbl` if you do not expand Hash-table
 * 		- `_new_capacity` > the capacity of `tbl` if you expand Hash-table and rehash keys
 * 	2. `state` return 0 or 1, indeicating whether the location has elements
 * 		- If the location is free (return 0), the location is for newly inserting elements
 * 		- If the location has elements (return 1), the location is for changing existing elements
 */
size_t __tds_hashtbl_get_loc(const tds_hashtbl *tbl, const void *key, size_t _new_capacity, int *state);

#ifdef __tds_debug
size_t __tds_hashtbl_get_n_conflicts(const tds_hashtbl *tbl);
void __tds_hashtbl_reset_n_conflicts(tds_hashtbl *tbl);
#endif

#ifdef __cplusplus
}
#endif

#endif
