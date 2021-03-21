# come è formattato vi /etc/passwd   ?

	root:x:0:0:root:/root:/bin/bash
	1) nome della login
		 2) una volta c'era la password. Adesso se c'è la x vuol dire che la password è nel file shadow
		   3) user ID (UID)
		     4) group ID
		       5) group name
		       		6) home directory
		       			   7) shell
tutti gli account che seguono, sono "messi in sicurezza" dal fatto che hanno come shell "/usr/sbin/nologin".
	Cos'è nologin? Sapete come funziona la definizione della shell, quando completi l'operazione di autenticazione con login e password, ti viene attivata quella shell.
	/bin/bash -> ti apre bash
	/nologin  -> ti risponde "attaccati al tram"
	/sync	  -> fa il writethrough dei dischi. Tutti i dati in memoria vengono effettivamente scritti nei dischi (cache nel sistema operativo flushate. Ormai non più molto utilizzato perché adesso i dischi magnetici o SSD hanno tipicamente una cache loro interna, quindi il cache viene delegato a loro stessi. Linux è ancora simil-anni 70, quindi ha ancora meccanismi "legacy")
	/bin/false -> do nothing, unsuccessfully (un due di picche silente)

suscettibile all'attacco con campi assenti (le virgole)



# GOAL: se io sono un user non root, voglio rendere possibile "cat" a tutti

scrivo catall.c
```
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
```
faccio eseguire il codice con:
	```./catall catall.c /etc/passwd /etc/shadow```

legge tutto tranne shadow: permission denied
perché è un programma NON setUID
```
	chown root catall
	FAIL: non posso farlo, operation not permitted (devo usare sudo)
	sudo bash
	ls -l	# per verificare che sono sempre sullo stesso path
	chown root catall 
	ls -l 	# verifico che catall abbia owner root
	exit	# per uscire da bash root
```
ora, se rieseguo il codice, ottengo lo stesso risultato (permission denied con shadow)
	il programma è di root ma chi lo sta eseguendo è sempre seed
	bisogna aggiungere setUID
```
	sudo bash
	chmod 4755 catall
	ls -l 	# verifico che catall sia setUID
	exit
```
rieseguo il codice. WHOPS! Non funziona. Perché?
	system è sanitizzato per setUID. Downgrada l'user al suo real UID
	dobbiamo usare exec!

	execv("/bin/cat", &(argv[1]));

Ogni funzione della famiglia exec vuole, come path del programma da eseguire, il suo path assoluto!
	* system: è stato sanitizzato per non eseguire comandi senza aver fatto il drop dei privilegi (torna a quella di partenza)
	* exec: mantiene i privilegi ma non può essere attaccata tramite variabile d'ambiente
		è suscettibile a:
			* andare in /bin/ e sostituire il programma che viene chiamato
			* solo che, io non ho diritti di scrittura su /bin/


> dash: è una bash che droppa i privilegi quando parte come set-uid process (è una shell a privilegi ridotti)
>	da notare che con ls -l /bin/sh
>			viene fuori che sh -> dash. Ovver, sh linka a dash
>			questo c'è in ubuntu 16.04 (vedere slides, A Note)
