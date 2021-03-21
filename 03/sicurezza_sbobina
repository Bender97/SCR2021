access environment variables inside a program in c

char *envp[]
	must terminate with a NULL value to stop the iteration

2 ways:
	1) from the main function 
	2) using a global variable 			-> this is better. why? look after


come fa un processo ad ottenere le variabili di ambiente?
	1) la shell fa una fork (crea un altro processo)
	2) dentro quel processo fa un exec con quello che gli diciamodi eseguire
	3) fa una join per aspettare il completamento di quell'altro processo

	