# tds

A general-purpose data structure library implemented in the pure ISO C90 standard.

The library is expected to include the following components:

## General Purpose Data Structure

   - [x] [bitarray.h](./include/tds/bitarray.h),
         [bitarray.c](./src/tds_bitarray.c),
         [test_bitarray.c](./test/test_bitarray.c)
   - [ ] [nbitsarray.h](./include/tds/nbitsarray.h)
   - [ ] [bytearray.h](./include/tds/bytearray.h)
   - [x] [array.h](./include/tds/array.h),
         [array.c](./src/tds_array.c),
         [test_array.c](./test/test_array.c)
   - [x] [arraylist.h](./include/tds/arraylist.h),
         [arraylist.c](./src/tds_arraylist.c),
         [test_arraylist.c](./test/test_arraylist.c)
   - [x] [linkedlist.h](./include/tds/linkedlist.h),
         [linkedlist.c](./src/tds_linkedlist.c),
         [test_linkedlist.c](./test/test_linkedlist.c)
   - [ ] [skiplist.h](./include/tds/skiplist.h)
   - [x] [hashtbl.h](./include/tds/hashtbl.h),
         [hashtbl.c](./src/tds_hashtbl.c),
         [test_hashtbl.c](./test/test_hashtbl.c)
   - [x] [avltree.h](./include/tds/avltree.h),
         [avltree.c](./src/tds_avltree.c),
         [test_avltree.c](./test/test_avltree.c)
   - [ ] [rbtree.h](./include/tds/rbtree.h)
   - [x] [deque.h](./include/tds/deque.h),
         [deque.c](./src/tds_deque.c),
         [test_deque.c](./test/test_deque.c)
   - [ ] [stack.h](./include/tds/stack.h)
   - [ ] [queue.h](./include/tds/queue.h)
   - [ ] ...

## Algorithms

   - [ ] [sort.h](./include/ta/sort.h)

## Compare with C++ STL

...

## Install

The library is organized using CMake. You can install the project in the standard way:

```sh
mkdir build
cd build
cmake ..
make
make install
```

The project is still under active development. Discussions and pull requests are welcome.
