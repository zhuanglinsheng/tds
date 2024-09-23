#include <tds/string.h>

#include <stdio.h>
#include <limits.h>
#include <float.h>


void test_tds_string(void)
{
	tds_string *str = tds_string_force_create();
	tds_string *substr = NULL;
	long numl = 0;
	double numf = 0;

	/* minimum int */
	tds_string_force_append_int32(str, INT_MIN);
	printf(">>> len(INT_MIN) = %zu, str = %s\n", tds_string_len(str), tds_string_cstr(str));
	substr = tds_string_create_substr2(str, 0);
	printf("Copy of str = %s\n", tds_string_cstr(substr));

	tds_string_2_int64(str, &numl);
	printf(">>> back to int = %li\n", numl);

	tds_string_clear(substr);
	tds_string_clear(str);

	/* minimum long */
	tds_string_force_append_int64(str, LONG_MIN);
	printf(">>> len(LONG_MIN) = %zu, str = %s\n", tds_string_len(str), tds_string_cstr(str));
	tds_string_clear(str);

	/* (negative of) minimum positive float */
	tds_string_force_append_float32(str, -FLT_MIN, 1);
	printf(">>> len(-FLT_MIN) = %zu, str = %s\n", tds_string_len(str), tds_string_cstr(str));
	tds_string_clear(str);

	/* (negative of) maximum positive float */
	tds_string_force_append_float32(str, -FLT_MAX, 0);
	printf(">>> len(-FLT_MAX) = %zu, str = %s\n", tds_string_len(str), tds_string_cstr(str));
	tds_string_clear(str);

	/* (negative of) minimum positive double */
	tds_string_force_append_float64(str, -DBL_MIN, 1);
	printf(">>> len(-DBL_MIN) = %zu, str = %s\n", tds_string_len(str), tds_string_cstr(str));
	tds_string_clear(str);

	/* (negative of) maximum positive float */
	tds_string_force_append_float64(str, -DBL_MAX, 0);
	printf(">>> len(-DBL_MAX) = %zu, str = %s\n", tds_string_len(str), tds_string_cstr(str));

	tds_string_2_float64(str, &numf);
	printf(">>> back to double float = %f\n", numf);

	/* substring */
	substr = tds_string_create_substr2(str, 1);
	printf(">>> len(DBL_MAX) = %zu, str = %s\n", tds_string_len(substr), tds_string_cstr(substr));
	tds_string_free(substr);

	/* find */
	tds_string_clear(str);
	tds_string_force_append_cstr(str, "  a.bc  ", 100);
	tds_string_trim(str);
	printf("str = %s\n", tds_string_cstr(str));
	printf("loc of `c` = %zu\n", tds_string_find(str, "d", 7));
	printf("loc of `c` = %zu\n", tds_string_rfind(str, "d", 7));
	printf("loc of `c` = %zu\n", tds_string_find_last_of(str, "d", 7));
	printf("loc of `c` = %zu\n", tds_string_rfind_last_of(str, "d", 7));

	/* popn */
	tds_string_pop_back(str, 1);
	printf(">>> pop(1),       len = %zu, str = '%s'\n", tds_string_len(str), tds_string_cstr(str));
	tds_string_pop_front(str, 1);
	printf(">>> pop_front(1), len = %zu, str = '%s'\n", tds_string_len(str), tds_string_cstr(str));

	/* trim */
	tds_string_trim(str);
	tds_string_trim(str);
	tds_string_trim(str);
	printf("trimmed string,   len = %zu, str = '%s'\n", tds_string_len(str), tds_string_cstr(str));

	tds_string_free(str);
}


int main(void)
{
	test_tds_string();
	return 0;
}
