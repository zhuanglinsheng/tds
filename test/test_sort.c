#include <ta/sort.h>

#include <assert.h>
#include <stdio.h>
#include <time.h>

const int n = 1024;


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

void test_worst(ta_fsort_t _fsort, int *arr)
{
	int idx = 0;
	struct timespec start, end;
	double time_taken;

	for (idx = 0; idx < n; idx++) {
		arr[idx] = n - 1 - idx;
	}
	/* sort */
	clock_gettime(CLOCK_MONOTONIC, &start);
	_fsort(arr, sizeof(int), n, cmp_int, 1);
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("Time taken = %.9f\n", time_taken);

	for (idx = 0; idx < n; idx++) {
		/* printf("arr[%i] = %i\n", idx, arr[idx]); */
		assert(arr[idx] == idx);
	}
}

void test_best(ta_fsort_t _fsort, int *arr)
{
	int idx = 0;
	struct timespec start, end;
	double time_taken;

	for (idx = 0; idx < n; idx++) {
		arr[idx] = idx;
	}
	clock_gettime(CLOCK_MONOTONIC, &start);
	_fsort(arr, sizeof(int), n, cmp_int, 1);
	clock_gettime(CLOCK_MONOTONIC, &end);
	time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
	printf("Time taken = %.9f\n", time_taken);

	for (idx = 0; idx < n; idx++) {
		/* printf("arr[%i] = %i\n", idx, arr[idx]); */
		assert(arr[idx] == idx);
	}
}

int main(void)
{
	int arr_buffer[n];

	/* test insert sorting */
	test_worst(ta_sort_insert, arr_buffer);
	test_best(ta_sort_insert, arr_buffer);

	/* test bubble sorting */
	test_worst(ta_sort_bubble, arr_buffer);
	test_best(ta_sort_bubble, arr_buffer);

	return 0;
}
