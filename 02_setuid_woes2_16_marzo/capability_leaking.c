#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

	// zzz file is only writable by root
	// sudo chown root zzz
	// sudo chmod 700 zzz


	// file descriptor is created (the program must be a root-owned set-UID program)
	int fd = open("zzz", O_RDWR | O_APPEND);
	if (fd==-1) {
		printf("Cannot open zzz\n");
		exit(0);
	}
	

	printf("fd is %d\n", fd);
	
	// the privilege is downgraded (like dash does)
	// disable privilege by making the effective uid the same as the real uid
	setuid(getuid());
	
	// Execute /bin/sh	(which, in theory, links to /bin/dash)
	//
	
	char *v[2];

	v[0] = "/bin/sh";
	v[1] = 0;	
	execve(v[0], v, 0);

	return 0;
}
