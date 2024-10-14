#include <ta/sort.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>


void __swap(void *_a, void *_b, void * _tmp, size_t elesize)
{
	memcpy(_tmp, _a, elesize);
	memcpy(_a, _b, elesize);
	memcpy(_b, _tmp, elesize);
}

void ta_sort_insert(void *arr, size_t elesize, size_t len, tds_fcmp_t _f, int ascend)
{
	size_t idx_j = 1;
	size_t idx_i = 0;
	size_t idx_i_next = 1;
	char *p = (char *) arr;
	char _tmp[__tds_sort_elesize_limit];
	void *ele_i = NULL;
	void *ele_i_next = NULL;
	void *ele_j = NULL;

	assert(NULL != arr);
	assert(elesize < __tds_sort_elesize_limit);

	for (idx_j = 1; idx_j < len; idx_j++) {
		int _tmp_is_asged = 0;
		ele_j = p + idx_j * elesize;
		memcpy(_tmp, ele_j, elesize);

		for (idx_i_next = idx_j; idx_i_next > 0; idx_i_next--) {
			idx_i = idx_i_next - 1;
			ele_i = p + idx_i * elesize;
			ele_i_next = p + idx_i_next * elesize;

			if ((ascend && 1 == _f(ele_i, _tmp))
			 || (!ascend && 1 == _f(_tmp, ele_i)))
			/* if `_tmp < e_i` (ascend) or `_tmp > e_i` (descend)
			 * move `e_i` forwardly */
				memcpy(ele_i_next, ele_i, elesize);
			else {
			/* otherwise, insert `_tmp` into the next of `e_i`
			 * then, `e_j` is sorted. If `_tmp == ele_i_next`,
			 * the lib function `memcpy` would do nothing. */
				memcpy(ele_i_next, _tmp, elesize);
				_tmp_is_asged = 1;
				break;
			}
		}
		if (!_tmp_is_asged)
			memcpy(ele_i, _tmp, elesize);
	}
}

void ta_sort_bubble(void *arr, size_t elesize, size_t len, tds_fcmp_t _f, int ascend)
{
	size_t len_sorted = 0;
	size_t idx_i = 0;
	char *p = (char *) arr;
	char _tmp[__tds_sort_elesize_limit];
	void *ele_i = NULL;
	void *ele_i_next = NULL;

	assert(NULL != arr);
	assert(elesize < __tds_sort_elesize_limit);

	for (len_sorted = 0; len_sorted < len; len_sorted++) {
		int is_swapped = 0;
		for (idx_i = 0; idx_i < len - 1 - len_sorted; idx_i++) {
			ele_i = p + idx_i * elesize;
			ele_i_next = p + (idx_i + 1) * elesize;

			if ((ascend && 1 == _f(ele_i, ele_i_next))
			 || (!ascend && 1 == _f(ele_i_next, ele_i))) {
				__swap(ele_i, ele_i_next, _tmp, elesize);
				is_swapped = 1;
			}
		}
		if (0 == is_swapped) {
			break;
		}
	}
}
