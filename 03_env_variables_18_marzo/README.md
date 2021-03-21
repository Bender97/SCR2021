## ACCESS ENVIRONMENT VARIABLES INSIDE A PROGRAM IN C

Cosa sono le variabili di ambiente?
	Un insieme di coppie chiave-valore
	Sono parte dell'ambiente in cui i processi vengono eseguti
	Hanno effetto sul comportamento dei processi stessi
	Sono state introdotte da unix, poi adottate da tutti gli OS successivi (es. Windows)


char *envp[]
	must terminate with a NULL value to stop the iteration

2 ways:
	1) from the main function 
	2) using a global variable 			-> this is better. why? look after


Il programma che accede direttamente a envp, enumera le variabili d'ambiente. Ma quelle di quale programma?
Come fa un processo ad ottenere le variabili di ambiente?
	1) la shell fa una fork (crea un altro processo) (crea una copia perfetta, a meno del PID, del processo padre)
	2) dentro quel processo fa un exec con quello che gli diciamodi eseguire
	3) fa una join per aspettare il completamento di quell'altro processo

Quindi enumera la variabili d'ambiente del processo FIGLIO della shell (quello che la shell lancia per eseguire il nostro comando)
Lanciando un programma con &, eseguiamo il precesso in virtual background.
Senza quello, non ci ritorna il comando della shell finché quel processo non è finito.

fork ritorna 2 volte:
	al figlio ritorna 0
	al  padre ritorna PID del figlio

Se un programma vuole eseguire un altro programma al suo interno, quello che succede 99 volte su 100 è l'esecuzione di qualche exec.
L'exec fa un overwrite dello spazio di memoria del processo mettendoci sopra l'eseguibile che si vuole eseguire. (è il figlio, non si perde info del codice di partenza)
Anche le variabili d'ambiente vengono sovrascritte!! Ecco perché si usa la versione **execve()**, che gliele passa esplicitamente scegliendo.

	(verdere codice main1.c)

modifiche suggerite a main1:
	non terminare con NULL newenv
		(vedere codice main2.c)
		boh, ho provato e non stampa nulla, come nel caso switch '1'

# Dove stanno in memoria le variabili di ambiente?
ovviamente nello stack, visto che sono "cose in movimento", possono cambiare durante l'esecuzione
N.B. : envp is accessible only in the main function, dato che è un parametro
	   environ è invece globale (esterna allo stack del processo)
	   ma tutte e due contengono l'indirizzo dell'array nello stack del processo

## struttura dello stack di processo
Possiamo identificare 4 zone principali (nell'immagine della slide, gli indirizzi dello stack sono crescenti dall'alto al basso)
 1) zona 1: ci sono le stringhe "vere" di envp (coppie chiave valore) e di argv
 2) array di puntatori di envp[i] che puntano alle stringhe "vere" di envp di cui al punto 1
 3) array di puntatori di argv[i] che puntano alle stringhe "vere" di argv di cui al punto 1
 4) contiene i parametri veri e propri 
 	char \*\*envp, che punta ad envp[0] di cui al punto 2
 	char \*\* argv, che punta ad argv[0] di cui al punto 3
 	int argc

/** di solito, un int nel c ha la dimensione dei registri della macchina
	perché dovrebbe essere il massimo dell'efficienza **/

## il programma env (per info, fare man env)
Può essere usato
 * visualizare le variabili d'ambiente
 * modificare le variabili d'ambiente
 * aggiungere o togliere variabili d'ambiente

Problema! Se aggiungo una variable d'amb, devo per forza cambiare la struttura dello stack

 1) copio la zona 2 (i puntatori di envp) nello heap
 	1) però envp non viene modificato: continua a puntare alla zona 2, ovvero ha la *copia originale* dell'ambiente. Tiene traccia dell'ambiente di partenza.
 	2) environ ha sempre il riferiemento alla *copia corrente* dell'ambiente
 	quindi non c'è modo per avere certezza, usando envp, che punti ad un ambiente originale o modificato: envp non si accorge delle modifiche fatte
 	environ viene aggironata automaticamente ogni volta che viene modificato l'ambiente
 	Perché? envp è una variabile interna al main, il sistema non ha accesso per modificarla. Environ è esterna, il sistema ha accesso e la può aggiornare

