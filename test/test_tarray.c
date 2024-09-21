#include <tds/tarray.h>
#include <tds/tds_vector.h>
#include <tds/tds_stack_arr.h>
#include <tds/thashtbl.h>
#include <tds/tds_string.h>

#include <stdio.h>

void test_array(void)
{
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

	char *p;

	tds_array *arr = tds_array_create(key_size, 10);
	tds_array_set(arr, 0, k0);
	tds_array_set(arr, 1, k1);
	tds_array_set(arr, 2, k2);
	tds_array_set(arr, 3, k3);
	tds_array_set(arr, 4, k4);
	tds_array_set(arr, 5, k5);
	tds_array_set(arr, 6, k6);
	tds_array_set(arr, 7, k7);
	tds_array_set(arr, 8, k8);
	tds_array_set(arr, 9, k9);

	p = tds_array_get(arr, 7);
	printf("%s\n", p);
	p = tds_array_get(arr, 8);
	printf("%s\n", p);
	p = tds_array_get(arr, 9);
	printf("%s\n", p);
}

void test_vector(void)
{
	struct tds_vector *arr = tds_vector_create(sizeof(tds_string*));  /* create array */
	int i, n = 100;

	for (i = 0; i < n; i++) {
		double ele = 1.0 / (i + 1);
		struct tds_string *tstr_i = tds_string_force_create();  /* create tds_string */
		tds_string_force_append_float64(tstr_i, ele, 0);
		tds_vector_force_pushback(arr, &tstr_i);  /* input is a pointer to pointer */
	}
	for (i = 0; i < tds_vector_len(arr); i++) {
		tds_string **tstr_i = (tds_string **)tds_vector_get(arr, i);  /* output is also a pointer to pointer */
		printf("[%i] %s\n", i, tds_string_cstr(*tstr_i));
		tds_string_free(*tstr_i);  /* release tds_string */
	}
	tds_vector_free(arr);  /* release array */
}

void test_hashtable(void)
{
	#define key_size 9
	tds_hashtbl *tbl = tds_hashtbl_create(key_size, sizeof(int));
	int state = 0;

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

	int v0 = 9;
	int v1 = 8;
	int v2 = 7;
	int v3 = 6;
	int v4 = 5;
	int v5 = 4;
	int v6 = 3;
	int v7 = 2;
	int v8 = 1;
	int v9 = 0;

	tds_hashtbl_set(tbl, k0, &v0);
	tds_hashtbl_set(tbl, k1, &v1);
	tds_hashtbl_set(tbl, k2, &v2);
	tds_hashtbl_set(tbl, k3, &v3);
	tds_hashtbl_set(tbl, k4, &v4);
	tds_hashtbl_set(tbl, k5, &v5);
	tds_hashtbl_set(tbl, k6, &v6);
	tds_hashtbl_set(tbl, k7, &v7);
	tds_hashtbl_set(tbl, k8, &v8);
	tds_hashtbl_set(tbl, k9, &v9);

	printf("capacity = %lu\n", tds_hashtbl_capacity(tbl));

	printf("k0 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k0, tds_hashtbl_capacity(tbl), &state));
	printf("k1 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k1, tds_hashtbl_capacity(tbl), &state));
	printf("k2 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k2, tds_hashtbl_capacity(tbl), &state));
	printf("k3 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k3, tds_hashtbl_capacity(tbl), &state));
	printf("k4 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k4, tds_hashtbl_capacity(tbl), &state));
	printf("k5 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k5, tds_hashtbl_capacity(tbl), &state));
	printf("k6 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k6, tds_hashtbl_capacity(tbl), &state));
	printf("k7 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k7, tds_hashtbl_capacity(tbl), &state));
	printf("k8 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k8, tds_hashtbl_capacity(tbl), &state));
	printf("k9 loc = %lu\n", __tds_hashtbl_get_loc(tbl, k9, tds_hashtbl_capacity(tbl), &state));

	printf("v0 = %i\n", *((int *)tds_hashtbl_get(tbl, k0)));
	printf("v1 = %i\n", *((int *)tds_hashtbl_get(tbl, k1)));
	printf("v2 = %i\n", *((int *)tds_hashtbl_get(tbl, k2)));
	printf("v3 = %i\n", *((int *)tds_hashtbl_get(tbl, k3)));
	printf("v4 = %i\n", *((int *)tds_hashtbl_get(tbl, k4)));
	printf("v5 = %i\n", *((int *)tds_hashtbl_get(tbl, k5)));
	printf("v6 = %i\n", *((int *)tds_hashtbl_get(tbl, k6)));
	printf("v7 = %i\n", *((int *)tds_hashtbl_get(tbl, k7)));
	printf("v8 = %i\n", *((int *)tds_hashtbl_get(tbl, k8)));
	printf("v9 = %i\n", *((int *)tds_hashtbl_get(tbl, k9)));

	printf("Usage = %lu\n", tds_hashtbl_usage(tbl));
	tds_hashtbl_rm(tbl, k0);
	printf("Usage = %lu\n", tds_hashtbl_usage(tbl));
}


int main(void)
{
	test_array();
	test_vector();
	test_hashtable();
	return 0;
}
