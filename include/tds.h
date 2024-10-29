/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef TDS_H
#define TDS_H

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

#define tds_ABS(a)  (((a) > 0) ? (a) : (-(a)))
#define tds_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define tds_MIN(a, b) (((a) < (b)) ? (a) : (b))

#ifdef __cplusplus
}
#endif

#endif
