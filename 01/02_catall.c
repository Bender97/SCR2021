/***
	GOAL: rendere il comando cat "privilegiato", ovvero consentire ad utenti non privilegiati di leggere il file /etc/shadow

	da usare previa
		sudo bash
		chown root 02_catall
		chmod 4755 02_catall
		exit
		ls -l	# just to check

##### WARNING!!!!!   this code doesn't work in ubuntu 18.04 LTS

***/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
	char buf[1024];
	int i=0, j=0;
	system("whoami");
	for (i=1; i<argc; i++) {
		for (j=0; j<1024;j++) buf[j]=0;
		printf("%s\n", argv[i]);
		if (strlen(argv[i])+5 > 1024) {printf("name too long"); continue;}
		//strcat(buf, "cat ");
		//strcat(buf, argv[i]);
		//printf("command: %s\n", buf);
		execv("/bin/cat", &(argv[1]));
	}
	return 0;
}
