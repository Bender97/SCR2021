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
```(vedere codice main2.c) ```
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


# Lezione 2

## variabili d'ambiente e variabili della shell

shell: interfaccia tra l'utente e l'OS, funzionante senza grafica ma da command line. E' la prima forma di interattività con l'OS.
Anche il desktop è una shell
sh, bash, dash, zsh, eccetera
	La shell, quando parte, copia le variabili d'ambiente (passate dalla shell genitrice) in variabili interne.
	Posso andare a manipolare le variabili della shell per modificare l'ambiente della shell, senza modificare l'ambiente globale.
	Quando partono, tutte le shell ereditano lo stesso ambiente (quello dell'utente)
	Poi viene copiato nello spazio di indirizzi della singola shell
	Da lì in poi, ogni shell puù modificare il suo. Perché?
		Una shell è sia una command line interface sia un "ambiente di programmazione", quindi per fare girare uno script, ad esempio.

	FOO=bar
	echo $FOO
	unset FOO # questo rimuove la variabile FOO della shell
	echo $FOO

	unset PATH	# questo rimuove la variabile PATH della shell, ovvero la copia locale della variabile d'ambiente.

## lo pseudo file system /proc
è una cartella, che ha una cartella per ogni processo chiamata con il PID.
La cartella di un processo contiene anche l'environment del processo
	
	strings # programma che mostra le stringhe di testo stampabili in un file (di default, lunghe almeno 4 caratteri)
	strings /proc/$$/environ 	# $$ è uno shell escape per il PID corrente
				# stampa l'environment della shell

env fa vedere l'environment ereditato! (quello su cui gira env)
	strings /proc/$$/environ fa vedere l'ambiente corrente

normalmente non si vedono differenze, ma se si usa **env** per fare modifiche all'ambiente, allora si
	
	env | grep LOGNAME
		LOGNAME=seed
	strings /proc/$$/environ | grep LOGNAME
		LOGNAME=seed
	LOGNAME="bob"		# con questo, modifico la variabile shell, non la variabile d'ambiente
	env | grep LOGNAME
	LOGNAME = "bob"
	strings /proc/$$/environ | grep LOGNAME
		LOGNAME=seed
      
	unset LOGNAME
	echo $LOGNAME	# come fare env
    # vuoto cosmico
	strings /proc/$$/environ | grep LOGNAME
	LOGNAME=seed

Quando la shell forka e fa exec per eseguire un comando:
	copia il suo ambiente nel figlio
		quali variabili copia? Le prende dall'ambiente originale? No.
		(bash) Fa una selezione diversa:
		1) Propaga le variabili shell che sono state copiate dall'ambiente
		2) Propaga le variabili shell che, anche se non sono state copiate dall'ambiente, risultano essere marcate per l'esportazione (export, un built-in di bash)

Attenzione, è possibile non fare ereditare variabili d'ambiente (globali) ad un processo figlio:
	se faccio unset di una variabile shell (locale), questa non verrà propagata


	FOO=bar 		# NON marcata per export
	export FOO=bar 	#     marcata per export


quindi: Variabili d'ambiente ricevute dal processo figlio (che si copia nelle sue variabili interne) sono le variabili shell copia dell'ambiente che il padre aveva ereditato + le variabili di shell che il padre ha settato e marcato per l'export


# Qual è la attack surface creata dalle variabili d'ambiente?
## dove sta il pericolo
Ci sono diverse componenti del sistema che usano le variabili d'ambiente.
Poiché queste sono modificabili dall'utente, bisogna fare in modo che le parti del sistema non prendino una brutta strada usandole.
esempio di parti del sistema che usano variabili d'ambiente: 
* linker  # per trovare i pezzi e risolvere i simboli
* codice all'interno dell'applicazione # per lanciare programmi esterni, per trovare le librerie, o per usarle direttamente

# danni derivati da applicazione che usa variabili d'ambiente *unsafe*
Ho un demone che deve fornire un servizio. Se so che va in crash con un certo input, posso interrompere il servizio in questo modo: *Denial Of Service*
	i.e. Ping of Death: fa crashare Windows
Se il servizio ha un restart automatico?
Beh comunque mando in sovraccarico il sistema per farlo ripartire:
	1) Devo interagire con il file system (devo caricare l'eseguibile)
		Il FS ha tempi epocali rispetto all'esecuzione in processore. Già la RAM è lenta, figurati da disco (diversi ordini di grandezza più lento).
	2) Per rilanciarlo, gli devo riallocare la memoria
		La malloc e la free, sono le chiamate di sistema più bloccanti
		Quando faccio quelle lì, tutto il resto deve stare fermo, perché la memoria la usano tutti, e cambiandone lo stato, occorre che un solo processo per volta ne abbia controllo (ad esempio il merge di due zone di memoria da unire per crearne una zona sola libera)
Il danno più piccolo è il Denial of Service, che manda in crash solo un programma, ma poi ci sono tanti comportamenti molto più dannosi, generatori di danni futuri.
Ad esempio, installare un eseguibile che sia in contatto con un Command&Control service, che arruola la macchina in un botnet. La faccio diventare uno zombie. 