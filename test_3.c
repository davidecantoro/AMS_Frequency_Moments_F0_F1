#include <stdio.h>
#include <limits.h>     // più piccolo e  più grande intero rappresentabile
#include <stdlib.h>     // err_sys
#include <errno.h>      // err_sys
#include <math.h>

//#define MAXLENGTH 50

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
    char formato_input[20];
    //char resto_input[MAXLENGTH] = "";

    int a_i;
    int r_i = 0;
    int a, b;  
    int z_i;
    int R = INT_MIN;    // R =  - inf
    
    unsigned int seed = 3454256;


    srand(seed); 
    a = rand() % 100;  // [0;99]
    b = rand() % 100;  // [0;99]

    //printf("Inserisci un numero intero positivo: ");
    //fgets(buffer, MAXLENGTH, stdin);
    FILE *file = fopen("stream_generator/stream.csv", "r");
    if (file == NULL) {
        printf("Errore: Impossibile aprire il file.\n");
        return 1;
    }
    


    char separatore = ';';
    sprintf(formato_input, "%%d%c", separatore);
    

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

    double lenght_stream_estimate = pow(2, R);
    printf("Lunghezza stimata dello stream: %.0f\n",lenght_stream_estimate);

    /*
            DA FARE
            - vedere se si può elevare a potenza in modo più efficiente di pow
            - aggiungere optarg per controllare il separatore
            - aggiungere opzione per nome file, estensione, location
            - pulizia preliminare del input file, magari togliendo tutti i caratteri escluso numeri e separatore
            - migliorare print
            - implementare naiv F0 
            - salvare risultato su file -> aggiungere tutto anche su optarg
            - commenti e pulizia codice
            - modificare README.md
    */
    fclose(file);
    return 0;
}
