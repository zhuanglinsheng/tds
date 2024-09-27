/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#include <tds/bitarray.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tds_bitarray
{
	void *__data;  /* created by malloc */
	size_t __nbyte;
};

static void print_binary(unsigned char num)
{
	int i;

	for (i = 7; i >= 0; i--) {
		unsigned char bit = (num >> i) & 1;
		printf("%u", bit);
	}
	printf(" ");
}

void tds_bitarray_print(const tds_bitarray *arr)
{
	int i;

	for (i = 0; i < arr->__nbyte; i++) {
		uint8_t *p = arr->__data;
		print_binary(p[i]);
	}
	printf("\n");
}

tds_bitarray *tds_bitarray_create(size_t capacity)
{
	tds_bitarray *arr = NULL;
	size_t nbyte = 1;

	while (8 * nbyte < capacity)
		nbyte++;
	if (NULL == (arr = (tds_bitarray *) malloc(sizeof(tds_bitarray)))) {
		printf("Error ... tds_array_create\n");
		return NULL;
	}
	if (NULL == (arr->__data = malloc(nbyte))) {
		free(arr);
		printf("Error ... tds_array_create\n");
		return NULL;
	}
	arr->__nbyte = nbyte;
	memset(arr->__data, 0, nbyte);  /* initialize as 0 */
	return arr;
}

tds_bitarray *tds_bitarray_force_create(size_t capacity)
{
	tds_bitarray *arr = tds_bitarray_create(capacity);

	if (NULL == arr) {
		printf("Error ... tds_bitarray_force_create\n");
		exit(-1);
	}
	return arr;
}

void tds_bitarray_free(tds_bitarray *arr)
{
	assert(NULL != arr);

	free(arr->__data);
	free(arr);
}

int tds_bitarray_resize(tds_bitarray *arr, size_t new_capacity)
{
	void *new_data = NULL;
	size_t new_nbyte;

	assert(NULL != arr);
	new_nbyte = arr->__nbyte;

	if (new_capacity <= 8 * arr->__nbyte)  /* no need to resize */
		return 1;  /* success */
	while (new_capacity > 8 * new_nbyte)  /* calculate the new number of bytes */
		new_nbyte *= 2;
	if (NULL == (new_data = realloc(arr->__data, new_nbyte))) {
		printf("Error ... tds_bitarray_resize\n");
		return 0;  /* failure */
	}
	arr->__data = new_data;
	arr->__nbyte = new_nbyte;
	return 1;
}

void tds_bitarray_force_resize(tds_bitarray *arr, size_t new_capacity)
{
	if (!tds_bitarray_resize(arr, new_capacity)) {
		printf("Error ... tds_bitarray_force_resize\n");
		exit(-1);
	}
}

size_t tds_bitarray_capacity(const tds_bitarray *arr)
{
	assert(NULL != arr);
	return 8 * arr->__nbyte;
}

void tds_bitarray_init(tds_bitarray *arr, int b)
{
	size_t idx;

	assert(NULL != arr);
	assert(b == 0 || b == 1);

	for (idx = 0; idx < tds_bitarray_capacity(arr); idx++)
		tds_bitarray_set(arr, idx, b);
}

int tds_bitarray_get(const tds_bitarray *arr, size_t loc)
{
	uint8_t fake_byte;  /* the unsigned char contains the bit */
	uint8_t bit_loc;    /* the index of the bit */
	size_t byte_loc;    /* the index of fake_byte */
	uint8_t *p;         /* transform void* to unsigned char pointer */

	assert(NULL != arr);
	assert(loc < 8 * arr->__nbyte);

	byte_loc = loc / 8;
	bit_loc = loc % 8;
	p = (uint8_t *) arr->__data;
	fake_byte = p[byte_loc];

	fake_byte <<= bit_loc;
	fake_byte >>= 7;
	return fake_byte;
}

void tds_bitarray_set(tds_bitarray *arr, size_t loc, int b)
{
	uint8_t fake_byte;  /* the unsigned char containing the bit */
	uint8_t bit_loc;    /* the index of the bit */
	size_t byte_loc;    /* the index of fake_byte */
	uint8_t *p;         /* transform void* to unsigned char pointer */

	assert(NULL != arr);
	assert(loc < 8 * arr->__nbyte);
	assert(b == 0 || b == 1);

	byte_loc = loc / 8;
	bit_loc = loc % 8;
	p = (uint8_t *) arr->__data;
	fake_byte = p[byte_loc];
	fake_byte = (fake_byte & ~(1 << (7 - bit_loc))) | (b << (7 - bit_loc));
	/* Assign the (7-bit_loc) bit of `fake_byte` as b (either 0 or 1)
	 *
	 * Note:
	 * 	1. `x & ~(1 << n)` assign the n bit of x as 0
	 * 	2. `x | (1 << n)` assign the n bit of x as 1
	 */
	memset(p + byte_loc, fake_byte, 1);
}

tds_bitarray *tds_bitarray_and(const tds_bitarray *arr1, const tds_bitarray *arr2)
{
	tds_bitarray *new_arr = tds_bitarray_create(8 * arr1->__nbyte);
	size_t idx;

	assert(NULL != arr1);
	assert(NULL != arr2);
	assert(arr1->__nbyte == arr2->__nbyte);

	if (NULL == new_arr)  /* create failure */
		return NULL;
	for (idx = 0; idx < arr1->__nbyte; idx++) {
		uint8_t *p = (uint8_t *) (new_arr->__data);
		uint8_t c1 = ((unsigned char *) (arr1->__data)) [idx];
		uint8_t c2 = ((unsigned char *) (arr2->__data)) [idx];
		uint8_t c = c1 & c2;
		memset(p + idx, c, 1);
	}
	return new_arr;
}

tds_bitarray *tds_bitarray_or(const tds_bitarray *arr1, const tds_bitarray *arr2)
{
	tds_bitarray *new_arr = tds_bitarray_create(8 * arr1->__nbyte);
	size_t idx;

	assert(NULL != arr1);
	assert(NULL != arr2);
	assert(arr1->__nbyte == arr2->__nbyte);

	if (NULL == new_arr)  /* create failure */
		return NULL;
	for (idx = 0; idx < arr1->__nbyte; idx++) {
		uint8_t *p = (uint8_t *) (new_arr->__data);
		uint8_t c1 = ((unsigned char *) (arr1->__data)) [idx];
		uint8_t c2 = ((unsigned char *) (arr2->__data)) [idx];
		uint8_t c = c1 | c2;
		memset(p + idx, c, 1);
	}
	return new_arr;
}

tds_bitarray *tds_bitarray_xor(const tds_bitarray *arr1, const tds_bitarray *arr2)
{
	tds_bitarray *new_arr = tds_bitarray_create(8 * arr1->__nbyte);
	size_t idx;

	assert(NULL != arr1);
	assert(NULL != arr2);
	assert(arr1->__nbyte == arr2->__nbyte);

	if (NULL == new_arr)  /* create failure */
		return NULL;
	for (idx = 0; idx < arr1->__nbyte; idx++) {
		uint8_t *p = (uint8_t *) (new_arr->__data);
		uint8_t c1 = ((unsigned char *) (arr1->__data)) [idx];
		uint8_t c2 = ((unsigned char *) (arr2->__data)) [idx];
		uint8_t c = c1 ^ c2;
		memset(p + idx, c, 1);
	}
	return new_arr;
}

tds_bitarray *tds_bitarray_not(const tds_bitarray *arr)
{
	tds_bitarray *new_arr = tds_bitarray_create(8 * arr->__nbyte);
	size_t idx;

	assert(NULL != arr);

	if (NULL == new_arr)  /* create failure */
		return NULL;
	for (idx = 0; idx < arr->__nbyte; idx++) {
		uint8_t *p = (uint8_t *) (new_arr->__data);
		uint8_t c1 = ((unsigned char *) (arr->__data)) [idx];
		uint8_t c = ~c1;
		memset(p + idx, c, 1);
	}
	return new_arr;
}
