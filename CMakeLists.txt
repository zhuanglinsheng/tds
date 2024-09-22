###############################################################################
# Setting
###############################################################################

cmake_minimum_required(VERSION 3.12)

project(tds LANGUAGES C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_EXTENSIONS OFF)
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_C_FLAGS_DEBUG MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
	else()
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /W4 -fsanitize=address -g")
	endif()
else()
	# Update if necessary
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wno-long-long -pedantic -fsanitize=address -g")
endif()
##
## Installation setting
##
## If user would like to choose their own default installation location,
## set the value of `CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT` to be
## `true` by switch the comments:
##
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "..." FORCE)
set(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT false)
##
## Then set the installation location below by changing the default value
## of `CMAKE_INSTALL_PREFIX`:
##
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "$HOME/.local" CACHE PATH "..." FORCE)
endif()
set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
##
## Third-party libraries
## add_compile_definitions(WITH_BLAS)


###############################################################################
# shared library
###############################################################################

set(SOURCES
	src/string.c
	src/bitarray.c
	src/array.c
	src/linkedlist.c
	src/vector.c
	src/hashtbl.c
	src/deque.c
	src/stack_arr.c
	src/avltree.c
)
add_library(tds SHARED
	${SOURCES}
)
target_include_directories(tds PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

if(BLAS_FOUND)
	message(STATUS "Found BLAS: ${BLAS_LIBRARIES}")
	set(USE_BLAS TRUE)
else()
	message(WARNING "BLAS not found")
	set(USE_BLAS FALSE)
endif()

if(LAPACK_FOUND)
	message(STATUS "Found LAPACK: ${LAPACK_LIBRARIES}")
	set(USE_LAPACK TRUE)
else()
	message(WARNING "LAPACK not found")
	set(USE_LAPACK FALSE)
endif()

if(USE_BLAS AND USE_LAPACK)
	target_link_libraries(tds ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
	target_compile_definitions(tds PRIVATE USE_BLAS USE_LAPACK)
else()
	target_link_libraries(tds)
endif()


###############################################################################
# Installation
###############################################################################

install(TARGETS tds LIBRARY
	DESTINATION lib
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/tds
	DESTINATION include
)


###############################################################################
# Build Mode
#
# Choose the build mode either "Release" or "Debug" by
# cmake -DCMAKE_BUILD_TYPE=Release/Debug ..
###############################################################################

## (Default) The "Release" mode is for installation.
if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O2 -DNDEBUG")
##
## The "Debug" mode is for unit testings.
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")
	enable_testing()
	add_subdirectory(test)
endif ()
