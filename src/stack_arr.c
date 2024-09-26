/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/stack_arr.h>


tds_stack_arr *tds_stack_force_create(size_t elesize)
{
	return (tds_stack_arr *) tds_vector_force_create(elesize);
}

tds_stack_arr *tds_stack_create(size_t elesize)
{
	return (tds_stack_arr *) tds_vector_create(elesize);
}

tds_stack_arr *tds_stack_create_gen(size_t elesize, size_t capacity)
{
	return (tds_stack_arr *) tds_vector_create_gen(elesize, capacity);
}

void tds_stack_free(tds_stack_arr *stk)
{
	tds_vector_free(stk);
}

size_t tds_stack_capacity(const tds_stack_arr *stk)
{
	return tds_vector_capacity(stk);
}

size_t tds_stack_len(const tds_stack_arr *stk)
{
	return tds_vector_len(stk);
}

void *tds_stack_get(const tds_stack_arr *stk, size_t loc)
{
	return tds_vector_get(stk, tds_vector_len(stk) - 1 - loc);
}

void tds_stack_set(tds_stack_arr *stk, size_t loc, void *ele)
{
	tds_vector_set(stk, tds_vector_len(stk) - 1 - loc, ele);
}

void *tds_stack_getfront(const tds_stack_arr *stk)
{
	return tds_vector_get(stk, tds_vector_len(stk) - 1);
}

void tds_stack_setfront(tds_stack_arr *stk, void *ele)
{
	tds_vector_set(stk, tds_vector_len(stk) - 1, ele);
}

void *tds_stack_getback(const tds_stack_arr *stk)
{
	return tds_vector_get(stk, 0);
}

void tds_stack_setback(tds_stack_arr *stk, void *ele)
{
	tds_vector_set(stk, 0, ele);
}

int tds_stack_pushfront(tds_stack_arr *stk, const void *ele)
{
	return tds_vector_pushback(stk, ele);
}

void *tds_stack_popfront(tds_stack_arr *stk)
{
	return tds_vector_popback(stk);
}

void tds_stack_clear(tds_stack_arr *stk)
{
	tds_vector_clear(stk);
}
