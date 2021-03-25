# Attacks via Dynamic Linker
linking dinamico: usa le shared library cercando seguendo le variabili d'ambiente
 * so per linux
 * dll per windows

La libreria è caricata in memoria la prima volta tutta, non c'è una distinzione a livello della singola funzione.<br>
Poi però grazie alla paginazione può essere che rimanga in memoria solo la/le funzione/i utilizzata.

## ldd command
comando per vedere da quali librerie dinamiche un programma dipende	
```	
	ldd hello_static
			# non mostra niente se il programma è statico
	ldd hello_dynamic
			# lists the dynamic libraries
```

Questo vuol dire che anche dopo la compilazione, parte del programma è "undecided", ovvero viene colmata grazie al linking.<br>
Ecco la superficie d'attacco.