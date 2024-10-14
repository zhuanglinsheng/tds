
clang -c -O2 ../src/avltree.c -o avltree.o
clang++ -std=c++11 -O2 avltree.o ./cmp_avltree.cpp  -o cmp_avltree

