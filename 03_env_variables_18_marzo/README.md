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