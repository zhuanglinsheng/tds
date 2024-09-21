include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(test_tstring test_tstring.c)
target_link_libraries(test_tstring tds)
add_test(
	NAME test_tstring
	COMMAND test_tstring
)

add_executable(test_tbitarray test_tbitarray.c)
target_link_libraries(test_tbitarray tds)
add_test(
	NAME test_tbitarray
	COMMAND test_tbitarray
)


add_executable(test_tarray test_tarray.c)
target_link_libraries(test_tarray tds)
add_test(
	NAME test_tarray
	COMMAND test_tarray
)
