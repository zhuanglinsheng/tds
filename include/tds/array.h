/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_ARRAY_H__
#define __T_ARRAY_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Array
 *
 *
 *****************************************************************************/

typedef struct tds_array  tds_array;

/* On failure, return NULL pointer
 * On success, the whole memorry space will be initialized as 0
 */
tds_array *tds_array_create(size_t elesize, size_t capacity);

/* On failure, exit the program
 * On success, the whole memorry space will be initialized as 0
 */
tds_array *tds_array_force_create(size_t elesize, size_t capacity);

int tds_array_resize(tds_array *arr, size_t new_capacity);
void tds_array_force_resize(tds_array *arr, size_t new_capacity);

void tds_array_free(tds_array *arr);

size_t tds_array_elesize(const tds_array *arr);
size_t tds_array_capacity(const tds_array *arr);

void *tds_array_get(const tds_array *arr, size_t loc);
void tds_array_set(tds_array *arr, size_t loc, const void *ele);

#ifdef __cplusplus
}
#endif

#endif
