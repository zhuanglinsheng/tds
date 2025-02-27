#include <map>
#include <chrono>
#include <iostream>

#include <tds/avltree.h>

struct avl_pair {
	int __key;
	int __data;
};

int cmp_pair(const void *pair_1, const void *pair_2)
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
	
	/*======== std map ========*/
	std::map<int, int> cpp_map;
	
	/*1. insert */
	auto start_cpp1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		int data = i - 1;
		cpp_map[i] = data;
	}
	auto end_cpp1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_cpp1 = end_cpp1 - start_cpp1;
	std::cout << "C++ Map (insert): " << elapsed_cpp1.count() << " ms" << std::endl;
	
	/* 2. search */
	long cpp_search_sum = 0;
	auto start_cpp2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		cpp_search_sum += cpp_map[i];
	}
	auto end_cpp2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_cpp2 = end_cpp2 - start_cpp2;
	std::cout << "C++ Map (search): " << elapsed_cpp2.count() << " ms" << std::endl;
	std::cout << "C++ Map (search): " << "sum = " << cpp_search_sum << std::endl;
       
       	/* 3. delete node */
	auto start_cpp3 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		cpp_map.erase(i);
	}
	auto end_cpp3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_cpp3 = end_cpp3 - start_cpp3;
	std::cout << "C++ Map (delete): " << elapsed_cpp3.count() << " ms " << std::endl;
	std::cout << "C++ Map (delete): " << "map size = " << cpp_map.size() << std::endl;
	std::cout << std::endl;
        
	/*======== avl ========*/
	tds_avltree *tds_avl = tds_avltree_create(sizeof(struct avl_pair));
	struct avl_pair pair;
	
	/* 1. insert */
	auto start_c1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		pair.__key = i;
		pair.__data = i - 1;
		tds_avltree_insert(tds_avl, &pair, cmp_pair);
	}
	auto end_c1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_c1 = end_c1 - start_c1;
	std::cout << "TDS AVL (insert): " << elapsed_c1.count() << " ms" << std::endl;
	
	/* 2. search */
	long c_search_sum = 0;
	struct avl_pair *pair_2;
	auto start_c2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		pair.__key = i;
	        pair_2 = (struct avl_pair *) tds_avltree_get(tds_avl, &pair, cmp_pair);
		pair.__data = pair_2->__data;
		c_search_sum += pair.__data;
	}
	auto end_c2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_c2 = end_c2 - start_c2;
	std::cout << "TDS AVL (search): " << elapsed_c2.count() << " ms" << std::endl;
	std::cout << "TDS AVL (search): " << "sum = " << c_search_sum << std::endl;

	/* 3. delete node */
	auto start_c3 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		pair.__key = i;
		tds_avltree_delete(tds_avl, &pair, cmp_pair);
	}
	auto end_c3 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed_c3 = end_c3 - start_c3;
	std::cout << "TDS AVL (delete): " << elapsed_c3.count() << " ms" << std::endl;
	std::cout << "TDS AVL (delete): " << "map size = " << tds_avltree_len(tds_avl) << std::endl;

	tds_avltree_free(tds_avl);
	return 0;
}

