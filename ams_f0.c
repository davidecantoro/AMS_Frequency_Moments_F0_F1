#include <stdio.h>
#include <limits.h>     // più piccolo e  più grande intero rappresentabile
#include <stdlib.h>     // err_sys
#include <errno.h>      // err_sys
//#include <math.h>
#include <time.h>       // calcolo del tempo di esecuzione

#define MAXLENGTH 20

void err_sys(const char* x) { 
    perror(x); 
    exit(1); 
}

/*
    trailing_0s accetta solo numeri positivi    
*/
int trailing_0s(int a_i) {

    int zeros = 0;
    while ((a_i & 1) == 0) {    // finchè il bit meno significativo è 0
        zeros++;    // counter trailing_0s
        a_i = a_i >> 1; // applico l'operazione bit a bit di spostamento a destra di 1 posizione
    }
    return zeros;
}

/*
    z_hash accetta solo numeri positivi    
*/
int z_hash(int a, int x, int b) {
    return a*x + b;
}

int max(int a, int b) {
    if(a > b)
        return a;
    else
        return b;
}

/*
        L'INPUT DEVE ESSERE FORNITO NEL SEGUENTE METODO:
        - SU UNA RIGA CI DEVE ESSERE UN SOLO NUMERO
        - SONO ACCETTATI I SEGUENTI SEPARATORI , ;
*/
int main() {

    //controllo input utente
    //char buffer[MAXLENGTH];
    char formato_input[MAXLENGTH];
    //char resto_input[MAXLENGTH] = "";

    int a_i;
    int r_i = 0;
    int a, b;  
    int z_i;
    int R = INT_MIN;    // R =  - inf
    int distinct_item_estimate;

    // calcolo del tempo
    clock_t t_0, t_f;
    double delta_t = 0.0;
    
    unsigned int seed = 3454256;


    srand(seed); 
    a = rand() % 10;  // [0;9]
    b = rand() % 10;  // [0;9]

    //printf("Inserisci un numero intero positivo: ");
    //fgets(buffer, MAXLENGTH, stdin);
    FILE *file = fopen("stream_generator/stream.csv", "r");
    if (file == NULL) {
        printf("Errore: Impossibile aprire il file.\n");
        return 1;
    }
    


    char separatore = ';';
    sprintf(formato_input, "%%d%c", separatore);
    
    t_0 = clock();
    while (fscanf(file, formato_input, &a_i) == 1){

        


        if (a_i >= 0 && a_i <= INT_MAX) {
            z_i = z_hash(a, a_i, b);
            //printf("a_i = %d\tz = %d \n", a_i, z_i);
            
            
            r_i = trailing_0s(z_i);
            //printf("Numero di 0s: %d\n", r_i);

            R = max(R,r_i);
            //printf("R: %d", R);


        } else {
            printf("Errore: Letto valore sconosciuto, il valore letto verrà scartato");
        }

        

        
    }
    
    t_f = clock();
    delta_t = ((double) (t_f - t_0) / CLOCKS_PER_SEC);

    fclose(file);

    //double distinct_item_estimate = pow(2, R);
    /*
        Visto che R è un intero positivo e visto il nunmero distinct_item_estimate è una potenza di due posso 
        usare lo shift a sinistra. Il vantaggio di usare lo shift invece che la funzione pow consiste nel 
        evitare di utilizzare una libreria in più.
        
        2^8 2^7 2^6 2^5 2^4 2^3 2^2 2^1 2^0
        Shift a sinistra: parto da 2^0 = 1 e shifto di R posizioni

    */
    distinct_item_estimate = 1 << R;
    printf("AMS Frequency Moments - momento di ordine 0 \n");
    printf("Distinct item stimati: %d\n",distinct_item_estimate);
    printf("Tempo di esecuzione: %f [s]\n",delta_t);
    

    /*
            DA FARE
            - aggiungere optarg per controllare il separatore
            - aggiungere opzione per nome file, estensione, location
            - salvare risultato su file -> aggiungere tutto anche su optarg
            - commenti e pulizia codice
            - modificare README.md
    */
    
    return 0;
}
