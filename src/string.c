/*
 * Copyright (C) 2022 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/string.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __tds_string_initlen  8

struct tds_string
{
	size_t capacity;
	size_t len;
	char * data;
};


/*****************************************************************************
 * Create tds_string
 *****************************************************************************/

tds_string *tds_string_create(void)
{
	tds_string * str = NULL;
	char *data;
	str = (tds_string *) malloc(sizeof(tds_string));

	if (NULL == str) {
		printf("Error ... tds_string_create\n");
		exit(-1);
	}
	data = (char *) malloc(__tds_string_initlen);
	data[0] = '\0';

	if (NULL == data) {
		free(str);
		printf("Error ... tds_string_create\n");
		exit(-1);
	}
	str->data = data;
	str->capacity = __tds_string_initlen;
	str->len = 0;
	return str;
}

tds_string *tds_string_create_substr(const tds_string *tstr, size_t pos, size_t n)
{
	tds_string *substr;
	const char *cstr;

	assert(NULL != tstr);
	assert(pos <= tstr->len);

	if (NULL == (substr = tds_string_create()))
		return NULL;
	cstr = tds_string_cstr(tstr);
	tds_string_force_append_cstr(substr, cstr + pos, n);
	return substr;
}

tds_string *tds_string_create_substr2(const tds_string *tstr, size_t pos)
{
	assert(NULL != tstr);
	assert(pos <= tstr->len);

	return tds_string_create_substr(tstr, pos, tds_string_len(tstr));
}

tds_string *tds_string_create_from_cstr(const char *cstr, size_t n)
{
	tds_string *tstr;

	assert(NULL != cstr);

	if (NULL == (tstr = tds_string_create()))
		return NULL;
	tds_string_force_append_cstr(tstr, cstr, n);
	return tstr;
}

tds_string *tds_string_copy(const tds_string *tstr)
{
	return tds_string_create_substr2(tstr, 0);
}

tds_string *tds_string_force_create(void)
{
	tds_string *re = tds_string_create();

	if (NULL == re) {
		printf("Error ... tds_string_force_create\n");
		exit(-1);
	}
	return re;
}

tds_string *tds_string_force_create_from_cstr(const char *cstr, size_t n)
{
	tds_string *re = tds_string_create_from_cstr(cstr, n);

	if (NULL == re) {
		printf("Error ... tds_string_force_create_from_cstr\n");
		exit(-1);
	}
	return re;
}

tds_string *tds_string_force_create_substr(const tds_string *tstr, size_t pos, size_t n)
{
	tds_string *re = tds_string_create_substr(tstr, pos, n);

	if (NULL == re) {
		printf("Error ... tds_string_force_create_substr\n");
		exit(-1);
	}
	return re;
}

tds_string *tds_string_force_create_substr2(const tds_string *tstr, size_t pos)
{
	tds_string *re = tds_string_create_substr2(tstr, pos);

	if (NULL == re) {
		printf("Error ... tds_string_force_create_substr2\n");
		exit(-1);
	}
	return re;
}

tds_string *tds_string_force_copy(const tds_string *tstr)
{
	tds_string *re = tds_string_copy(tstr);

	if (NULL == re) {
		printf("Error ... tds_string_force_copy\n");
		exit(-1);
	}
	return re;
}

void tds_string_free(tds_string *tstr)
{
	assert(NULL != tstr);

	free(tstr->data);
	free(tstr);
}

void tds_string_clear(tds_string *tstr)
{
	assert(NULL != tstr);

	tstr->data[0] = '\0';
	tstr->len = 0;
}


/*****************************************************************************
 * Get items from tds_string / Set
 *****************************************************************************/

size_t tds_string_len(const tds_string *tstr)
{
	assert(NULL != tstr);

	return tstr->len;
}

const char *tds_string_cstr(const tds_string *tstr)
{
	assert(NULL != tstr);

	return tstr->data;
}

char tds_string_getc(const tds_string *tstr, size_t loc)
{
	assert(NULL != tstr);
	assert(loc < tstr->len);

	return tstr->data[loc];
}

char tds_string_getc_front(const tds_string *tstr)
{
	assert(NULL != tstr);

	return tstr->data[0];
}

char tds_string_getc_back(const tds_string *tstr)
{
	assert(NULL != tstr);

	return tstr->data[tstr->len - 1];
}

void tds_string_setc(const tds_string *tstr, size_t loc, char c)
{
	assert(NULL != tstr);
	assert(loc < tstr->len);

	tstr->data[loc] = c;
}

void tds_string_setc_front(const tds_string *tstr, char c)
{
	assert(NULL != tstr);

	tstr->data[0] = c;
}

void tds_string_setc_back(const tds_string *tstr, char c)
{
	assert(NULL != tstr);

	tstr->data[tstr->len - 1] = c;
}


/*****************************************************************************
 * Append
 *****************************************************************************/

void tds_string_force_append_cstr(tds_string *tstr, const char *cstr, size_t len)
{
	size_t cstrlen;
	size_t newlen;

	assert(NULL != tstr);
	assert(NULL != cstr);
	assert(len >= 0);

	if (0 == len)
		return;
	cstrlen = strnlen(cstr, len);
	newlen = tstr->len + cstrlen;

	if (newlen < tstr->capacity) {  /* length + 1 <= capacity */
		memcpy(tstr->data + tstr->len, cstr, cstrlen);
		tstr->len += cstrlen;
		tstr->data[tstr->len] = '\0';
	} else {
		int n = 2;
		char *new_data = NULL;

		while (tstr->capacity * n < newlen)
			n *= 2;
		new_data = (char *) realloc(tstr->data, tstr->capacity * n);

		if (NULL == new_data) {
			tds_string_free(tstr);
			printf("Error ... tds_string_append_str\n");
			exit(-1);
		}
		tstr->capacity *= n;
		tstr->data = new_data;
		tds_string_force_append_cstr(tstr, cstr, len);
	}
}

void tds_string_force_append(tds_string *tstr, const tds_string *tstr2)
{
	assert(NULL != tstr);
	assert(NULL != tstr2);

	tds_string_force_append_cstr(tstr, tds_string_cstr(tstr2), tds_string_len(tstr2));
}

void tds_string_force_append_since(tds_string *tstr, const tds_string *tstr2, size_t loc)
{
	const char *p = tds_string_cstr(tstr2);

	assert(loc <= tds_string_len(tstr2));

	tds_string_force_append_cstr(tstr, p + loc, tds_string_len(tstr2) - loc);
}

void tds_string_force_append_c(tds_string *tstr, char c)
{
	char cstr[2];

	assert(NULL != tstr);

	cstr[0] = c;
	cstr[1] = '\0';
	tds_string_force_append_cstr(tstr, cstr, 1);
}

void tds_string_force_append_int32(tds_string *tstr, const int num)
{
#ifdef bufsize
#undef bufsize
#endif
#define bufsize 16
	char buffer[bufsize];

	assert(NULL != tstr);

	memset(buffer, '\0', bufsize);
	snprintf(buffer, bufsize, "%d", num);
	tds_string_force_append_cstr(tstr, buffer, bufsize);
}

void tds_string_force_append_int64(tds_string *tstr, const long num)
{
#ifdef bufsize
#undef bufsize
#endif
#define bufsize 32
	char buffer[bufsize];

	assert(NULL != tstr);

	memset(buffer, '\0', bufsize);
	snprintf(buffer, bufsize, "%ld", num);
	tds_string_force_append_cstr(tstr, buffer, bufsize);
#undef bufsize
}

void tds_string_force_append_float32(tds_string *tstr, const float num, int sci)
{
	assert(NULL != tstr);

	if (sci) {
#ifdef bufsize
#undef bufsize
#endif
#define bufsize 32
		char buffer[bufsize];
		memset(buffer, '\0', bufsize);
		snprintf(buffer, bufsize, "%.8e", num);
		tds_string_force_append_cstr(tstr, buffer, bufsize);
#undef bufsize
	} else {
#ifdef bufsize
#undef bufsize
#endif
#define bufsize 64
		char buffer[bufsize];
		memset(buffer, '\0', bufsize);
		snprintf(buffer, bufsize, "%.8f", num);
		tds_string_force_append_cstr(tstr, buffer, bufsize);
#undef bufsize
	}
}

void tds_string_force_append_float64(tds_string *tstr, const double num, int sci)
{
	assert(NULL != tstr);

	if (sci) {
#ifdef bufsize
#undef bufsize
#endif
#define bufsize 32
		char buffer[bufsize];
		memset(buffer, '\0', bufsize);
		snprintf(buffer, bufsize, "%.18e", num);
		tds_string_force_append_cstr(tstr, buffer, bufsize);
#undef bufsize
	} else {
#ifdef bufsize
#undef bufsize
#endif
#define bufsize 512
		char buffer[bufsize];
		memset(buffer, '\0', bufsize);
		snprintf(buffer, bufsize, "%.18f", num);
		tds_string_force_append_cstr(tstr, buffer, bufsize);
#undef bufsize
	}
}


/*****************************************************************************
 * Pop
 *****************************************************************************/

void tds_string_popn(tds_string *tstr, size_t n)
{
	assert(NULL != tstr);
	assert(n <= tstr->len);

	tstr->len -= n;
	tstr->data[tstr->len] = '\0';
}

void tds_string_popn_front(tds_string *tstr, size_t n)
{
	size_t idx = 0;

	assert(NULL != tstr);
	assert(n <= tstr->len);

	while (idx < tstr->len - n) {
		tstr->data[idx] = tstr->data[idx + n];
		idx++;
	}
	tstr->data[idx] = '\0';
	tstr->len -= n;
}


/*****************************************************************************
 * Find
 *****************************************************************************/

size_t tds_string_find(const tds_string *tstr, const char *cstr, size_t n)
{
	size_t cstrlen;
	size_t tstrlen;
	size_t loc;
	const char *str;

	assert(NULL != tstr);
	assert(NULL != cstr);

	cstrlen = strnlen(cstr, n);
	tstrlen = tstr->len;
	loc = 0;
	str = tstr->data;

	if (0 == cstrlen) {
		return tstrlen;
	}
	for (; loc <= tstrlen - cstrlen; loc++) {
		if (0 == memcmp(str + loc, cstr, cstrlen)) {
			return loc;
		}
	}
	return tstrlen;
}

size_t tds_string_find_last_of(const tds_string *tstr, const char *cstr, size_t n)
{
	size_t cstrlen;
	size_t tstrlen;
	size_t loc;
	size_t the_loc;
	const char *str;

	assert(NULL != tstr);
	assert(NULL != cstr);

	cstrlen = strnlen(cstr, n);
	tstrlen = tstr->len;
	loc = 0;
	the_loc = tstrlen;
	str = tstr->data;

	if(0 == cstrlen) {
		return tstrlen;
	}
	for (; loc <= tstrlen - cstrlen; loc++) {
		if (0 == memcmp(str + loc, cstr, cstrlen)) {
			the_loc = loc;
		}
	}
	return the_loc;
}

size_t tds_string_rfind(const tds_string *tstr, const char *cstr, size_t n)
{
	size_t cstrlen;
	size_t tstrlen;
	size_t loc;
	const char *str;

	assert(NULL != tstr);
	assert(NULL != cstr);

	cstrlen = strnlen(cstr, n);
	tstrlen = tstr->len;
	loc = 1;
	str = tstr->data;

	if(0 == cstrlen) {
		return tstrlen;
	}
	for (; loc <= tstrlen - cstrlen + 1; loc++) {
		if (0 == memcmp(str + tstrlen - loc, cstr, cstrlen)) {
			return tstrlen - loc;
		}
	}
	return tstrlen;
}

size_t tds_string_rfind_last_of(const tds_string *tstr, const char *cstr, size_t n)
{
	size_t cstrlen;
	size_t tstrlen;
	size_t loc;
	size_t the_loc;
	const char *str;

	assert(NULL != tstr);
	assert(NULL != cstr);

	cstrlen = strnlen(cstr, n);
	tstrlen = tstr->len;
	loc = 1;
	the_loc = 0;
	str = tstr->data;

	if(0 == cstrlen) {
		return tstrlen;
	}
	for (; loc <= tstrlen - cstrlen + 1; loc++) {
		if (0 == memcmp(str + tstrlen - loc, cstr, cstrlen)) {
			the_loc = loc;
		}
	}
	return tstrlen - the_loc;
}


/*****************************************************************************
 * Trim
 *****************************************************************************/

void tds_string_trim_back(tds_string *tstr)
{
	char end;

	assert(NULL != tstr);

	if (tstr->len == 0)
		return;
TRIM_BACK:
	end = tds_string_getc_back(tstr);

	if (end == '\n' || end == '\r'
	 || end == '\t' || end == ' ' || end == EOF) {
		tds_string_popn(tstr, 1);
		goto TRIM_BACK;
	}
}

void tds_string_trim_front(tds_string *tstr)
{
	char bgn;

	assert(NULL != tstr);

	if (tstr->len == 0)
		return;
TRIM_FRONT:
	bgn = tds_string_getc_front(tstr);

	if (bgn == '\n' || bgn == '\r'
	 || bgn == '\t' || bgn == ' ' || bgn == EOF) {
		tds_string_popn_front(tstr, 1);
		goto TRIM_FRONT;
	}
}

void tds_string_trim(tds_string *tstr)
{
	assert(NULL != tstr);

	tds_string_trim_front(tstr);
	tds_string_trim_back(tstr);
}


/*****************************************************************************
 * Transform to number
 *****************************************************************************/

int tds_string_2_float64(const tds_string *tstr, double *num)
{
	char* end = NULL;
	const char *str = tds_string_cstr(tstr);
	*num = strtod(str, &end);
	return end != str && *end == '\0';
}

int tds_string_2_int64(const tds_string *tstr, long *num)
{
	char* end = NULL;
	const char *str = tds_string_cstr(tstr);
	*num = strtol(str, &end, 10);
	return end != str && *end == '\0';
}


/*****************************************************************************
 * Compafre
 *****************************************************************************/

int tds_string_first_n_same(const tds_string *tstr, const char *cstr, size_t n)
{
	size_t cstrlen = strnlen(cstr, n);

	if (tds_string_len(tstr) < cstrlen) {
		return 0;
	} else {
		return 0 == strncmp(tstr->data, cstr, n);
	}
}

int tds_string_same_cstr(const tds_string *tstr, const char *cstr, size_t len)
{
	size_t cstrlen = strnlen(cstr, len);

	if (tds_string_len(tstr) != cstrlen) {
		return 0;
	} else {
		return 0 == strncmp(tstr->data, cstr, len);
	}
}

int tds_string_same(const tds_string *tstr, const tds_string *tstr2)
{
	return tds_string_same_cstr(tstr, tds_string_cstr(tstr2), tds_string_len(tstr2));
}
