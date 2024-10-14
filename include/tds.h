/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_TDS_H__
#define __T_TDS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Comparing function
 *
 * Assumption:
 * 	- return  1 if a > b
 * 	- return  0 if a = b
 * 	- return -1 if a < b
 */
typedef int tds_fcmp_t(void *_a, void *_b);


#ifdef __cplusplus
}
#endif

#endif
