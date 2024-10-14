/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_SORT_H__
#define __T_SORT_H__

#include <stddef.h>
#include <tds.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Sorting Algorithms
 *
 *
 *****************************************************************************/

#define __tds_elesize_limit  1024

/* A general type of sorting functions
 */
typedef int ta_fsort_t(void *, size_t, size_t, tds_fcmp_t, int);

int ta_sort_insert(void *arr, size_t elesize, size_t len, tds_fcmp_t f, int ascend);
int ta_sort_bubble(void *arr, size_t elesize, size_t len, tds_fcmp_t f, int ascend);


#ifdef __cplusplus
}
#endif

#endif
