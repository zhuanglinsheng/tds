/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/string.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tds_string_initlen  8

struct tds_string
{
	char *__data;       /* created by malloc */
	size_t __capacity;
	size_t __len;
};


/*****************************************************************************
 * Create tds_string
 *****************************************************************************/

tds_string *tds_string_create_g(size_t buffersize)
{
	tds_string * str = NULL;
	char *data;
	size_t c_str_cap = tds_string_initlen;
	str = (tds_string *) malloc(sizeof(tds_string));

	if (NULL == str) {
		printf("Error ... tds_string_create\n");
		exit(-1);
	}
	while (c_str_cap < buffersize)  /* get `c_str_cap` that covers buffersize */
		c_str_cap *= 2;
	data = (char *) malloc(c_str_cap);
	memset(data, '\0', c_str_cap);

	if (NULL == data) {
		free(str);
		printf("Error ... tds_string_create\n");
		exit(-1);
	}
	str->__data = data;
	str->__capacity = c_str_cap;
	str->__len = 0;
	return str;
}

tds_string *tds_string_create(void)
{
	return tds_string_create_g(tds_string_initlen);
}

tds_string *tds_string_create_substr(const tds_string *tstr, size_t pos, size_t n)
{
	tds_string *substr;
	const char *cstr;

	assert(NULL != tstr);
	assert(pos <= tstr->__len);

	if (NULL == (substr = tds_string_create()))
		return NULL;
	cstr = tds_string_cstr(tstr);
	tds_string_force_append_cstr(substr, cstr + pos, n);
	return substr;
}

tds_string *tds_string_create_substr2(const tds_string *tstr, size_t pos)
{
	assert(NULL != tstr);
	assert(pos <= tstr->__len);

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

	free(tstr->__data);
	free(tstr);
}

void tds_string_clear(tds_string *tstr)
{
	assert(NULL != tstr);

	memset(tstr->__data, '\0', tstr->__capacity);
	tstr->__len = 0;
}


/*****************************************************************************
 * Get items from tds_string / Set
 *****************************************************************************/

size_t tds_string_len(const tds_string *tstr)
{
	assert(NULL != tstr);
	return tstr->__len;
}

const char *tds_string_cstr(const tds_string *tstr)
{
	assert(NULL != tstr);
	return tstr->__data;
}

char tds_string_getc(const tds_string *tstr, size_t loc)
{
	assert(NULL != tstr);
	assert(loc < tstr->__len);
	return tstr->__data[loc];
}

char tds_string_getc_front(const tds_string *tstr)
{
	assert(NULL != tstr);
	return tstr->__data[0];
}

char tds_string_getc_back(const tds_string *tstr)
{
	assert(NULL != tstr);
	return tstr->__data[tstr->__len - 1];
}

void tds_string_setc(const tds_string *tstr, size_t loc, char c)
{
	assert(NULL != tstr);
	assert(loc < tstr->__len);
	tstr->__data[loc] = c;
}

void tds_string_setc_front(const tds_string *tstr, char c)
{
	assert(NULL != tstr);
	tstr->__data[0] = c;
}

void tds_string_setc_back(const tds_string *tstr, char c)
{
	assert(NULL != tstr);
	tstr->__data[tstr->__len - 1] = c;
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
	newlen = tstr->__len + cstrlen;

	if (newlen < tstr->__capacity) {  /* length + 1 <= capacity */
		memcpy(tstr->__data + tstr->__len, cstr, cstrlen);
		tstr->__len += cstrlen;
		tstr->__data[tstr->__len] = '\0';
	} else {
		int n = 2;
		char *new_data = NULL;

		while (tstr->__capacity * n < newlen)
			n *= 2;
		new_data = (char *) realloc(tstr->__data, tstr->__capacity * n);

		if (NULL == new_data) {
			tds_string_free(tstr);
			printf("Error ... tds_string_append_str\n");
			exit(-1);
		}
		tstr->__capacity *= n;
		tstr->__data = new_data;
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

void tds_string_pop_back(tds_string *tstr, size_t n)
{
	assert(NULL != tstr);
	assert(n <= tstr->__len);

	tstr->__len -= n;
	memset(tstr->__data + tstr->__len, '\0', n);
}

void tds_string_pop_front(tds_string *tstr, size_t n)
{
	size_t idx = 0;

	assert(NULL != tstr);
	assert(n <= tstr->__len);

	while (idx < tstr->__len - n) {
		tstr->__data[idx] = tstr->__data[idx + n];
		idx++;
	}
	tstr->__len -= n;
	memset(tstr->__data + tstr->__len, '\0', n);
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
	tstrlen = tstr->__len;
	loc = 0;
	str = tstr->__data;

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
	tstrlen = tstr->__len;
	loc = 0;
	the_loc = tstrlen;
	str = tstr->__data;

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
	tstrlen = tstr->__len;
	loc = 1;
	str = tstr->__data;

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
	tstrlen = tstr->__len;
	loc = 1;
	the_loc = 0;
	str = tstr->__data;

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

	if (tstr->__len == 0)
		return;
TRIM_BACK:
	end = tds_string_getc_back(tstr);

	if (end == '\n' || end == '\r'
	 || end == '\t' || end == ' ' || end == EOF) {
		tds_string_pop_back(tstr, 1);
		goto TRIM_BACK;
	}
}

void tds_string_trim_front(tds_string *tstr)
{
	char bgn;

	assert(NULL != tstr);

	if (tstr->__len == 0)
		return;
TRIM_FRONT:
	bgn = tds_string_getc_front(tstr);

	if (bgn == '\n' || bgn == '\r'
	 || bgn == '\t' || bgn == ' ' || bgn == EOF) {
		tds_string_pop_front(tstr, 1);
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
		return 0 == strncmp(tstr->__data, cstr, n);
	}
}

int tds_string_same_cstr(const tds_string *tstr, const char *cstr, size_t len)
{
	size_t cstrlen = strnlen(cstr, len);

	if (tds_string_len(tstr) != cstrlen) {
		return 0;
	} else {
		return 0 == strncmp(tstr->__data, cstr, len);
	}
}

int tds_string_same(const tds_string *tstr, const tds_string *tstr2)
{
	return tds_string_same_cstr(tstr, tds_string_cstr(tstr2), tds_string_len(tstr2));
}
