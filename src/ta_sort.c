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

int ta_sort_insert(void *arr, size_t elesize, size_t len, tds_fcmp_t _f, int ascend)
{
	size_t idx_j = 1;
	size_t idx_i = 0;
	size_t idx_i_next = 1;
	char *p = (char *) arr;
	static char _tmp[__tds_elesize_limit];
	void *ele_i = NULL;
	void *ele_i_next = NULL;
	void *ele_j = NULL;

	assert(NULL != arr);
	assert(elesize < __tds_elesize_limit);

	if (len < 2)
		return 1;  /* success, no need to sort */
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
	return 0;
}

int ta_sort_bubble(void *arr, size_t elesize, size_t len, tds_fcmp_t f, int ascend)
{
	return 0;
}
