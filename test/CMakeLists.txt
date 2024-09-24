include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test_array test_array.c)
target_link_libraries(test_array tds)
add_test(
	NAME test_array
	COMMAND test_array
)

add_executable(test_bitarray test_bitarray.c)
target_link_libraries(test_bitarray tds)
add_test(
	NAME test_bitarray
	COMMAND test_bitarray
)

add_executable(test_linkedlist test_linkedlist.c)
target_link_libraries(test_linkedlist tds)
add_test(
	NAME test_linkedlist
	COMMAND test_linkedlist
)

add_executable(test_deque test_deque.c)
target_link_libraries(test_deque tds)
add_test(
	NAME test_deque
	COMMAND test_deque
)

add_executable(test_hashtbl test_hashtbl.c)
target_link_libraries(test_hashtbl tds)
add_test(
	NAME test_hashtbl
	COMMAND test_hashtbl
)
