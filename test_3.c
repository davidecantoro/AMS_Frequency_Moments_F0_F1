#include <stdio.h>
#include <limits.h>     // più piccolo intero rappresentabile
#include <stdlib.h>     // err_sys
#include <errno.h>      // err_sys

#define MAXLENGTH 50

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

int main() {

    //controllo input utente
    char buffer[MAXLENGTH];
    char formato_input[20];
    char resto_input[MAXLENGTH] = "";

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
        printf("eRRORE: Impossibile aprire il file.\n");
        return 1;
    }
    

    sprintf(formato_input, "%%d%%%ds", MAXLENGTH-1);


    while (fscanf(file, "%d,", &a_i) == 1){

        if (sscanf(buffer, formato_input, &a_i) != 1 || a_i < 0 || resto_input[0] != '\0') {
            errno = 1;      //: Operation not permitted
            err_sys("Errore: Non hai inserito un numeri intero positivo\t");
        }

        z_i = z_hash(a, a_i, b);
        printf("z = %d \n", z_i);
        
        
        r_i = trailing_0s(z_i);
        printf("Numero di 0s: %d\n", r_i);

        R = max(R,r_i);
        printf("R: %d", R);
    }
    return 0;
}
