gcc -c sleep.c
gcc -shared -o libmylib.so.1.0.1 sleep.o
# doing export here will not modify the shell env
# export LD_PRELOAD=./libmylib.so.1.0.1
