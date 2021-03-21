
altro problema di system: fargli aprire una shell ed ottenere root
	problema sanificato in ubuntu grazie al fatto che system apre una sh che linka a dash, la quale fa drop dei privilegi

> per rimuovere la contromisura imposta da sh->dash, basta linkare sh a zsh, che non fa drop dei privilegi

PROBLEMA: in Ubuntu 18.04 non c'è zsh (è la shell di Arch Linux, fonte: https://wiki.ubuntu-it.org/Programmazione/CambiareShell)
	basta installarla, comunque

	# Before experiment: link /bin/sh to /bin/zsh
	$ sudo ln -sf /bin/zsh /bin/sh

	# After experiment: remember to change it back
	$ sudo ln -sf /bin/dash /bin/sh

	l'experiment, è usare catall con system(buf) alla fine, facendolo girare con:
		./catall "aaa; /bin/sh"
			## nota bene: virgolette NeCeSsArIe


facciamo una shell

	(vedi codice root-zsh.c)


come invocare programmi in modo sicuro?
	usando execve, ovvero separando "comandi" da "parametri"
		ma anche execve è suscettibile agli attacchi da variabili di ambiente

	catall come lo trova il programma da eseguire?
	se usiamo execv or execl devo dargli il path completo, quindi il programma è esattamente quello che gli dico io
	però esistono versioni che ereditano l'ambiente dalla shell di partenza, quindi anche il path, e quindi anche l'ordine con cui vado a cercare gli eseguibili. (execp)

attacco via environment variable
	(vedi codice root-ls.c)
		il codice usa:
			execlp("ls", "ls", argv[1], (const char* ) NULL);

una volta che so che c'è un programma che usa execlp, non ho bisogno di avere diritti di root per ottenere una shell di root.

1) Devo cambiare il path della mia shell (export PATH="/path/to/dir:$PATH")
2) mettere un link simbolico dentro alla prima cartella dove punta il mio path (esempio, /home/seed/bin)



invece di usare una shell (system), uso exec: separando comandi da parametri, è più difficile far passare comandi per pseudodati

neanche exec è invulnerabile:
	nelle versioni in cui eredita l'ambiente, può essere attaccato tramite l'ambiente. Quindi bisogna essere ancora più restrittivi!
		Il path devi darlo assoluto -> vulnerabile, perché alcune variabili d'ambiente definiscono l'ambiente stesso (decidono da dove si parte a cercare)
			QUELLO davvero sicuro è l'ultra ristretto

			ovvero
				1) path assoluto
				2) non eredita l'ambiente
				lista dei parametri terminata da NULL
				execl("/bin/ls", "/bin/ls", argv[1], (const char*) NULL);



CAPABILITY LEAKING

	cosa fa su? ti fa diventare un altro utente
	ma solo root può diventare un altro utente
	quindi, 


	(vedi codice capability_leaking, che deve essere own da root e chmod set-uid)

	ti apre una shell. Apre un fd to il file, ma non lo chiude, quindi facendo

	esempio: se fd=3:

		echo "ciao" >& 3

	scrive nel file. Che quando è stato aperto, è stato aperto con i diritti di root. La shell non ha privilegi di root (tra l'altro, è pure dash), eppure riusciamo a scriverci

	apriamo un terminale (dopo setuid) perché vogliamo preservare la file descriptor table (che è visibile con qualche comando)
