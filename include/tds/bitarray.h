/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_BITARRAY_H__
#define __T_BITARRAY_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Bit Array
 *
 * Bit array, also known as a bit vector or bitset, is a data structure that
 * compactly stores bits (binary values of 0 and 1)
 *****************************************************************************/

typedef struct tds_bitarray  tds_bitarray;

/* On failure, return NULL pointer
 * On success, the whole array initialized as 0
 */
tds_bitarray *tds_bitarray_create(size_t capacity);

/* On failure, exit the program
 * On success, the whole array initialized as 0
 */
tds_bitarray *tds_bitarray_force_create(size_t capacity);

void tds_bitarray_free(tds_bitarray *arr);

void *tds_bitarray_data(const tds_bitarray *arr);
size_t tds_bitarray_capacity(const tds_bitarray *arr);
void tds_bitarray_print(const tds_bitarray *arr);

/* Input `b` is either 0 or 1
 */
void tds_bitarray_init(tds_bitarray *arr, int b);

/* Output is either 0 or 1
 */
int tds_bitarray_get(const tds_bitarray *arr, size_t loc);

/* Input `b` is either 0 or 1
 */
void tds_bitarray_set(tds_bitarray *arr, size_t loc, int b);

tds_bitarray *tds_bitarray_and(const tds_bitarray *arr1, const tds_bitarray *arr2);
tds_bitarray *tds_bitarray_or(const tds_bitarray *arr1, const tds_bitarray *arr2);
tds_bitarray *tds_bitarray_xor(const tds_bitarray *arr1, const tds_bitarray *arr2);
tds_bitarray *tds_bitarray_not(const tds_bitarray *arr);

int tds_bitarray_resize(tds_bitarray **arr, size_t new_capacity);
void tds_bitarray_force_resize(tds_bitarray **arr, size_t new_capacity);

#ifdef __cplusplus
}
#endif

#endif
