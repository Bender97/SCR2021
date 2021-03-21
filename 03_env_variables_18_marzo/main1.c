/**
 * The program takes in input an operation mode, and
 * prints out or not the environment variables of the
 * current process calling another program located in
 * /usr/bin/env
**/

#include <stdio.h>
#include <unistd.h>

extern char ** environ;

int main(int argc, char *argv[], char *envp[]) {
	int i=0; char * v[2]; char * newenv[3];
	if (argc<2) {printf("please insert argv[1]\n"); return 1;}

	// Construct the argument array
	// /usr/bin/env is a program which prints out
	// the environment variables of the current process
	v[0] = "/usr/bin/env";
	v[1] = NULL;

	// Construct the environment variable array
	newenv[0] = "AAA=aaa";
	newenv[1] = "BBB=bbb";
	newenv[2] = NULL;

	switch(argv[1][0]) {
		case '1': // Passing no environment variable
			execve(v[0], v, NULL);
		case '2':
			execve(v[0], v, newenv);
		case '3':
			execve(v[1], v, environ);
		default:
			execve(v[0], v, NULL);
	}

	return 0;
}
