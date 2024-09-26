/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_STACK_ARR_H__
#define __T_STACK_ARR_H__

#include <stddef.h>
#include <tds/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef tds_vector  tds_stack_arr;

/* On failure, exit the program
 */
tds_stack_arr *tds_stack_force_create(size_t elesize);

/* On failure, return `NULL`
 */
tds_stack_arr *tds_stack_create(size_t elesize);

/* On failure, return `NULL`
 */
tds_stack_arr *tds_stack_create_gen(size_t elesize, size_t capacity);

void tds_stack_free(tds_stack_arr *stk);

size_t tds_stack_capacity(const tds_stack_arr *stk);
size_t tds_stack_len(const tds_stack_arr *stk);

void *tds_stack_get(const tds_stack_arr *stk, size_t loc);
void tds_stack_set(tds_stack_arr *stk, size_t loc, void *ele);

void *tds_stack_getfront(const tds_stack_arr *stk);
void tds_stack_setfront(tds_stack_arr *stk, void *ele);

void *tds_stack_getback(const tds_stack_arr *stk);
void tds_stack_setback(tds_stack_arr *stk, void *ele);

int tds_stack_pushfront(tds_stack_arr *vec, const void *ele);
void *tds_stack_popfront(tds_stack_arr *vec);
void tds_stack_clear(tds_stack_arr *vec);

#ifdef __cplusplus
}
#endif

#endif
