#include <tds/array.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define key_size 9

char k0[key_size] = "abcdefg0";
char k1[key_size] = "abcdefg1";
char k2[key_size] = "abcdefg2";
char k3[key_size] = "abcdefg3";
char k4[key_size] = "abcdefg4";
char k5[key_size] = "abcdefg5";
char k6[key_size] = "abcdefg6";
char k7[key_size] = "abcdefg7";
char k8[key_size] = "abcdefg8";
char k9[key_size] = "abcdefg9";

/* testing
 * 	- tds_array_force_create
 * 	- tds_array_free
 * 	- tds_array_force_resize
 * 	- tds_array_set
 * 	- tds_array_get
 */
void test_array(void)
{
	tds_array *arr = tds_array_force_create(key_size, 5);

	tds_array_set(arr, 0, k0);
	tds_array_set(arr, 1, k1);
	tds_array_set(arr, 2, k2);
	tds_array_set(arr, 3, k3);
	tds_array_set(arr, 4, k4);

	tds_array_force_resize(&arr, 10);

	tds_array_set(arr, 5, k5);
	tds_array_set(arr, 6, k6);
	tds_array_set(arr, 7, k7);
	tds_array_set(arr, 8, k8);
	tds_array_set(arr, 9, k9);

	assert(0 == strncmp(k6, tds_array_get(arr, 6), key_size - 1));
	assert(0 == strncmp(k7, tds_array_get(arr, 7), key_size - 1));
	assert(0 == strncmp(k8, tds_array_get(arr, 8), key_size - 1));
	assert(0 == strncmp(k9, tds_array_get(arr, 9), key_size - 1));

	tds_array_free(arr);
}

int main(void)
{
	test_array();
	return 0;
}
