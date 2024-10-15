#include <ta/sort.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static void __swap(void *_a, void *_b, void * _tmp, size_t elesize)
{
	memcpy(_tmp, _b, elesize);
	memcpy(_b, _a, elesize);
	memcpy(_a, _tmp, elesize);
}

void ta_sort_bubble(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	size_t idx_sorted = 0;
	size_t idx_i = 0;
	char *p = (char *) arr;
	char _tmp[__tds_sort_elesize_limit];
	void *ele_i = NULL;
	void *ele_i_next = NULL;

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	for (idx_sorted = 0; idx_sorted < len; idx_sorted += inc) {
		int is_swapped = 0;
		for (idx_i = 0; idx_i + inc + idx_sorted < len; idx_i += inc) {
			ele_i = p + idx_i * elesize;
			ele_i_next = p + (idx_i + inc) * elesize;

			if ((ascend && 1 == _f(ele_i, ele_i_next))
			 || (!ascend && 1 == _f(ele_i_next, ele_i))) {
				__swap(ele_i, ele_i_next, _tmp, elesize);
				is_swapped = 1;
			}
		}
		if (0 == is_swapped)
			break;
	}
}

void ta_sort_select(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	size_t idx_sorted = 0;
	size_t idx_i = 0;
	size_t idx_select = 0;
	/* select the smallest if `ascend = 1` and the largest if `ascend = 0` */
	char *p = (char *) arr;
	char _tmp[__tds_sort_elesize_limit];
	void *ele_i = NULL;
	void *ele_select = NULL;

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	for (idx_sorted = 0; idx_sorted < len; idx_sorted += inc) {
		idx_select = idx_sorted;
		ele_select = p + idx_select * elesize;

		for (idx_i = idx_sorted + inc; idx_i < len; idx_i += inc) {
			ele_i = p + idx_i * elesize;

			if ((ascend && 1 == _f(ele_select, ele_i))
			 || (!ascend && 1 == _f(ele_i, ele_select))) {
				idx_select = idx_i;
				ele_select = p + idx_select * elesize;
			}
		}
		if (idx_sorted != idx_select)
			__swap(p + idx_sorted * elesize, ele_select, _tmp, elesize);
	}
}

void ta_sort_insert(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	size_t idx_j = inc;
	size_t idx_i = 0;
	size_t idx_i_next = inc;
	char *p = (char *) arr;
	char _tmp[__tds_sort_elesize_limit];
	void *ele_i = NULL;
	void *ele_i_next = NULL;
	void *ele_j = NULL;

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	for (idx_j = inc; idx_j < len; idx_j += inc) {
		int _tmp_is_asged = 0;
		ele_j = p + idx_j * elesize;
		memcpy(_tmp, ele_j, elesize);

		for (idx_i_next = idx_j; idx_i_next > 0; idx_i_next -= inc) {
			idx_i = idx_i_next - inc;
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

void ta_sort_shell(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	int n_skip = 2;
	int idx = 0;
	char *p = (char *) arr;

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	while (n_skip * 2 < INT_MAX && n_skip < len / 4)
		n_skip *= 2;
	while (n_skip > inc) {
		for (idx = 0; idx < n_skip; idx += inc) {
			void *arr_i = p + idx * elesize;
			size_t len_i = len - idx;
			ta_sort_insert(arr_i, elesize, len_i, n_skip, _f, ascend);
		}
		n_skip /= 2;
	}
	ta_sort_insert(arr, elesize, len, inc, _f, ascend);
}

void __sort_merge(char *arr, char *arr_1, char *arr_2, size_t len_1, size_t len_2, \
		int inc, size_t elesize, tds_fcmp_t _f, int ascend)
{
	size_t idx_merged = 0;
	size_t idx_1 = 0;
	size_t idx_2 = 0;
	void *ele_1 = NULL;
	void *ele_2 = NULL;

Sort_Merge_Begin:
	if (idx_1 >= len_1 && idx_2 >= len_2)
		goto Sort_Merge_End;
	if (idx_1 < len_1 && idx_2 < len_2) {
		ele_1 = arr_1 + idx_1 * elesize;
		ele_2 = arr_2 + idx_2 * elesize;
		if ((ascend && 1 == _f(ele_1, ele_2))
		 || (!ascend && 1 == _f(ele_2, ele_1))) {
			memcpy(arr + idx_merged * elesize, ele_2, elesize);
			idx_merged += inc;
			idx_2 += inc;
		} else {
			memcpy(arr + idx_merged * elesize, ele_1, elesize);
			idx_merged += inc;
			idx_1 += inc;
		}
	} else if (idx_1 < len_1 && idx_2 >= len_2) {
		ele_1 = arr_1 + idx_1 * elesize;
		memcpy(arr + idx_merged * elesize, ele_1, elesize);
		idx_merged += inc;
		idx_1 += inc;
	} else if (idx_1 >= len_1 && idx_2 < len_2) {
		ele_2 = arr_2 + idx_2 * elesize;
		memcpy(arr + idx_merged * elesize, ele_2, elesize);
		idx_merged += inc;
		idx_2 += inc;
	}
	goto Sort_Merge_Begin;
Sort_Merge_End:;
}

void ta_sort_merge(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	size_t idx = 0;
	size_t n_skip = 2 * inc;
	size_t total_size = elesize * len;
	size_t len_subarr_1 = 0;
	size_t len_subarr_2 = 0;
	char *arr_1 = NULL;
	char *arr_2 = NULL;
	char *arr_merged = NULL;
	char *buffer = NULL;  /* length of buffer is equal to `len` */
	char *p = (char *) arr;

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	if (NULL == (buffer = (char *) malloc(total_size))) {
		printf("Error .. ta_sort_merge\n");
		exit(-1);
	}
Merge_Sort_Loop:
	for (idx = 0; idx < len; idx += n_skip) {
		size_t remain = len - idx;
		len_subarr_1 = __tds_MIN(n_skip / 2, remain);
		len_subarr_2 = __tds_MIN(n_skip, remain) - len_subarr_1;
		arr_1 = p + idx * elesize;
		arr_2 = NULL;
		if (len_subarr_2 > 0)
			arr_2 = p + (idx + len_subarr_1) * elesize;
		arr_merged = buffer + idx * elesize;
		__sort_merge(arr_merged, arr_1, arr_2, len_subarr_1, \
			len_subarr_2, inc, elesize, _f, ascend);
	}
	memcpy(arr, buffer, total_size);
	n_skip *= 2;
	if (n_skip < 2 * len)
		goto Merge_Sort_Loop;
	free(buffer);
}

void ta_sort_quick(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	size_t idx_l = 0;
	size_t idx_r = 0;
	size_t idx_hole = 0;
	void *ele = NULL;
	void *hole_place = NULL;
	char *p = (char *) arr;
	static char pivot[__tds_sort_elesize_limit];

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	if (len <= inc)
		return;
	/* randomly select the pivot */
	idx_hole = rand() % len;
	idx_hole /= inc;
	idx_hole *= inc;

	if (0 != idx_hole) {
		void *ele_pivot = p + idx_hole * elesize;
		memcpy(pivot, ele_pivot, elesize);
		memcpy(ele_pivot, p, elesize);
		idx_hole = 0;
	} else
		memcpy(pivot, p, elesize);
	hole_place = p;
	idx_r = (len / inc + (len % inc > 0) - 1) * inc;
	ele = p + idx_r * elesize;

Quick_Sort_Partition:
	if (idx_l >= idx_r)
		goto Quick_Sort_End;
	if (idx_hole == idx_l) {
	/* ... | xxxx | ... | ele | ... */
		if ((ascend && 1 == _f(pivot, ele))
		 || (!ascend && 1 == _f(ele, pivot))) {
			memcpy(hole_place, ele, elesize);
			idx_hole = idx_r;
			hole_place = p + idx_hole * elesize;
			idx_l += inc;
			ele = p + idx_l * elesize;
		} else {
			idx_r -= inc;
			ele = p + idx_r * elesize;
		}
	} else {
	/* ... | ele | ... | xxxx | ... */
		if ((ascend && 1 == _f(ele, pivot))
		 || (!ascend && 1 == _f(pivot, ele))) {
			memcpy(hole_place, ele, elesize);
			idx_hole = idx_l;
			hole_place = p + idx_hole * elesize;
			idx_r -= inc;
			ele = p + idx_r * elesize;
		} else {
			idx_l += inc;
			ele = p + idx_l * elesize;
		}
	}
	goto Quick_Sort_Partition;

Quick_Sort_End:
	memcpy(hole_place, pivot, elesize);

	if (idx_l > inc)
		ta_sort_quick(arr, elesize, idx_l, inc, _f, ascend);
	if (len > inc + inc + idx_r) {
		void *arr_r = p + (idx_r + inc) * elesize;
		ta_sort_quick(arr_r, elesize, len - inc - idx_r, inc, _f, ascend);
	}
}

/* Heapify the node with at `idx_node`
 * Note:
 * 	Here, the input `idx_node` is the root node to be heapified.
 * 	Since heap is inc-indexed, so is the node.
 */
static void __heapify_node(char *arr, char *buffer, size_t elesize, size_t idx_last_leaf, \
		int inc, size_t idx_node, tds_fcmp_t _f, int ascend)
{
	size_t idx_child_l = 0;
	size_t idx_child_r = 0;
	size_t idx_picked_child = 0;
	int pick_right = 0;
	void *ele_child_l = NULL;
	void *ele_child_r = NULL;
	void *ele_picked_child = NULL;
	void *ele_tmp_root = NULL;

Heapify_Loop_Downwardly:
	ele_tmp_root = arr + (idx_node - inc) * elesize;  /* inc-indexed! */

	/* First step: pick the largest child node */
	idx_child_l = idx_node * 2;
	idx_child_r = idx_child_l + inc;
	ele_child_l = arr + (idx_child_l - inc) * elesize;  /* inc-indexed! */

	if (idx_child_r <= idx_last_leaf) {
	/* has left and right child */
		ele_child_r = arr + (idx_child_r - inc) * elesize;  /* inc-indexed! */
		pick_right = (ascend && 1 == _f(ele_child_r, ele_child_l))
			|| (!ascend && 1 == _f(ele_child_l, ele_child_r));
	} else if (idx_child_l <= idx_last_leaf)
	/* only has left child */
		pick_right = 0;
	else
	/* is leaf */
		return;
	idx_picked_child = idx_child_l + pick_right * inc;
	ele_picked_child = arr + (idx_picked_child - inc) * elesize;  /* inc-indexed! */

	/* Second step: compare & swap */
	if ((ascend && 1 == _f(ele_picked_child, ele_tmp_root))
	 || (!ascend && 1 == _f(ele_tmp_root, ele_picked_child))) {
		__swap(ele_picked_child, ele_tmp_root, buffer, elesize);
		idx_node = idx_picked_child;
		goto Heapify_Loop_Downwardly;
	}
}

size_t __heap_idx_last_leaf(size_t len, int inc)
{
	return ((len + inc - 1) / inc) * inc;
}

size_t __heap_idx_last_root(size_t idx_last_leaf, int inc)
{
	return (idx_last_leaf / (2 * inc)) * inc;
}

size_t ta_heapify(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	/* Note: heap is `inc` index, i.e., index of the first node is `inc`
	 * Only `idx = k * inc` is in heap.
	 */
	size_t idx_last_leaf = 0;
	size_t idx_last_root = 0;
	static char _tmp[__tds_sort_elesize_limit];

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	if (len < 1 + inc)  /* at least 2 nodes */
		return 0;
	idx_last_leaf = __heap_idx_last_leaf(len, inc);
	idx_last_root = __heap_idx_last_root(idx_last_leaf, inc);

	for (; idx_last_root > 0; idx_last_root -= inc)
		__heapify_node(arr, _tmp, elesize, idx_last_leaf, \
			inc, idx_last_root, _f, ascend);
	return idx_last_leaf - inc;  /* inc-indexed! */
}

void ta_sort_heap(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend)
{
	char *p = (char *) arr;
	char _tmp[__tds_sort_elesize_limit];
	size_t idx_last_leaf = len;

	assert(NULL != arr);
	assert(NULL != _f);
	assert(elesize < __tds_sort_elesize_limit);
	assert(inc > 0);
	assert(ascend == 0 || ascend == 1);

	if (len < 1 + inc)  /* at least 2 nodes */
		return;
	idx_last_leaf = ta_heapify(arr, elesize, len, inc, _f, ascend);

	while (len > inc) {
		void * p_last_leaf = p + idx_last_leaf * elesize;
		__swap(p, p_last_leaf, _tmp, elesize);
		len -= inc;
		idx_last_leaf = __heap_idx_last_leaf(len, inc);  /* inc-indexed */
		/* heapify the head: inc is the index of head */
		__heapify_node(arr, _tmp, elesize, idx_last_leaf, inc, inc, _f, ascend);
		idx_last_leaf -= inc;  /* 0-indexed */
	}
}
