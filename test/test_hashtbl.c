#include <tds/hashtbl.h>
#include <tds/string.h>

#include <assert.h>
#include <stdio.h>


/* testing
 * 	- tds_hashtbl_force_create
 * 	- tds_hashtbl_free
 *
 * 	- tds_hashtbl_force_set
 * 	- tds_hashtbl_get
 * 	- __tds_hashtbl_get_loc
 *
 * 	- tds_hashtbl_rm
 */
void test_hashtable(void)
{
	size_t npairs = 10000;  /* number of pairs */
	size_t idx = 0;
	size_t buffersize = 16;

	tds_string *key_tstr = tds_string_create_gen(buffersize);
	tds_hashtbl *tbl = tds_hashtbl_force_create(buffersize, sizeof(size_t));

	for (idx = 0; idx < npairs; idx++) {
		const char *key_cstr;
		tds_string_force_append_cstr(key_tstr, "abcdefg", 7);
		tds_string_force_append_int64(key_tstr, idx);  /* idx = 0 to 9999 */
		key_cstr = tds_string_cstr(key_tstr);

		tds_hashtbl_force_set(tbl, key_cstr, &idx);
		tds_string_clear(key_tstr);
	}
#ifdef __tds_debug
	__tds_hashtbl_restore_n_conflicts(tbl);
#endif
	for (idx = 0; idx < npairs; idx++) {
		/* int state = 0; */
		const char *key_cstr;
		tds_string_force_append_cstr(key_tstr, "abcdefg", 7);
		tds_string_force_append_int64(key_tstr, idx);  /* idx = 0 to 9999 */
		key_cstr = tds_string_cstr(key_tstr);
		/* printf("loc of '%s' = %lu\n", key_cstr, \
			__tds_hashtbl_get_loc(tbl, key_cstr, tds_hashtbl_capacity(tbl), &state)); */
		assert(idx == *(size_t *)tds_hashtbl_get(tbl, key_cstr));
		tds_string_clear(key_tstr);
	}
#ifdef __tds_debug
	printf("Number of conflictions = %lu / %lu\n", __tds_hashtbl_get_n_conflicts(tbl), npairs);
	__tds_hashtbl_restore_n_conflicts(tbl);
#endif
	/*
	printf("\nCapacity = %lu\n", tds_hashtbl_capacity(tbl));
	printf("Load factor = %f\n", tds_hashtbl_load_factor(tbl));
	*/
	assert(tds_hashtbl_usage(tbl) == npairs);

	/* remove first one */
	tds_string_force_append_cstr(key_tstr, "abcdefg", 7);
	tds_string_force_append_int64(key_tstr, 0);
	tds_hashtbl_rm(tbl, tds_string_cstr(key_tstr));

	assert(tds_hashtbl_usage(tbl) == npairs - 1);
	tds_hashtbl_free(tbl);
}

int main(void)
{
	test_hashtable();
	return 0;
}
