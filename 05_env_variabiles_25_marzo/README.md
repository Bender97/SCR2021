# Attacks via Dynamic Linker
linking dinamico: usa le shared library cercando seguendo le variabili d'ambiente
 * **.so** per *Linux*
 * **.dll** per *Lindows*

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

### LD_PRELOAD
è una variabile d'ambiente che serve a dire al linker quali librerie deve cercare per prime (è una lista di librerie)

### LD_LIBRARY_PATH
Se qualche libreria della lista precedente non è stata trovata, questa variabile dice al linker dove stanno le librerie di sistema.

Entrambe queste due variabili d'ambiente sono controllabili dall'utente, quindi possono indurre problemi.

```
vedere directory ex
```
Se mytest viene reso Set-UID, LD_PRELOAD e LD_LIBRARY_PATH vengono ignorate se RUID e EUID sono diverse.

# Attacks via External Programs (that may use environment variables)
// this program is vulnerable, cal can be searched as we want if we manipulate
// the path env variable
// use execve
```
	int main() { system("cal"); }
```

# Attacks via Library (that may use environment variables)
I programmi usano, come abbiamo visto nel linker, delle librerie esterne. Se le funzioni delle librerie usano delle variabili d'ambiente, ovviamente queste sono vulnerabili all'attacco. <br>
*Caso di studio: il sottosistema locale in Unix.*

# Attacks via Application Code
for example, *buffer overflow* causato da una sprintf che mette una variabile d'ambiente in un array che potrebbe non contenerla tutta, senza fare controlli
 * Countermeasures:
 	* sanificare tutto. Però è molto difficile
 		* usare versioni *sicure* delle funzioni (ovvero che operano in maniera severa quando si accorgono che RUID e EUID differiscono)
 		* invece di offrire possibilità di Set-UID ai programmi, adottare un approccio orientato ai Servizi (offerti da un processo privilegiato, ovvero eseguito da un utente privilegiato)
 			* Il tutto è più sicuro, perché le richieste avvengono tramite uno specifico protocollo
 			* È più facile debuggare un protocollo rispetto ad un programma.
 			* Tuttavia, anche questo approccio offre vulnerabilità (ma è comunque più sicuro: esempio, Android elimina set-UID e mette servizi. Non c'è il root. Rootare Android richiede di modificare il Kernel)


# SHELLSHOCK
Definire una funzione in shell code.<br>
Farsi stampare quella funzione

```
$ foo() { echo "Inside function"; }
$ declare -f foo
	foo()
	{
		echo "Inside function"
	}
```
Se avessi scritto echo, non mi farebbe vedere niente, perché non è una variabile <br>
Però posso rimuovere la funzione dalla shell
```
$ unset -f foo
```

### Come si possono passare le funzioni definite in una shell a processi figli?
* Approccio 1:	# funzione diretta
	1) definisco la funzione nel processo padre
	2) `export -f foo` 	# senza -f, esporterei la variabile foo (vuota)
	3) fatto, il figlio la otterrà
* Approccio 2: 	# variabile che contiene la funzione
	1) `foo='() { echo "hello world"; } '`		# tra apici: non viene interpretato dalla shell
	2) `export foo`
	3) `bash_shellshock`	# eseguo la shell figlia, quella pre-sanificazione
	4) `(child) echo $foo`
			-> niente
	5) `(child) declare -f foo`
			-> interpreta la variabile come funzione! Perché? Nel momento in cui la shell figlia ha ottenuto la variabile, l'ha interpretata. *Gli apici vengono consumati dalla shell genitrice*, quindi la figlia riceve foo senza apici. Ergo una funzione.

Differenze. <br>
 * nel primo caso, occorre che la shell genitrice sia una shell. Oppure un programma che agisce e manipola le variabili esattamente come una shell
 * nel secondo caso, qualunque programma può settare una variabile di ambiente (es, setenv() in C ). Quando questo ambiente verrà ereditato dal processo figlio, se questo processo figlio è una shell, interpreterà in automatico quella variabile d'ambiente per settare una funzione.

### Vulnerability
Se negli apici c'è anche un comando, dopo la dichiarazione della funzione, quel comando verrà eseguito dalla shell figlia.
```
	foo = '() {echo "hello world"; }; echo "extra";'
```
la shell figlia si troverà la funzione, ma eseguirà pure il comando dopo.
> Il baco stava nell'ingenuo riuso di codice: parse_and_execute(string)

Ma perché potrebbe essere **pericoloso**?
Se il processo target è un processo server, o comunque un processo privilegiato, il processo figlio eseguirà comandi con privilegi (security breach)