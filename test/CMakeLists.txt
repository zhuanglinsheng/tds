include_directories(${PROJECT_SOURCE_DIR}/include)

##
## Algorithms
##
add_executable(test_sort test_sort.c)
target_link_libraries(test_sort tds_static)
add_test(
	NAME test_sort
	COMMAND test_sort
)

##
## Data Structure
##

add_executable(test_array test_array.c)
target_link_libraries(test_array tds_static)
add_test(
	NAME test_array
	COMMAND test_array
)

add_executable(test_arraylist test_arraylist.c)
target_link_libraries(test_arraylist tds_static)
add_test(
	NAME test_arraylist
	COMMAND test_arraylist
)

add_executable(test_bitarray test_bitarray.c)
target_link_libraries(test_bitarray tds_static)
add_test(
	NAME test_bitarray
	COMMAND test_bitarray
)

add_executable(test_linkedlist test_linkedlist.c)
target_link_libraries(test_linkedlist tds_static)
add_test(
	NAME test_linkedlist
	COMMAND test_linkedlist
)

add_executable(test_deque test_deque.c)
target_link_libraries(test_deque tds_static)
add_test(
	NAME test_deque
	COMMAND test_deque
)

add_executable(test_hashtbl test_hashtbl.c)
target_link_libraries(test_hashtbl tds_static)
add_test(
	NAME test_hashtbl
	COMMAND test_hashtbl
)

add_executable(test_avltree test_avltree.c)
target_link_libraries(test_avltree tds_static)
add_test(
	NAME test_avltree
	COMMAND test_avltree
)
