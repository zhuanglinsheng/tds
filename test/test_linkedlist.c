#include <tds/linkedlist.h>

#include <assert.h>
#include <stdio.h>

/* testing
 * 	- tds_linkedlist_create
 * 	- tds_linkedlist_free
 */
void test(void)
{
	tds_linkedlist *list = tds_linkedlist_create(sizeof(long));
	tds_linkedlist_node *iter = NULL;
	long idx = 0;

	for (idx = 0; idx < 10; idx++) {
		if (idx % 2 == 0)
			tds_linkedlist_push_back(list, &idx);
	}
	idx = 0;  /* reset `idx` */
	iter = tds_linkedlist_head(list);

	while (iter != NULL) {
		long *value = (long *)tds_linkedlist_node_data(iter);
		iter = tds_linkedlist_node_next(iter);
		printf("value = %li\n", *value);
		assert(*value == idx);
		idx += 2;
	}
	idx = 0;  /* reset `idx` */
	iter = tds_linkedlist_head(list);

	while (iter != NULL) {
		/* code */
		break;
	}
	tds_linkedlist_free(list);
}

int main(void)
{
	test();
	return 0;
}
