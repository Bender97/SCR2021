gcc mytest.c -o mytest
sh compile.sh
export LD_PRELOAD=./libmylib.so.1.0.1
./mytest
