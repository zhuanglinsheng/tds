#include <tds/arraylist.h>
#include <tds/string.h>
#include <stdio.h>

void test_arraylist(void)
{
	struct tds_arraylist *arr = tds_arraylist_create(sizeof(tds_string*));  /* create array */
	int i, n = 100;

	for (i = 0; i < n; i++) {
		double ele = 1.0 / (i + 1);
		struct tds_string *tstr_i = tds_string_force_create();  /* create tds_string */
		tds_string_force_append_float64(tstr_i, ele, 0);
		tds_arraylist_force_pushback(arr, &tstr_i);  /* input is a pointer to pointer */
	}
	for (i = 0; i < tds_arraylist_len(arr); i++) {
		tds_string **tstr_i = (tds_string **)tds_arraylist_get(arr, i);  /* output is also a pointer to pointer */
		printf("[%i] %s\n", i, tds_string_cstr(*tstr_i));
		tds_string_free(*tstr_i);  /* release tds_string */
	}
	tds_arraylist_free(arr);  /* release array */
}

int main(void)
{
	test_arraylist();
	return 0;
}
