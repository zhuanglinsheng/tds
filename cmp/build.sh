g++ -std=c++11 -O2 ./cmp_avltree.cpp ../src/tds_avltree.c  -o cmp_avltree.exe
g++ -std=c++11 -O2 -flto ./cmp_avltree.cpp -ltds  -o test_avltree_dy.exe
g++ -std=c++11 -O2 -flto ./cmp_avltree.cpp /usr/local/lib/libtds_static.a  -o test_avltree_st.exe

