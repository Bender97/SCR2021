gcc -g -o stack -w -z execstack -fno-stack-protector main.c
chown root stack
chmod 4755 stack
