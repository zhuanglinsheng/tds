#include <map>
#include <chrono>
#include <iostream>

#include <tds/avltree.h>

struct avl_pair {
	int __key;
	int __data;
};

int cmp_pair(void *pair_1, void *pair_2)
{
	struct avl_pair *p1 = (struct avl_pair *) pair_1;
	struct avl_pair *p2 = (struct avl_pair *) pair_2;

	if (p1->__key < p2->__key)
		return -1;
	else if (p1->__key == p2->__key)
		return 0;
	else
		return 1;
}

int main(void)
{
	int n = 10000000;
	/* std map */
	auto start_1 = std::chrono::high_resolution_clock::now();
	std::map<int, int> cpp_map;
	for (int i = 0; i < n; i++) {
		int data = i - 1;
		cpp_map[i] = data;
	}
	auto end_1 = std::chrono::high_resolution_clock::now();

	/* avl */
	auto start_2 = std::chrono::high_resolution_clock::now();
	tds_avltree *tds_avl = tds_avltree_create(sizeof(struct avl_pair));
	struct avl_pair pair;
	for (int i = 0; i < n; i++) {
		pair.__key = i;
		pair.__data = i - 1;
		tds_avltree_insert(tds_avl, &pair, cmp_pair);
	}
	auto end_2 = std::chrono::high_resolution_clock::now();
	tds_avltree_free(tds_avl);

	/* compare */
	std::chrono::duration<double, std::milli> elapsed_1 = end_1 - start_1;
	std::chrono::duration<double, std::milli> elapsed_2 = end_2 - start_2;

	std::cout << "C++ Map: " << elapsed_1.count() << " ms" << std::endl;
	std::cout << "TDS AVL: " << elapsed_2.count() << " ms" << std::endl;
	return 0;
}
