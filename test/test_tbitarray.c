#include <tds/tbitarray.h>
#include <stdio.h>

void test_1(void)
{
	tds_bitarray *arr = tds_bitarray_force_create(3);

	printf("Test `tds_bitarray_get` and `tds_bitarray_set`:\n");

	tds_bitarray_set(arr, 0, 1);
	printf("set arr[0] = 1, get %i\n", tds_bitarray_get(arr, 0));

	tds_bitarray_set(arr, 0, 1);
	printf("set arr[0] = 1, get %i\n", tds_bitarray_get(arr, 0));

	tds_bitarray_set(arr, 0, 0);
	printf("set arr[0] = 0, get %i\n", tds_bitarray_get(arr, 0));

	tds_bitarray_set(arr, 0, 0);
	printf("set arr[0] = 0, get %i\n", tds_bitarray_get(arr, 0));

	printf("\n");
	tds_bitarray_free(arr);
}

void test_2(void)
{
	tds_bitarray *arr_1 = tds_bitarray_force_create(3);
	tds_bitarray *arr_2 = tds_bitarray_force_create(3);
	tds_bitarray *arr = NULL;

	tds_bitarray_set(arr_1, 0, 0);
	tds_bitarray_set(arr_1, 1, 1);
	tds_bitarray_set(arr_1, 2, 1);
	printf("arr_1[0] = %i\n", tds_bitarray_get(arr_1, 0));
	printf("arr_1[1] = %i\n", tds_bitarray_get(arr_1, 1));
	printf("arr_1[2] = %i\n", tds_bitarray_get(arr_1, 2));
	tds_bitarray_print(arr_1);
	printf("\n");

	tds_bitarray_set(arr_2, 0, 0);
	tds_bitarray_set(arr_2, 1, 0);
	tds_bitarray_set(arr_2, 2, 1);
	printf("arr_2[0] = %i\n", tds_bitarray_get(arr_2, 0));
	printf("arr_2[1] = %i\n", tds_bitarray_get(arr_2, 1));
	printf("arr_2[2] = %i\n", tds_bitarray_get(arr_2, 2));
	tds_bitarray_print(arr_2);
	printf("\n");

	arr = tds_bitarray_and(arr_1, arr_2);
	printf("Test `tds_bitarray_and`:\n");
	printf("arr[0] = %i\n", tds_bitarray_get(arr, 0));
	printf("arr[1] = %i\n", tds_bitarray_get(arr, 1));
	printf("arr[2] = %i\n", tds_bitarray_get(arr, 2));
	tds_bitarray_print(arr);
	printf("\n");
	tds_bitarray_free(arr);

	arr = tds_bitarray_or(arr_1, arr_2);
	printf("Test `tds_bitarray_or`:\n");
	printf("arr[0] = %i\n", tds_bitarray_get(arr, 0));
	printf("arr[1] = %i\n", tds_bitarray_get(arr, 1));
	printf("arr[2] = %i\n", tds_bitarray_get(arr, 2));
	tds_bitarray_print(arr);
	printf("\n");
	tds_bitarray_free(arr);

	arr = tds_bitarray_xor(arr_1, arr_2);
	printf("Test `tds_bitarray_xor`:\n");
	printf("arr[0] = %i\n", tds_bitarray_get(arr, 0));
	printf("arr[1] = %i\n", tds_bitarray_get(arr, 1));
	printf("arr[2] = %i\n", tds_bitarray_get(arr, 2));
	tds_bitarray_print(arr);
	printf("\n");
	tds_bitarray_free(arr);

	arr = tds_bitarray_not(arr_1);
	printf("Test `tds_bitarray_not`:\n");
	printf("arr[0] = %i\n", tds_bitarray_get(arr, 0));
	printf("arr[1] = %i\n", tds_bitarray_get(arr, 1));
	printf("arr[2] = %i\n", tds_bitarray_get(arr, 2));
	tds_bitarray_print(arr);
	printf("\n");
	tds_bitarray_free(arr);

	tds_bitarray_free(arr_1);
	tds_bitarray_free(arr_2);
}

int main(void)
{
	test_1();
	test_2();
	return 0;
}
