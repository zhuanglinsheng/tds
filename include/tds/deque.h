/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_DEQUE_H__
#define __T_DEQUE_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tds_deque  tds_deque;

tds_deque *tds_deque_create_gen(size_t elesize, size_t blk_capacity);
tds_deque *tds_deque_create(size_t elesize);
tds_deque *tds_deque_force_create(size_t elesize);
tds_deque *tds_deque_free(tds_deque *q);

size_t tds_dequq_nblks(const tds_deque * q);

#ifdef __cplusplus
}
#endif

#endif
