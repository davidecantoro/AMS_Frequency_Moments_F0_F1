# AMS Frequency Moments
Il seguente documento è tratto da un analisi del paper di Alon, Matias, e Szegedy, "The Space Complexity of Approximating the Frequency Moments" [1].

Questo documento si propone di definire uno pseudocodice per l'algoritmo AMS Frequency Moments, in modo da definire un implementazione in C/C++.

L'attenzione è stata concentrata sui momenti di ordine $k$ pari a 0 e 1.

## Introduzione
Sia $A = (a_1, a_2, ..., a_m)$ una sequenza di m elementi, con $a_i ∈ N=\{1,2,...,n\}$.

Denotiamo con $m_i$ le occorrenze i-esime nella sequenza A.

Il momento di ordine k è dato da $ F_k = \sum_{i=1}^{n} m_i^k $, dove $m_i$ è il numero di occorrenze di $i$ nella sequenza.

$F_k$ è definito come la somma k-esime potenze dei conteggi $m_i$.

## Estimating $F_k$
Estraiamo un numero casuale $a_p$ della sequenza, dove l'indice $p$ è stato scelto in modo casuale e uniforme tra gli indici $1, ...,m$.

Sia $r=|\{q:q \ge p, a_q =  a_p\}|$ il conteggio di $a_p$ in $A$, da un fissato $p$ in poi.

Definiamo la variabile $X=m(r^k - (r-1)^k)$.

Nel caso in cui $m$ è sconosciuta: quando $a_m$ (elemento m-esimo dello stram) arriva, lo rimpiazziamo ad $a_p$ con probabilità $1/m$, in questo caso $r$ viene impostato a 1, se non c'è rimpiazzo $r$ viene incrementato se $a_m = a_p$.

Come dimostrato da Alon et al. [1], si ha che $E(X) = ∑_{i=1}^n m_i^k = F_k$ e $Var(X) = E(X^2) - (E(X))^2$ dove $E(X^2) ≤ kF_1F_{2k-1} $ .

### Pseudocode
Procedimento per una singola variabile X.


Case: m known
```
AMS_Frequency_Moment_m_known(A, k, m): # A stream, k moment order, m length of the stream
  initialize...
  p <- rand uniform (1,m)
  r <- 1
  q <- p + 1

  # procedure...
  while(steam, starting from q)
    if(a_q == a_p)
      r <- r +1  
    q <- q + 1
  end while
  compute X = m(r^k - (r-1)^k)
  return X

```

Case: m unknown
```
AMS_Frequency_Moment(A, k): # A stream, k moment order
  # initialize...
  r <- 1
  a_p <- A[0]
  m <- 1

  # procedure...
  while stream do:
      pick random number in U(0,1)
      if random number < 1 / m: # with prob. 1/m accept replacement
          a_p <- A[m]    
          r <- 1
      else if A[m] == a_p:  # increase r
          r <- r + 1
      end if
      m <- m + 1 # update m
  end while
  X <- m * (r^k - (r - 1)^k)  # compute X
  return X

```




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
### Implementazione
Lo stream utilizzato è stato generato dal programma "generate_stream" ed è disponibile nella directory "stream_generator".

#### Trailing 0s
La seguente implementazione in c calcola il numero di trailing 0s di un dato numero in input.
La funzione accetta solo numeri interi non negativi.
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

#### Hash function z
La funzione hash $z_i = a * a_i +b$ è stata implementata come segue
```c
srand(3454256);  // seed
a = rand() % 100;  // [0;99]
b = rand() % 100;  // [0;99]
```

```c
int z_hash(int a, int x, int b) {
    return a*x + b;
}
```


## Stream generator
Per generare lo stream utilizzato in questo lavoro è stato implementato in C++ un generatore di numeri pseudo-casuali. Questo generatore, disponibile nella directory "stream_generator", si occupa inoltre del salvataggio dello stream in formato CSV, con possibilità di salvare l'output anche altre estensioni.

Nella stessa directory sono presenti due script di utility scritti in bash: "test_generatore.sh" e "controllo_input.sh". Questi script hanno lo scopo di testare le combinazioni di input disponibili, verificando sia il corretto funzionamento delle opzioni disponibili che il corretto funzionamento dei meccanismi di controllo dell'input inserito da utente.

Un Makefile è disponibile per semplificare la compilazione del programma.

Le distribuzioni implementate per generare numeri pseudo-casualmente sono:
- uniforme
- esponenziale
- poisson

### Usage
Il programma dispone di un opzione di usage, richiamabile tramite l'opzione -h, che stampa il seguente messaggio.
```
Utilizzo: ./generate_stream [-d distribuzione] [-l lambda] [-a min] [-b max] [-n lunghezza] [-f file]
Il seguente programma genera uno stream di numeri pseudo-casuali, salvando il risultato in un file in formato CSV.
ATTENZIONE: Il seguente programma fornisce in output un file CSV di numeri interi, quindi per conservare le cifre decimali bisogna utilizzare l'opzione x
Le opzioni disponibili sono le seguenti:
  -h                Messaggio di aiuto
  -d distribuzione  Permette di specificare una distribuzione da usare: uniforme, esponenziale, poisson. Default = uniforme
  -l lambda         Permette di specificare il parametro lambda usato per le distribuzioni esponenziale e Poisson. Default = 10
  -a min            Permette di specificare il limite inferiore per la distribuzione uniforme. Default = 0
  -b max            Permette di specificare il limite superiore per la distribuzione uniforme. Default = 100
  -n lunghezza      Permette di specificare la lunghezza dello stream. Default = 200
  -x cifre          Permette di specificare il numero di cifre decimali da mantenere. Default = 0
  -f file           Permette di specificare il nome del file CSV fino ad un massimo di 49 caratteri. Default = stream
  -e estensione     Permette di specificare l'estensione del file fino ad un massimo di 4 caratteri. Default = CSV
  NOTA - caratteri non accettati: spazi, stringhe vuote, stringhe con solo spazi, caratteri speciali diversi da virgola, trattino e punto
```

### Implementazione
La libreria getopt è stata utilizzata in modo da permettere l'utilizzo delle opzioni.

La funzione err_sys è stata implementata per gestire gli errori. Tale funzione ha lo scopo di mostrare a schermo un messaggio di errore descrittivo e di terminare l'esecuzione del programma.

Le funzioni "uniforme" e "esponenziale" sono state adattate per generare numeri interi. Il numero generato viene moltiplicato per $10^x$, dove $x$ è uno dei parametri modificabili, e successivamente troncato.

Sono stati eseguiti i seguenti controlli sull'input inserito da utente:
- dopo i numeri (a, b, lambda, x ed n) non ci siano lettere o caratteri simili
- la lunghezza dello stream e x siano un numeri interi e positivi
- a,b e lambda siano numeri decimali positivi
-  i numeri decimali e interi (a, b, lambda, x e n) siano inferiori al valore massimo possibile.
- b sia maggiore di a
- l'input relativo alla distribuzione è troncato per accettare massimo 12 caratteri
- il nome del file sia più corto di 50 caratteri e che non contenga spazi, stringhe vuote, stringhe con solo spazi e caratteri speciali diversi da virgola, trattino e punto
- il nome dell'estensione sia più corto di 5 caratteri

Implementazione della generazione dello strem:
```c
// ----------------------- PSEUDO NUMBER GENERATOR -----------------------
int uniforme(std::default_random_engine& generator, double a, double b, int x) {
    std::uniform_real_distribution<double> distribuzione(a, b);
    double numero = distribuzione(generator);
    return static_cast<int>(numero * std::pow(10, x));
}

int esponenziale(std::default_random_engine& generator, double lambda, int x) {
    std::exponential_distribution<double> distribuzione(lambda);
    double numero = distribuzione(generator);
    return static_cast<int>(numero * std::pow(10, x));
}


double poisson(std::default_random_engine& generator, double lambda) {
    std::poisson_distribution<int> distribuzione(lambda);
    return distribuzione(generator);
}
// -----------------------------------------------------------------------
```

```c
    // --- GENERAZIONE STREAM ---
    for (int i = 0; i < n; i++) {       // nota: fare switch-for oppure for-switch è computazionalmente ininfluente
        switch (distribuzione) {
            case UNIFORME:
                file << uniforme(generator, a, b, x) << "\n";      // genero numero e salvo su file
                break;
            case ESPONENZIALE:
                file << esponenziale(generator, lambda, x) << "\n";
                break;
            case POISSON:
                file << poisson(generator, lambda) << "\n";
                break;
     
        }
    }
```



# Bibliografia
[1] Alon, N., Matias, Y., & Szegedy, M. (1999). The Space Complexity of Approximating the Frequency Moments. *Journal of Computer and System Sciences, 58*(1), 137-147. https://doi.org/10.1006/jcss.1997.1545


[2] P. Flajolet and G. N. Martin, "Probabilistic counting," 24th Annual Symposium on Foundations of Computer Science (sfcs 1983), Tucson, AZ, USA, 1983, pp. 76-82, doi: 10.1109/SFCS.1983.46.