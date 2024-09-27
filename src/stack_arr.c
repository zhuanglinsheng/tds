/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/stack_arr.h>


tds_stack_arr *tds_stack_arr_force_create(size_t elesize)
{
	return (tds_stack_arr *) tds_arraylist_force_create(elesize);
}

tds_stack_arr *tds_stack_arr_create(size_t elesize)
{
	return (tds_stack_arr *) tds_arraylist_create(elesize);
}

tds_stack_arr *tds_stack_arr_create_gen(size_t elesize, size_t capacity)
{
	return (tds_stack_arr *) tds_arraylist_create_gen(elesize, capacity);
}

void tds_stack_arr_free(tds_stack_arr *stk)
{
	tds_arraylist_free(stk);
}

size_t tds_stack_arr_capacity(const tds_stack_arr *stk)
{
	return tds_arraylist_capacity(stk);
}

size_t tds_stack_arr_len(const tds_stack_arr *stk)
{
	return tds_arraylist_len(stk);
}

void *tds_stack_arr_get(const tds_stack_arr *stk, size_t loc)
{
	return tds_arraylist_get(stk, tds_arraylist_len(stk) - 1 - loc);
}

void tds_stack_arr_set(tds_stack_arr *stk, size_t loc, void *ele)
{
	tds_arraylist_set(stk, tds_arraylist_len(stk) - 1 - loc, ele);
}

void *tds_stack_arr_getfront(const tds_stack_arr *stk)
{
	return tds_arraylist_get(stk, tds_arraylist_len(stk) - 1);
}

void tds_stack_arr_setfront(tds_stack_arr *stk, void *ele)
{
	tds_arraylist_set(stk, tds_arraylist_len(stk) - 1, ele);
}

void *tds_stack_arr_getback(const tds_stack_arr *stk)
{
	return tds_arraylist_get(stk, 0);
}

void tds_stack_arr_setback(tds_stack_arr *stk, void *ele)
{
	tds_arraylist_set(stk, 0, ele);
}

int tds_stack_arr_pushfront(tds_stack_arr *stk, const void *ele)
{
	return tds_arraylist_pushback(stk, ele);
}

void *tds_stack_arr_popfront(tds_stack_arr *stk)
{
	return tds_arraylist_popback(stk);
}

void tds_stack_arr_clear(tds_stack_arr *stk)
{
	tds_arraylist_clear(stk);
}
