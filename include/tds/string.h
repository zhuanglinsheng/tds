/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_STRING_H__
#define __T_STRING_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tds_string  tds_string;

/* On failure, return a `NULL` pointer
 */

tds_string *tds_string_create(void);
tds_string *tds_string_create_from_cstr(const char *cstr, size_t n);
tds_string *tds_string_create_substr(const tds_string *tstr, size_t pos, size_t n);
tds_string *tds_string_create_substr2(const tds_string *tstr, size_t pos);
tds_string *tds_string_copy(const tds_string *tstr);

/* On failure, exit the program
 */

tds_string *tds_string_force_create(void);
tds_string *tds_string_force_create_from_cstr(const char *cstr, size_t n);
tds_string *tds_string_force_create_substr(const tds_string *tstr, size_t pos, size_t n);
tds_string *tds_string_force_create_substr2(const tds_string *tstr, size_t pos);
tds_string *tds_string_force_copy(const tds_string *tstr);

void tds_string_clear(tds_string *tstr);
void tds_string_free(tds_string *tstr);

size_t tds_string_len(const tds_string *tstr);
const char *tds_string_cstr(const tds_string *tstr);
char tds_string_getc(const tds_string *tstr, size_t loc);
char tds_string_getc_front(const tds_string *tstr);
char tds_string_getc_back(const tds_string *tstr);
void tds_string_setc(const tds_string *tstr, size_t loc, char c);
void tds_string_setc_front(const tds_string *tstr, char c);
void tds_string_setc_back(const tds_string *tstr, char c);

/* On failure, exit the program
 */

/* Append `cstr` to the back of `tstr`
 *
 * Note: `cstr` ends with `\0`.
 */
void tds_string_force_append_cstr(tds_string *tstr, const char *cstr, size_t len);

void tds_string_force_append(tds_string *tstr, const tds_string *tstr2);
void tds_string_force_append_since(tds_string *tstr, const tds_string *tstr2, size_t loc);
void tds_string_force_append_c(tds_string *tstr, char c);
void tds_string_force_append_int32(tds_string *tstr, const int num);
void tds_string_force_append_int64(tds_string *tstr, const long num);
void tds_string_force_append_float32(tds_string *tstr, const float num, int sci);
void tds_string_force_append_float64(tds_string *tstr, const double num, int sci);

int tds_string_2_float64(const tds_string *tstr, double *num);
int tds_string_2_int64(const tds_string *tstr, long *num);

void tds_string_popn(tds_string *tstr, size_t n);
void tds_string_popn_front(tds_string *tstr, size_t n);

size_t tds_string_find(const tds_string *tstr, const char *cstr, size_t n);
size_t tds_string_find_last_of(const tds_string *tstr, const char *cstr, size_t n);
size_t tds_string_rfind(const tds_string *tstr, const char *cstr, size_t n);
size_t tds_string_rfind_last_of(const tds_string *tstr, const char *cstr, size_t n);

void tds_string_trim_back(tds_string *tstr);
void tds_string_trim_front(tds_string *tstr);
void tds_string_trim(tds_string *tstr);

int tds_string_first_n_same(const tds_string *tstr, const char *cstr, size_t len);
int tds_string_same(const tds_string *tstr, const tds_string *tstr2);
int tds_string_same_cstr(const tds_string *tstr, const char *cstr, size_t len);

#ifdef __cplusplus
}
#endif

#endif
