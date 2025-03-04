/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef TDS_HASHTBL
#define TDS_HASHTBL

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Hash Table
 *
 * Hashtable (or hash table) is a data structure that implements an associative
 * array, allowing for efficient data retrieval based on a ele. It uses a
 * technique called hashing to convert ele into indice in an underlying array,
 * which allows for average-case constant time complexity O(1) for search,
 * insert, and delete operations.
 *****************************************************************************/

typedef struct tds_hashtbl  tds_hashtbl;

/* The first `keysize` bytes of the pair struct must be hash-able
 */

tds_hashtbl *tds_hashtbl_create_g(size_t pairsize, size_t keysize, size_t init_capacity);
tds_hashtbl *tds_hashtbl_force_create_g(size_t pairsize, size_t keysize, size_t init_capacity);
tds_hashtbl *tds_hashtbl_create(size_t pairsize, size_t keysize);
tds_hashtbl *tds_hashtbl_force_create(size_t pairsize, size_t keysize);

void tds_hashtbl_free(tds_hashtbl *arr);

size_t tds_hashtbl_usage(const tds_hashtbl *arr);
size_t tds_hashtbl_capacity(const tds_hashtbl *arr);
double tds_hashtbl_load_factor(const tds_hashtbl *arr);

/* Get the value of an existing pair
 * Return a NULL pointer if the ele is not found
 */
void *tds_hashtbl_get(const tds_hashtbl *tbl, const void *key);

/* Return a bool indicating success
 *
 * Note
 * 	- the `ele` can be existing or new
 */
int tds_hashtbl_set(tds_hashtbl *tbl, const void *ele);

/* On failure, exit the program
 */
void tds_hashtbl_force_set(tds_hashtbl *tbl, const void *ele);

/* Return a boolean
 */
int tds_hashtbl_contains(const tds_hashtbl *tbl, const void *ele, size_t *loc);

/* Remove an element from the `tbl`
 * Return a boolean indicating success.
 */
int tds_hashtbl_rm(tds_hashtbl *tbl, const void *pair);

/* Calculate the unique location for a given `ele` in Hash-table `tbl`
 *
 * Note
 * 	1. `_new_capacity` can be different from the current capacity of `tbl`
 * 		- `_new_capacity` == the capacity of `tbl` if you do not expand Hash-table
 * 		- `_new_capacity` > the capacity of `tbl` if you expand Hash-table and rehash keys
 * 	2. `state` return 0 or 1, indeicating whether the location has elements
 * 		- If the location is free (return 0), the location is for newly inserting elements
 * 		- If the location has elements (return 1), the location is for changing existing elements
 */
size_t tds_hashtbl_getloc(const tds_hashtbl *tbl, const void *ele, size_t _new_capacity, int *state);

#ifdef __cplusplus
}
#endif

#endif
