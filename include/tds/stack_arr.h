/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef TDS_STACK_ARR_H
#define TDS_STACK_ARR_H

#include <stddef.h>
#include <tds/arraylist.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Array List based Stack
 *
 * Stack is a structure that follows the "Last In, First Out" (LIFO) principle
 *****************************************************************************/

typedef tds_arraylist  tds_stack_arr;

tds_stack_arr *tds_stack_arr_create(size_t elesize);
tds_stack_arr *tds_stack_arr_create_g(size_t elesize, size_t capacity);
tds_stack_arr *tds_stack_arr_force_create(size_t elesize);
tds_stack_arr *tds_stack_arr_force_create_g(size_t elesize, size_t capacity);

void tds_stack_arr_free(tds_stack_arr *stk);

size_t tds_stack_arr_capacity(const tds_stack_arr *stk);
size_t tds_stack_arr_len(const tds_stack_arr *stk);

void *tds_stack_arr_get(const tds_stack_arr *stk, size_t loc);
void tds_stack_arr_set(tds_stack_arr *stk, size_t loc, void *ele);

void *tds_stack_arr_getfront(const tds_stack_arr *stk);
void tds_stack_arr_setfront(tds_stack_arr *stk, void *ele);

void *tds_stack_arr_getback(const tds_stack_arr *stk);
void tds_stack_arr_setback(tds_stack_arr *stk, void *ele);

int tds_stack_arr_pushfront(tds_stack_arr *stk, const void *ele);
void *tds_stack_arr_popfront(tds_stack_arr *stk);
void tds_stack_arr_clear(tds_stack_arr *stk);

#ifdef __cplusplus
}
#endif

#endif
