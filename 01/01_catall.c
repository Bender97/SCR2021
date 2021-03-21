/***
	GOAL: rendere il comando cat "privilegiato", ovvero consentire ad utenti non privilegiati di leggere il file /etc/shadow

	questo programma però non ci riesce, dato che system è sanitizzato (downgrada l'user al suo real UID se il codice è eseguito come setUID

	occorre usare una funzione della famiglia exec
***/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char buf[1024];
	int i=0, j=0;
	for (i=1; i<argc; i++) {
		for (j=0; j<1024;j++) buf[j]=0;
		printf("%s\n", argv[i]);
		if (strlen(argv[i])+5 > 1024) {printf("name too long"); continue;}
		strcat(buf, "cat ");
		strcat(buf, argv[i]);
		printf("command: %s\n", buf);
		system(buf);
	}
	return 0;
}
