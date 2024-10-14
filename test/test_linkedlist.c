#include <tds/linkedlist.h>

#include <assert.h>
#include <stdio.h>

void test_0(void)
{
	tds_linkedlist *list = tds_linkedlist_create(sizeof(long));
	size_t num_tests = 1000;

	/* pre-allocate all the nodes */
	tds_linkedlist_prealloc(list, num_tests);

	assert(num_tests == tds_linkedlist_bufferlen(list));
	tds_linkedlist_free_buffer(list);
	assert(0 == tds_linkedlist_bufferlen(list));
	tds_linkedlist_free(list);
}

/* testing basic
 * 	- tds_linkedlist_create
 * 	- tds_linkedlist_prealloc
 * 	- tds_linkedlist_free
 * 	- tds_linkedlist_pushback
 *
 * testing iteration
 * 	- tds_linkedlistiter_head
 * 	- tds_linkedlistiter_tail
 * 	- tds_linkedlistiter_next
 * 	- tds_linkedlistiter_prev
 */
void test_worst(void)
{
	tds_linkedlist *list = tds_linkedlist_create(sizeof(long));
	size_t num_tests = 10;
	tds_linkedlistiter *iter = NULL;
	long idx = 0;

	for (idx = 0; idx < num_tests; idx++)
		tds_linkedlist_pushback(list, &idx);

	idx = 0;
	iter = tds_linkedlistiter_head(list);
	while (iter != NULL) {
		long *value = (long *)tds_linkedlistiter_data(iter);
		iter = tds_linkedlistiter_next(iter);
		assert(*value == idx);
		idx += 1;
	}

	iter = tds_linkedlistiter_tail(list);
	while (iter != NULL) {
		long *value = (long *)tds_linkedlistiter_data(iter);
		iter = tds_linkedlistiter_prev(iter);
		idx -= 1;
		assert(*value == idx);
	}
	tds_linkedlist_free(list);
}

/* testing
 * 	- tds_linkedlist_create
 * 	- tds_linkedlist_free
 *
 * 	- tds_linkedlist_len
 * 	- tds_linkedlist_bufferlen
 *
 * 	- tds_linkedlist_pushback
 * 	- tds_linkedlist_pushfront
 * 	- tds_linkedlist_popback
 * 	- tds_linkedlist_popfront
 *
 * 	- tds_linkedlist_insert
 * 	- tds_linkedlist_insert_after
 * 	- tds_linkedlist_insert_after
 * 	- tds_linkedlist_delete2
 * 	- tds_linkedlist_delete
 */
void test_best(void)
{
	size_t bufferlen = 100;
	tds_linkedlist *list = tds_linkedlist_create_g(sizeof(long), bufferlen);
	tds_linkedlistiter *iter = NULL;
	long idx = 0;

	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_pushback(list, &idx);
	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_popfront(list);
	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_pushfront(list, &idx);
	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_popback(list);
	assert(0 == tds_linkedlist_len(list));
	assert(bufferlen == tds_linkedlist_bufferlen(list));

	tds_linkedlist_free_buffer(list);
	assert(0 == tds_linkedlist_bufferlen(list));

	idx = 3;
	assert(1 == tds_linkedlist_insert(list, &idx, 0));  /* 3 */
	idx = 2;
	assert(1 == tds_linkedlist_insert(list, &idx, 1));  /* 3,2 */
	idx = 1;
	assert(1 == tds_linkedlist_insert(list, &idx, 1));  /* 3,1,2 */
	tds_linkedlist_delete2(list, 1);                     /* 3,2 */
	assert(2 == tds_linkedlist_len(list));

	idx = 0;
	tds_linkedlist_insert_after(list, tds_linkedlistiter_tail(list), &idx);   /* 3,2,0 */
	idx = 1;
	tds_linkedlist_insert_before(list, tds_linkedlistiter_tail(list), &idx);  /* 3,2,1,0 */
	tds_linkedlist_delete(list, tds_linkedlistiter_tail(list));          /* 3,2,1 */

	iter = tds_linkedlistiter_tail(list);
	idx = 0;
	while (iter != NULL) {
		long *value = (long *)tds_linkedlistiter_data(iter);
		idx += 1;
		iter = tds_linkedlistiter_prev(iter);
		assert(*value == idx);
	}
	tds_linkedlist_free(list);
}

int main(void)
{
	test_0();
	test_worst();
	test_best();
	return 0;
}
