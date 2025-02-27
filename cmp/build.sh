g++-14 -std=c++11 -O1 ./cmp_avltree.cpp ../src/tds_avltree.c  -o cmp_avltree.exe
g++-14 -std=c++11 -O1 -flto ./cmp_avltree.cpp -ltds  -o cmp_avltree_dy.exe
g++-14 -std=c++11 -O1 -flto ./cmp_avltree.cpp /usr/local/lib/libtds_static.a  -o cmp_avltree_st.exe

