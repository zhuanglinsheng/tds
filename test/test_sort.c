#include <ta/sort.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define n  1000

int cmp_int(void *_a, void *_b)
{
	int *p1 = (int *)_a;
	int *p2 = (int *)_b;

	if (*p1 < *p2)
		return -1;
	else if (*p1 == *p2)
		return 0;
	else
		return 1;
}

void test_worst(ta_fsort_t _fsort, int *arr, int inc)
{
	int idx = 0;
	struct timespec start, end;
	double time_taken;

	for (idx = 0; idx < n; idx++) {
		arr[idx] = n - 1 - idx;
	}
	/* sort */
	clock_gettime(CLOCK_MONOTONIC, &start);
	_fsort(arr, sizeof(int), n, inc, cmp_int, 1);
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("Time taken = %.9f\n", time_taken);

	for (idx = 0; idx < n; idx += inc) {
		/* printf("arr[%i] = %i\n", idx, arr[idx]); */
		if (n % 2 == 0)
			assert(arr[idx] == idx + 1);
		else
			assert(arr[idx] == idx);
	}
}

void test_best(ta_fsort_t _fsort, int *arr, int inc)
{
	int idx = 0;
	struct timespec start, end;
	double time_taken;

	for (idx = 0; idx < n; idx++) {
		arr[idx] = idx;
	}
	clock_gettime(CLOCK_MONOTONIC, &start);
	_fsort(arr, sizeof(int), n, inc, cmp_int, 1);
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("Time taken = %.9f\n", time_taken);

	for (idx = 0; idx < n; idx += inc) {
		/* printf("arr[%i] = %i\n", idx, arr[idx]); */
		assert(arr[idx] == idx);
	}
}

void test_rand(ta_fsort_t _fsort, int *arr_rand, int inc, int ascend)
{
	struct timespec start, end;
	double time_taken;

	clock_gettime(CLOCK_MONOTONIC, &start);
	_fsort(arr_rand, sizeof(int), n, inc, cmp_int, ascend);
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("Time taken = %.9f\n", time_taken);
}

void test_heapify(int *arr, int inc)
{
	int idx = 0;

	for (idx = 0; idx < n; idx++) {
		arr[idx] = idx;
	}
	ta_heapify(arr, sizeof(int), n, inc, cmp_int, 1);
	for (idx = 0; idx < n; idx += inc) {
		printf("<test_heapify> arr[%i] = %i\n", idx + inc, arr[idx]);
		/* assert(arr[idx] == idx); */
	}
}

void assign_rand(int *arr_1, int *arr_2, int *arr_3, size_t len)
{
	int idx = 0;

	for (idx = 0; idx < n; idx++)
		arr_1[idx] = rand();
	for (idx = 0; idx < n; idx++)
		arr_2[idx] = rand();
	for (idx = 0; idx < n; idx++)
		arr_3[idx] = rand();
}

void print_arr(int *arr, size_t len, int inc)
{
	size_t idx = 0;

	for (idx = 0; idx < len; idx += inc)
		printf("arr[%lu] = %i\n", idx, arr[idx]);
}

int main(void)
{
	int arr_buffer[n];
	int arr_rand_1[n];
	int arr_rand_2[n];
	int arr_rand_3[n];

	printf("\nBubble sort:\n");
	test_worst(ta_sort_bubble, arr_buffer, 2);
	test_best(ta_sort_bubble, arr_buffer, 2);
	assign_rand(arr_rand_1, arr_rand_2, arr_rand_3, n);
	test_rand(ta_sort_bubble, arr_rand_1, 1, 0);
	test_rand(ta_sort_bubble, arr_rand_2, 1, 0);
	test_rand(ta_sort_bubble, arr_rand_3, 1, 0);

	printf("\nInsertion sort:\n");
	test_worst(ta_sort_insert, arr_buffer, 2);
	test_best(ta_sort_insert, arr_buffer, 2);
	assign_rand(arr_rand_1, arr_rand_2, arr_rand_3, n);
	test_rand(ta_sort_insert, arr_rand_1, 1, 0);
	test_rand(ta_sort_insert, arr_rand_2, 1, 0);
	test_rand(ta_sort_insert, arr_rand_3, 1, 0);

	printf("\nSelection sort:\n");
	test_worst(ta_sort_select, arr_buffer, 2);
	test_best(ta_sort_select, arr_buffer, 2);
	assign_rand(arr_rand_1, arr_rand_2, arr_rand_3, n);
	test_rand(ta_sort_select, arr_rand_1, 1, 0);
	test_rand(ta_sort_select, arr_rand_2, 1, 0);
	test_rand(ta_sort_select, arr_rand_3, 1, 0);

	printf("\nShell sort:\n");
	test_worst(ta_sort_shell, arr_buffer, 2);
	test_best(ta_sort_shell, arr_buffer, 2);
	assign_rand(arr_rand_1, arr_rand_2, arr_rand_3, n);
	test_rand(ta_sort_shell, arr_rand_1, 1, 0);
	test_rand(ta_sort_shell, arr_rand_2, 1, 0);
	test_rand(ta_sort_shell, arr_rand_3, 1, 0);

	printf("\nMerging sort:\n");
	test_worst(ta_sort_merge, arr_buffer, 2);
	test_best(ta_sort_merge, arr_buffer, 2);
	assign_rand(arr_rand_1, arr_rand_2, arr_rand_3, n);
	test_rand(ta_sort_merge, arr_rand_1, 1, 0);
	test_rand(ta_sort_merge, arr_rand_2, 1, 0);
	test_rand(ta_sort_merge, arr_rand_3, 1, 0);

	printf("\nQuick sort:\n");
	test_worst(ta_sort_quick, arr_buffer, 2);
	test_best(ta_sort_quick, arr_buffer, 2);
	assign_rand(arr_rand_1, arr_rand_2, arr_rand_3, n);
	test_rand(ta_sort_quick, arr_rand_1, 1, 0);
	test_rand(ta_sort_quick, arr_rand_2, 1, 0);
	test_rand(ta_sort_quick, arr_rand_3, 1, 0);

	printf("\nHeap sort:\n");
	test_worst(ta_sort_heap, arr_buffer, 2);
	test_best(ta_sort_heap, arr_buffer, 2);
	assign_rand(arr_rand_1, arr_rand_2, arr_rand_3, n);
	test_rand(ta_sort_heap, arr_rand_1, 1, 0);
	test_rand(ta_sort_heap, arr_rand_2, 1, 0);
	test_rand(ta_sort_heap, arr_rand_3, 1, 0);
	return 0;
}
