#include <tds/arraylist.h>
#include <tds/string.h>
#include <stdio.h>

void test(void)
{
	struct tds_arraylist *list = tds_arraylist_create(sizeof(tds_string*));
	int i, n = 100;

	for (i = 0; i < n; i++) {
		double ele = 1.0 / (i + 1);
		struct tds_string *tstr_i = tds_string_force_create();
		tds_string_force_append_float64(tstr_i, ele, 0);
		/* input is a pointer to pointer */
		tds_arraylist_force_pushback(list, &tstr_i);
	}
	for (i = 0; i < tds_arraylist_len(list); i++) {
		/* output is also a pointer to pointer */
		tds_string **tstr_i = (tds_string **)tds_arraylist_get(list, i);
		printf("[%i] %s\n", i, tds_string_cstr(*tstr_i));
		tds_string_free(*tstr_i);  /* release tds_string */
	}
	tds_arraylist_free(list);  /* release array */
}

int main(void)
{
	test();
	return 0;
}
