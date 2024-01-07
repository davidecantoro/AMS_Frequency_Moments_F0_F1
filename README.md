# AMS Frequency Moments

## Stream generator
Per generare lo stream utilizzato in questo lavoro si è utilizzato un generatore di numeri pseudo-casuali. Il generatore è disponibile nella directory "stream_generator" ed è stato implementato in C++. Il generatore si occupa, in aggiunta, del salvataggio dello stream in formato CSV.

In tale directory sono presenti due script di utility scritti in bash chiamati "test_generatore.sh" e "controllo_input.sh" che hanno lo scopo di testare le combinazioni di input disponibili, controllando sia il corretto funzionamento delle opzioni disponibili che il corretto funzionamento dei meccanismi di controllo dell'input inserito da utente.

Un Makefile è disponibile, in modo da semplificare la compilazione del programma.

Le distribuzioni implementate per generare numeri pseudo-casualmente sono:
- uniforme
- esponenziale
- poisson

### Usage
```
Utilizzo: ./random [-d distribuzione] [-l lambda] [-a min] [-b max] [-n lunghezza] [-f file]
Il seguente programma genera uno stream di numeri pseudo-casuali, salvando il risultato in un file in formato CSV.
ATTENZIONE: Il seguente programma fornisce in output un file CSV di numeri interi, quindi per conservare le cifre decimali bisogna utilizzare l'opzione x
Le opzioni disponibili sono le seguenti:
  -h                Messaggio di aiuto
  -d distribuzione  Permette di specificare una distribuzione da usare: uniforme, esponenziale, poisson. Se non specificato, verra' utilizzata la distribuzione uniforme
  -l lambda         Permette di specificare il parametro lambda usato per le distribuzioni esponenziale e Poisson
  -a min            Permette di specificare il limite inferiore per la distribuzione uniforme
  -b max            Permette di specificare il limite superiore per la distribuzione uniforme
  -n lunghezza      Permette di specificare la lunghezza dello stream
  -x cifre          Permette di specificare il numero di cifre decimali da mantenere
  -f file           Permette di specificare il nome del file CSV
```

### Implementazione
La libreria getopt è stata utilizzata in modo da permettere l'utilizzo delle opzioni.

La funzione err_sys è stata implementata in modo da mostrare a schermo un messaggio di errore descrittivo nel caso l'input fornito da utente non sia corretto e di terminare l'esecuzione del programma.

Le funzioni "uniforme" e "esponenziale" sono state adattate per generare numeri interi: il numero generato viene moltiplicato per $10^x$, dove $x$ è uno dei parametri modificabili, per poi venire troncato.

Sono stati eseguiti i seguenti controlli sull'input inserito da utente:
- che dopo i numeri (a,b,lambda, x ed n) non ci siano lettere o simili
- che la lunghezza dello stream e x siano un numeri interi e positivi
- che a,b e lambda numeri decimali positivi
- che  i numeri decimali e interi (a, b, lambda, x e n) siano inferiori al valore massimo possibile.


## Estimation of $F_0$
Il paper di Alon et al., [1] propone una modifica all'algoritmo Flajolet-Martin proposto da Flajolet et al., [2] per calcolare il momento di ordine 0.

Il momento di ordine k = 0, indicato anche $F_0$, è utilizzato per stimare il numero di elementi distinti in uno stream. 

Sia definito il campo $F=GF(2^d)$, dove d è il più grande intero t.c. $2^d>n$.

Siano $a,b$ due numeri casuali definiti in $F$, si computi $z_i = a * a_i +b$, con prodotto e somma riferiti al campo $F$. 
La funzione z così definita, come fatto notare da Alon et al., [1], fornisce un mapping pairwise independent.

Sia $r(z)$ il numero di trailing $0s$. Sia $R$ il massimo valore di $r_i$, dove $r_i=r(z_i)$.

L'output dell'algoritmo sarà $Y=2^R$.


### Pseudocode
```
AMS_Frequency_Moment_0(A): # A stream
  // initialize...
  a, b random chosen
  
  // procedure...
  R <- (- inf)
  while(stream)
    z_i <- z(a_i)
    r_i <- r(z_i)
    R <- Max(r_i,R)
  end
  return 2^R

define z: z=a*x+b
define r: r calculate number of trailing 0s
```
- $r$ è la funzione che calcola il numero di trailing 0s di $z_i$
### Implementazione: Trailing 0s
La seguente implementazione in c va a calcolare il numero di trailing 0s di un dato numero in input.
La funzione accetta solo numeri interi non negativi
```c
int trailing_0s(int a_i) {

    int zeros = 0;
    while ((a_i & 1) == 0) {    // finchè il bit meno significativo è 0
        zeros++;    // counter trailing_0s
        a_i = a_i >> 1; // applico l'operazione bit a bit di spostamento a destra di 1 posizione
    }
    return zeros;
}
```

### Implementazione: z hash
La generazione dei numeri pseudo-casuali è la seguente
```c
srand(3454256);  // seed
a = rand() % 100;  // [0;99]
b = rand() % 100;  // [0;99]
```
Invece la funzione hash $z_i = a * a_i +b$ è stata implementata come segue
```c
int z_hash(int a, int x, int b) {
    return a*x + b;
}
```
