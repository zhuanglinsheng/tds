#include <tds/hashtbl.h>
#include <tds/string.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define buffersize 16

const char *__base = "xxadrfh";

struct pair {
	char __key[buffersize];
	size_t __data;
};


/* testing
 * 	- tds_hashtbl_force_create_g
 * 	- tds_hashtbl_free
 *
 * 	- tds_hashtbl_force_set
 * 	- tds_hashtbl_get
 * 	- tds_hashtbl_getloc
 *
 * 	- tds_hashtbl_rm
 */
void test_hashtable(void)
{
	size_t npairs = 1000;  /* number of pairs */
	size_t idx = 0;

	tds_string *key_tstr = tds_string_create_g(buffersize);
	tds_hashtbl *tbl = tds_hashtbl_force_create_g(sizeof(struct pair), buffersize, npairs / 10);

#ifdef __tds_debug
	printf("Number of conflictions = %lu / %lu\n", __tds_hashtbl_get_n_conflicts(tbl), npairs);
	__tds_hashtbl_reset_n_conflicts(tbl);
#endif
	for (idx = 0; idx < npairs; idx++) {
		const char *key_cstr;
		struct pair p;
		tds_string_force_append_cstr(key_tstr, __base, 7);
		tds_string_force_append_int64(key_tstr, idx);  /* idx = 0 to npairs-1 */
		key_cstr = tds_string_cstr(key_tstr);

		memcpy(p.__key, key_cstr, buffersize);
		p.__data = idx;
		tds_hashtbl_force_set(tbl, &p);
		tds_string_clear(key_tstr);
	}
#ifdef __tds_debug
	printf("Number of conflictions = %lu / %lu\n", __tds_hashtbl_get_n_conflicts(tbl), npairs);
	__tds_hashtbl_reset_n_conflicts(tbl);
#endif
	printf("\nCapacity = %lu\n", tds_hashtbl_capacity(tbl));
	printf("Load factor = %f\n", tds_hashtbl_load_factor(tbl));
	assert(tds_hashtbl_usage(tbl) == npairs);

	/* test delete */
	tds_string_force_append_cstr(key_tstr, __base, 7);
	tds_string_force_append_int64(key_tstr, 0);
	assert(1 == tds_hashtbl_rm(tbl, tds_string_cstr(key_tstr)));
	assert(tds_hashtbl_usage(tbl) == npairs);
	tds_string_clear(key_tstr);

	/* test get */
	for (idx = 1; idx < npairs; idx++) {
		const char *key_cstr;
		struct pair *value_p;
		tds_string_force_append_cstr(key_tstr, __base, 7);
		tds_string_force_append_int64(key_tstr, idx);  /* idx = 1 to npairs-1 */
		key_cstr = tds_string_cstr(key_tstr);
		value_p = (struct pair *)tds_hashtbl_get(tbl, key_cstr);
		assert(NULL != value_p);
		assert(idx == value_p->__data);
		tds_string_clear(key_tstr);
	}
#ifdef __tds_debug
	printf("Number of conflictions = %lu / %lu\n", __tds_hashtbl_get_n_conflicts(tbl), npairs);
	__tds_hashtbl_reset_n_conflicts(tbl);
#endif
	tds_hashtbl_free(tbl);
}

int main(void)
{
	test_hashtable();
	return 0;
}
