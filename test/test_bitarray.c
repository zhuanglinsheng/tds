#include <tds/bitarray.h>

#include <assert.h>
#include <stdio.h>


/* testing
 * 	- tds_bitarray_force_create
 * 	- tds_bitarray_set
 * 	- tds_bitarray_get
 * 	- tds_bitarray_free
 */
void test_worst(void)
{
	tds_bitarray *arr = tds_bitarray_force_create(3);

	tds_bitarray_set(arr, 0, 1);
	assert(1 == tds_bitarray_get(arr, 0));

	tds_bitarray_set(arr, 0, 1);
	assert(1 == tds_bitarray_get(arr, 0));

	tds_bitarray_set(arr, 0, 0);
	assert(0 == tds_bitarray_get(arr, 0));

	tds_bitarray_set(arr, 0, 0);
	assert(0 == tds_bitarray_get(arr, 0));

	tds_bitarray_free(arr);
}

/* testing
 * 	- tds_bitarray_force_create
 * 	- tds_bitarray_force_resize
 * 	- tds_bitarray_set
 * 	- tds_bitarray_get
 * 	- tds_bitarray_free
 */
void test_best(void)
{
	tds_bitarray *arr = tds_bitarray_force_create(3);
	size_t idx = 0;

	tds_bitarray_force_resize(&arr, 100);

	for (idx = 0; idx < tds_bitarray_capacity(arr); idx++) {
		int b = idx % 2;
		tds_bitarray_set(arr, idx, b);
	}
	for (idx = 0; idx < tds_bitarray_capacity(arr); idx++) {
		int b = idx % 2;
		assert(b == tds_bitarray_get(arr, idx));
	}
	tds_bitarray_free(arr);
}

/* testing
 * 	- tds_bitarray_force_create
 * 	- tds_bitarray_and
 * 	- tds_bitarray_or
 * 	- tds_bitarray_xor
 * 	- tds_bitarray_not
 */
void test_3(void)
{
	size_t arr_size = 100;
	size_t idx = 0;

	tds_bitarray *arr_1 = tds_bitarray_force_create(arr_size);
	tds_bitarray *arr_2 = tds_bitarray_force_create(arr_size);
	tds_bitarray *arr = NULL;

	for (idx = 0; idx < 100; idx++) {
		int b = idx % 2;
		tds_bitarray_set(arr_1, idx, b);
		tds_bitarray_set(arr_2, idx, 1-b);
	}
	/*
	tds_bitarray_print(arr_1);
	printf("\n");
	tds_bitarray_print(arr_2);
	printf("\n");
	*/
	arr = tds_bitarray_and(arr_1, arr_2);

	for (idx = 0; idx < tds_bitarray_capacity(arr); idx++)
		assert(tds_bitarray_get(arr, idx) == (tds_bitarray_get(arr_1, idx) && tds_bitarray_get(arr_2, idx)));
	tds_bitarray_free(arr);

	arr = tds_bitarray_or(arr_1, arr_2);

	for (idx = 0; idx < tds_bitarray_capacity(arr); idx++)
		assert(tds_bitarray_get(arr, idx) == (tds_bitarray_get(arr_1, idx) || tds_bitarray_get(arr_2, idx)));
	tds_bitarray_free(arr);

	arr = tds_bitarray_xor(arr_1, arr_2);

	for (idx = 0; idx < tds_bitarray_capacity(arr); idx++)
		assert(tds_bitarray_get(arr, idx) == (tds_bitarray_get(arr_1, idx) != tds_bitarray_get(arr_2, idx)));
	tds_bitarray_free(arr);

	arr = tds_bitarray_not(arr_1);

	for (idx = 0; idx < tds_bitarray_capacity(arr); idx++)
		assert(tds_bitarray_get(arr, idx) == !tds_bitarray_get(arr_1, idx));
	tds_bitarray_free(arr);

	tds_bitarray_free(arr_1);
	tds_bitarray_free(arr_2);
}

int main(void)
{
	test_worst();
	test_best();
	test_3();
	return 0;
}
