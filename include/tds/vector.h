/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_VECTOR_H__
#define __T_VECTOR_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tds_vector  tds_vector;

/* On failure, the program is stopped
 */
tds_vector *tds_vector_force_create(size_t elesize);

/* On failure, return `NULL`
 */
tds_vector *tds_vector_create(size_t elesize);

/* On failure, return `NULL`
 */
tds_vector *tds_vector_create_gen(size_t elesize, size_t capacity);

void tds_vector_free(tds_vector *vec);

size_t tds_vector_capacity(const tds_vector *vec);
size_t tds_vector_len(const tds_vector *vec);

void *tds_vector_get(const tds_vector *vec, size_t loc);
void tds_vector_set(tds_vector *vec, size_t loc, const void *ele);

void *tds_vector_getback(const tds_vector *vec);
void tds_vector_setback(tds_vector *vec, const void *ele);

/* Shadow copy contents of the pointer `ele` into `vec`
 * Return a bool indicating the success
 */
int tds_vector_pushback(tds_vector *vec, const void *ele);

/* Shadow copy contents of the pointer `ele` into `vec`
 * On failure, exit the program
 */
void tds_vector_force_pushback(tds_vector *vec, const void *ele);

/* Move all elements in `v2` into `vec`
 */
void tds_vector_force_eat_vec(tds_vector *vec, tds_vector *v2);

void *tds_vector_popback(tds_vector *vec);
void *tds_vector_delete(tds_vector *vec, size_t idx);
void tds_vector_clear(tds_vector *vec);

#ifdef __cplusplus
}
#endif

#endif
