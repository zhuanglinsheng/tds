#include <tds/linkedlist.h>

#include <assert.h>
#include <stdio.h>

/* testing basic
 * 	- tds_linkedlist_create
 * 	- tds_linkedlist_free
 * 	- tds_linkedlist_push_back
 *
 * testing iteration
 * 	- tds_linkedlist_head
 * 	- tds_linkedlist_tail
 * 	- tds_linkedlist_node_next
 * 	- tds_linkedlist_node_prev
 */
void test_1(void)
{
	tds_linkedlist *list = tds_linkedlist_create(sizeof(long));
	tds_linkedlist_node *iter = NULL;
	long idx = 0;

	for (idx = 0; idx < 10; idx++)
		tds_linkedlist_push_back(list, &idx);

	idx = 0;
	iter = tds_linkedlist_head(list);
	while (iter != NULL) {
		long *value = (long *)tds_linkedlist_node_data(iter);
		iter = tds_linkedlist_node_next(iter);
		assert(*value == idx);
		idx += 1;
	}

	iter = tds_linkedlist_tail(list);
	while (iter != NULL) {
		long *value = (long *)tds_linkedlist_node_data(iter);
		iter = tds_linkedlist_node_prev(iter);
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
 * 	- tds_linkedlist_buffer_len
 *
 * 	- tds_linkedlist_push_back
 * 	- tds_linkedlist_push_front
 * 	- tds_linkedlist_pop_back
 * 	- tds_linkedlist_pop_front
 *
 * 	- tds_linkedlist_insert
 * 	- tds_linkedlist_insert_after
 * 	- tds_linkedlist_insert_after
 * 	- tds_linkedlist_delete
 * 	- tds_linkedlist_delete_node
 */
void test_2(void)
{
	size_t bufferlen = 100;
	tds_linkedlist *list = tds_linkedlist_create_gen(sizeof(long), bufferlen);
	tds_linkedlist_node *iter = NULL;
	long idx = 0;

	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_push_back(list, &idx);
	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_pop_front(list);
	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_push_front(list, &idx);
	for (idx = 0; idx < bufferlen; idx++)
		tds_linkedlist_pop_back(list);
	assert(0 == tds_linkedlist_len(list));
	assert(bufferlen == tds_linkedlist_buffer_len(list));

	tds_linkedlist_free_buffer(list);
	assert(0 == tds_linkedlist_buffer_len(list));

	idx = 3;
	assert(1 == tds_linkedlist_insert(list, &idx, 0));  /* 3 */
	idx = 2;
	assert(1 == tds_linkedlist_insert(list, &idx, 1));  /* 3,2 */
	idx = 1;
	assert(1 == tds_linkedlist_insert(list, &idx, 1));  /* 3,1,2 */
	tds_linkedlist_delete(list, 1);                     /* 3,2 */
	assert(2 == tds_linkedlist_len(list));

	idx = 0;
	tds_linkedlist_insert_after(list, tds_linkedlist_tail(list), &idx);   /* 3,2,0 */
	idx = 1;
	tds_linkedlist_insert_before(list, tds_linkedlist_tail(list), &idx);  /* 3,2,1,0 */
	tds_linkedlist_delete_node(list, tds_linkedlist_tail(list));          /* 3,2,1 */

	iter = tds_linkedlist_tail(list);
	idx = 0;
	while (iter != NULL) {
		long *value = (long *)tds_linkedlist_node_data(iter);
		idx += 1;
		iter = tds_linkedlist_node_prev(iter);
		assert(*value == idx);
	}
	tds_linkedlist_free(list);
}

int main(void)
{
	test_1();
	test_2();
	return 0;
}
