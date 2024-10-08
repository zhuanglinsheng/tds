#include <tds/deque.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

/* testing
 * 	- tds_deque_force_create
 * 	- tds_deque_free
 * 	- tds_deque_len
 * 	- tds_deque_nblks
 * 	- tds_deque_pushfront
 * 	- tds_deque_pushback
 * 	- tds_deque_popfront
 * 	- tds_deque_popback
 */
void test_1(void)
{
	tds_deque *q = tds_deque_force_create(sizeof(int));
	int ele;

	ele = 10;
	tds_deque_pushfront(q, &ele); /* ~~ 10 */

	ele = -10;
	tds_deque_pushback(q, &ele);  /* ~~ 10 | -10 ~~ */

	assert(2 == tds_deque_len(q));
	assert(2 == tds_deque_nblks(q));

	tds_deque_popfront(q);  /* ~~ empty | -10 ~~ */
	assert(1 == tds_deque_len(q));
	assert(2 == tds_deque_nblks(q));
	tds_deque_popfront(q);  /* empty ~~ */
	assert(0 == tds_deque_len(q));
	assert(1 == tds_deque_nblks(q));
	tds_deque_pushfront(q, &ele);  /* -10 ~~ */
	assert(1 == tds_deque_len(q));
	assert(1 == tds_deque_nblks(q));
	tds_deque_popback(q);  /* empty ~~ */
	assert(0 == tds_deque_len(q));
	assert(1 == tds_deque_nblks(q));
	tds_deque_free(q);
}

/* testing
 * 	- tds_deque_create_g
 * 	- tds_deque_free
 * 	- tds_deque_get
 * 	- tds_deque_set
 */
void test_2(void)
{
	tds_deque *q = tds_deque_create_g(sizeof(size_t), 16, 8);
	size_t idx = 0;
	size_t idx_front = 0;
	size_t idx_back = 0;
	size_t n = 100;

	for (idx = 0; idx < n; idx++) {
		if (idx % 2 == 0)
			tds_deque_pushfront(q, &idx);
		else
			tds_deque_pushback(q, &idx);
	}
	for (idx = 0; idx < n; idx++) {
		if (idx % 2 == 0) {
			size_t tmp = *(size_t *) tds_deque_get(q, idx_front) + 1;
			tds_deque_set(q, idx_front, &tmp);
			idx_front++;
		}
	}
	assert(n == tds_deque_len(q));
	idx_front = 0;

	for (idx = 0; idx < n; idx++) {
		if (idx % 2 == 0) {
			size_t tmp_1 = *(size_t *) tds_deque_get(q, idx_front);
			assert(n - 1 - idx_front * 2 == tmp_1);
			idx_front++;
		} else {
			size_t tmp_2 = *(size_t *) tds_deque_get(q, n - 1 - idx_back);
			assert(n - 1 - idx_back * 2 == tmp_2);
			idx_back++;
		}
	}
	tds_deque_free(q);
}

int main(void)
{
	test_1();
	test_2();
	return 0;
}
