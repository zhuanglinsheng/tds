/*
 * Copyright (C) 2024 Zhuang Linsheng <zhuanglinsheng@outlook.com>
 * License: MIT <https://opensource.org/licenses/MIT>
 */
#ifndef __T_SORT_H__
#define __T_SORT_H__

#include <stddef.h>
#include <tds.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Sorting Algorithms
 *
 * Including bubble, quick, insert, shell, select, heap and merge
 *****************************************************************************/

#define __tds_sort_elesize_limit  1024

/* A general type of the sorting functions
 */
typedef void ta_fsort_t(void *, size_t, size_t, int, tds_fcmp_t, int);

/* Bubble Sort is the simplest sorting algorithm that works by repeatedly
 * swapping the adjacent elements if they are in the wrong order.
 *
 * Time: Best O(N), Average O(N^2), Worst O(N^2)
 * Space: O(1)
 */
void ta_sort_bubble(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

/* QuickSort is a sorting algorithm based on the Divide and Conquer that picks
 * an element as a pivot and partitions the given array around the picked
 * pivot by placing the pivot in its correct position in the sorted array.
 *
 * Time: Best O(N*log2(N)), Average O(N*log2(N)), Worst O(N^2)
 * Space: O(log2(N))
 *
 * Warning: on failure of memory allocation, exit the program!
 */
void ta_sort_quick(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

/* Insertion sort is a simple sorting algorithm that works by iteratively
 * inserting each element of an unsorted list into its correct position in a
 * sorted portion of the list.
 *
 * Time: Best O(N), Average O(N^2), Worst O(N^2)
 * Space: O(1)
 */
void ta_sort_insert(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

/* Shell sort is mainly a variation of Insertion Sort. In insertion sort, we
 * move elements only one position ahead. When an element has to be moved far
 * ahead, many movements are involved. The idea of ShellSort is to allow the
 * exchange of far items. In Shell sort, we make the array h-sorted for a
 * large value of h. We keep reducing the value of h until it becomes 1. An
 * array is said to be h-sorted if all sublists of every h’th element are sorted.
 *
 * Time: Best O(N*log2(N)), Average O(N^4/3), Worst O(N^3/2)
 * Space: O(1)
 */
void ta_sort_shell(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

/* Selection Sort is a comparison-based sorting algorithm. It sorts an array
 * by repeatedly selecting the smallest (or largest) element from the unsorted
 * portion and swapping it with the first unsorted element. This process
 * continues until the entire array is sorted.
 *
 * Time: Best O(N^2), Average O(N^2), Worst O(N^2)
 * Space: O(1)
 */
void ta_sort_select(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

/* Reorder array to make it a heap
 * Input:
 * 	- `ascend = 1` means root is largest
 * 	- `ascend = 0` means root is smallest
 * Return: the index of the last element in heap
 */
size_t ta_heapify(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

/* Heap sort is a comparison-based sorting technique based on Binary Heap Data
 * Structure. It can be seen as an optimization over selection sort where we
 * first find the max (or min) element and swap it with the last (or first).
 * We repeat the same process for the remaining elements. In Heap Sort, we use
 * Binary Heap so that we can quickly find and move the max element in O(Log n)
 * instead of O(n) and hence achieve the O(n Log n) time complexity.
 */
void ta_sort_heap(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

/* Merge sort is a sorting algorithm that follows the divide-and-conquer
 * approach. It works by recursively dividing the input array into smaller
 * subarrays and sorting those subarrays then merging them back together to
 * obtain the sorted array.
 *
 * Time: Best O(N*log2(N)), Average O(N*log2(N)), Worst O(N*log2(N))
 * Space: O(N)
 *
 * Warning: on failure of memory allocation, exit the program!
 */
void ta_sort_merge(void *arr, size_t elesize, size_t len, int inc, tds_fcmp_t _f, int ascend);

#ifdef __cplusplus
}
#endif

#endif
