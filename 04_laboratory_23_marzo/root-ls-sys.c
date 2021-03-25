#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
	char* const* args = NULL;
	system("whoami");
	system("id");

	//execlp("ls", "ls", argv[1], (const char*) NULL);
	if (argc<2 || strlen(argv[1])+4 > 1000) return 1;
	char command[1000];
	sprintf(command, "ls %s", argv[1]);
	printf("%s\n", command);
	system(command);
	// cosa fa execlp? mi permette di eseguire ls essendo root (una volta reso set-UID il programma, dopo chown root)

	// dove lo trova ls?
	//
	// which ls
	// 	/bin/ls
	//
	// prova a vedere con il comando "export" cosa c'è alla riga
	// 	declare -x PATH="/home/seed/bin:/usr/local/sbin: ....."
	//
	// cosa succede se in quella variabile PATH vado a mettere qualcosa che si chiama ls? ovvero, nella prima directory che viene cercata come eseguibile
	//
	// cd /home/seed/bin
	// ls -l
	// 	## vengono fuori cose strane
	// vi ls
	// inserisci nel (nuovo) file la riga
	// 	echo "non lo voglio fare"
	//
	// chmod 755 ls
	//
	// which ls
	// 	/home/seed/bin/ls
	//
	// se fai ls, fa ancora quello originale
	//
	// per vedere la modifica, bisogna aprire un nuovo terminale
	//
	//
	//
	// cd bin
	// rm ls
	// ln -sf /bin/zsh ls
	//
	// in un nuovo terminale, digita
	// 	root-ls

	return 0;
}
