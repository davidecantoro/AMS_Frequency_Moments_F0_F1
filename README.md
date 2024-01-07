# AMS Frequency Moments
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
