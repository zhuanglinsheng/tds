# tds

This is a general-purpose data structure library implemented in the pure ISO C90 standard. The library is expected to include the following components:

1. Linear list

   - [x] [sting.h](./include/tds/string.h)
   - [x] [bitarray.h](./include/tds/bitarray.h)
   - [x] [array.h](./include/tds/array.h)
   - [x] [vector.h](./include/tds/vector.h)
   - [ ] [deque.h](./include/tds/deque.h)
   - [x] [hashtbl.h](./include/tds/hashtbl.h)
   - [x] [stack_arr.h](./include/tds/stack_arr.h)
   - [x] [stack_deq.h](./include/tds/stack_deq.h)
   - [x] [queue_deq.h](./include/tds/queue_deq.h)

2. Linked list

   - [ ] [list.h](./include/tds/list.h)
   - [ ] [queue_lst.h](./include/tds/queue_lst.h)
   - [ ] [stack_lst.h](./include/tds/stack_lst.h)

3. Tree

   - [ ] [avltree.h](./include/tds/avltree.h)
   - [ ] [rbtree.h](./include/tds/rbtree.h)

4. Other

   - [ ] ...

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
