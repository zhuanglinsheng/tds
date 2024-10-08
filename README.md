# tds

This is a general-purpose data structure library implemented in the pure ISO C90 standard. The library is expected to include the following components:

## General Purpose Data Structure

   - [x] [bitarray.h](./include/tds/bitarray.h)
   - [ ] [nbitsarray.h](./include/tds/nbitsarray.h)
   - [ ] [bytearray.h](./include/tds/bytearray.h)
   - [x] [array.h](./include/tds/array.h)
   - [x] [arraylist.h](./include/tds/arraylist.h)

   - [x] [linkedlist.h](./include/tds/linkedlist.h)
   - [ ] [skiplist.h](./include/tds/skiplist.h)

   - [x] [hashtbl.h](./include/tds/hashtbl.h)

   - [x] [deque.h](./include/tds/deque.h)

   - [x] [stack_arr.h](./include/tds/stack_arr.h)
   - [ ] [stack_lst.h](./include/tds/stack_lst.h)
   - [ ] [stack_deq.h](./include/tds/stack_deq.h)

   - [ ] [queue_deq.h](./include/tds/queue_deq.h)
   - [ ] [queue_lst.h](./include/tds/queue_lst.h)

   - [x] [avltree.h](./include/tds/avltree.h)
   - [ ] [rbtree.h](./include/tds/rbtree.h)
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
