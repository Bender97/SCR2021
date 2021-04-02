# BUFFER OVERFLOW - INTRODUCTION
Fortran non ha lo stack. Tutti i dati esistono per tutta la durata dell'esecuzione (località è solo per i nomi).<br>
Sistemi Operativi moderni != tutti i sistemi operativi ad oggi in uso.<br>
ad esempio: pSOS (real time) non ha meccanismi di protezione della memoria.<br>
*protezione della memoria* è una contromisura inter-process, non intra-process.<br>
Alcuni OS moderni hanno anche protezioni intra-process.<br>

## STACK LAYOUT
Quali sono le parti della memoria che vengono occupate dalla memoria durante l'esecuzione?<br>
*Normalmente*: Stack parte dagli indirizzi più alti e va verso quelli iniziali.
**BSS & Data**: contengono i dati globali
 * BSS: non inizializzati o messi a zero per default			`static int y`
 * Data: dati inizializzati										`int x = 100; // fuori da funzioni`
 * Text: il codice del programma (linguaggio macchina)

Quanto è grande un `int *ptr*?` dipende dall'architettura. E' la dimensione del registro degli indirizzi del processore.<br>
*malloc*: punta a memoria nello heap.

**static** ha significati diversi per C e Java:
	* *static* in Java: ce n'è una copia sola per tutte le possibili istanze della classe
	* *static* in C: non salvarla nello stack, né nello heap, salvala in Data o BSS segment.

Oltre che per le variabili automatiche, lo stack viene usato anche per il passaggio dei parametri.<br>
GCC:
	* I primi parametri li passo tramite dei registri, non tramite lo stack: questioni di velocità.
Machinery Storica:
	* push dei parametri sullo stack, poi la funzione chiamata fa pop dallo stack per recuperarli
	* c'è un modo di compilare con gcc per fargli usare solo stack

Lo stack viene usato anche per i valori di ritorno.

 * **ebp**: stack pointer
 * **eax**: registro accumulatore, è sempre il target di tutte le operazioni matematiche
 * i parametri vengono passati in reverse order
```
 +12	b 	; (da +12 a +8)
 +8	    a 	; (da  +8 a +4)
 +4	    	; return value
 ebp  		; previous frame pointer
 -4	    x 	; (da -4 a -8)	le variabili crescono dopo ebp
 -8     y
```
*assembly x86*: l'architettura intel moderna è stratificata. E' compatibile, ma internamente è risk load store, con una pipeline molto profonda. <br>
Macchina accumulatore: due operandi, e risultato salvato in **eax**. Poi lo prelevo con movl e lo salvo da qualche parte.

## VULNERABLE PROGRAM
 * Già il main ha una vulnerabilità: non controlla che badfile sia NULL dopo fread.
	In un sistema a memoria protetta, segmentation fault. In MS-DOS: boom (negli indirizzi zero sono salvati dati per interrupt)
 * in foo()
 	* buffer[100] è più piccolo di str[400].
 	* Scrivo i primi 100 byte nello stack correttamente, gli altri 200 "piallo" tutto quello che c'era prima nello stack!
 	* Se il badfile era tutti zeri, scrivo indirizzo di ritorno uguale a zero, e senza memoria protetta avrei seri problemi.
 	* Se invece scrivo indirizzi coerenti con lo spazio di memoria del processo, ritorno in quei punti: **control hijacking**.

 * **control hijacking** Dirottamento del flusso del programma.

## CONSEQUENCES
 * invalid instructions, non-existing address, access violation: BOOM, os detects the problem.
 * attacker's code: eseguo del codice che non era da nessuna parte: DoS (con tutti zeri) oppure *gain access*

che struttura deve avere il **badfile** per causare danni?
 * i primi bytes sono irrilevanti (vengono scritti nello stack senza sovrascrivere niente)
 * *return address* deve essere sovrascritto con *new address*
 * il *malicious code* (il codice vero e proprio) deve essere puntato dal *new address*

## COME SETTARE L'AMBIENTE PER FARE GIRARE QUESTI CODICI
 * togliere la randomizzazione dello spazio degli indirizzi
 * in compilazione, dire a gcc di non usare lo stack protector e di rendere non eseguibile tutto ciò che c'è nello stack