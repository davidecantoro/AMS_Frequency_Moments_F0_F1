#include <stdio.h>
#include <limits.h>     // più piccolo e  più grande intero rappresentabile
#include <stdlib.h>     // err_sys
#include <errno.h>      // err_sys
//#include <math.h>
#include <time.h>       // calcolo del tempo di esecuzione

#include <stdbool.h>
#include <unistd.h>     // per optarg
#include <string.h>
#include <errno.h>
#include <regex.h>      // libreria per implementare le regex

#define MAXLENGTHSTREAM 20
#define MAXLENGTH 50

void err_sys(const char* x) { 
    errno = 1;      //: Operation not permitted
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
int main(int argc, char *argv[]) {

    //controllo input utente
    //char buffer[MAXLENGTH];
    char formato_input[MAXLENGTHSTREAM];
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

    // parametri
    char separatore = ';';
    bool quiet = false;
    char filename[MAXLENGTH] = "stream.csv";
    char path[MAXLENGTH] = "stream_generator/";
    char fullpath[MAXLENGTH] = "";
    int opt;

    // --- IMPLEMENTAZIONE ESPRESSIONI REGOLARI PER CONTROLLO DELL'INPUT ---
    char *regex_filename = "^[a-zA-Z0-9_.-\\ ]+$";        // per il filename
    char *regex_path = "^[a-zA-Z0-9_.-/\\ ]+$";           // per il percorso
    regex_t regex_function_filename, regex_function_path;   // prima del controllo vanno compilate
    


    srand(seed); 
    a = rand() % 10;  // [0;9]
    b = rand() % 10;  // [0;9]



    // ---  OPZIONI ---
    while ((opt = getopt(argc, argv, "f:p:s:qh")) != -1) {
            switch (opt) {
                case 'f':   // file name

                    strncpy(filename, optarg, MAXLENGTH - 1);
                    filename[MAXLENGTH - 1] = '\0';



                    // implementazione espressioni regolari
                    // compilazione espressione
                    int result_compilazione_f = regcomp(&regex_function_filename, regex_filename, REG_EXTENDED);
                    

                    if (result_compilazione_f) {    // compilazione regex filename

                        char error_mex[20],error_mex_output[100];
                        regerror(result_compilazione_f, &regex_function_filename, error_mex, sizeof(error_mex));
                         sprintf(error_mex_output, "Errore durante la compilazione della regex per il filename: %s\t", error_mex);


                        regfree(&regex_function_filename);  // libero memoria filename regex
                        err_sys(error_mex_output);
                    }

                    // controllo espressione
                    int result_controllo_regex_f = regexec(&regex_function_filename, optarg, 0, NULL, 0);
                    if (result_controllo_regex_f){  // controllo regex

     
                        char error_mex[20],error_mex_output[100];
                        regerror(result_controllo_regex_f, &regex_function_filename, error_mex, sizeof(error_mex));

                        sprintf(error_mex_output, "Errore durante il controllo della regex per il filename: %s\t", error_mex);

                        regfree(&regex_function_filename);  // libero memoria filename regex
                        err_sys(error_mex_output);

                    }
                    
                    regfree(&regex_function_filename);  // libero memoria filename regex
                    
                    break;
                case 'p':   // path
                    strncpy(path, optarg, MAXLENGTH - 1);
                    path[MAXLENGTH - 1] = '\0';



                    // implementazione espressioni regolari
                    // compilazione espressione
                    int result_compilazione_p = regcomp(&regex_function_path, regex_filename, REG_EXTENDED);
                    

                    if (result_compilazione_p) {    // compilazione regex filename

                        char error_mex[20], error_mex_output[100];
                        regerror(result_compilazione_p, &regex_function_path, error_mex, sizeof(error_mex));
                        sprintf(error_mex_output, "Errore nella compilazione dell'espressione regolare: %s\t", error_mex);


                        regfree(&regex_function_path);  // libero memoria filename regex
                        err_sys(error_mex_output);
                    }

                    // controllo espressione
                    int result_controllo_regex_p = regexec(&regex_function_path, optarg, 0, NULL, 0);
                    if (result_controllo_regex_p){  // controllo regex

     
                        char error_mex[20], error_mex_output[100];
                        regerror(result_controllo_regex_p, &regex_function_path, error_mex, sizeof(error_mex));
                        sprintf(error_mex_output, "Errore durante il controllo della regex per il path: %s\t", error_mex);



                        regfree(&regex_function_path);  // libero memoria filename regex
                        err_sys(error_mex_output);

                    }
                    
                    regfree(&regex_function_path);  // libero memoria filename regex


                    break;
                case 's':   // separatore
                    separatore = optarg[0];
                    break;
                case 'q':   // quiet, se attivato non stampa l'output
                    quiet = true;
                    break;
                case 'h':
                    printf("Utilizzo: ./naive_f0 [-f nome_file] [-p path] [-s separatore] [-q] [-h]\n"
                            "Il seguente programma utilizza un metodono naive per calcolare il numero esatto di F0.\n"
                            "Le opzioni disponibili sono le seguenti:\n"
                            "  -h                   Messaggio di aiuto\n"
                            "  -f nome_dile         Permette di specificare il nome del file da utilizzare per il calcolo di F0.\n"
                            "  -p path              Permette di specificare il percorso del file da utilizzare per il calcolo di F0.\n"
                            "  -s separatore        Permette di specificare il carattere di separazione degli elementi utilizzati nel file di input.\n"
                            "  -q                   L'opzione quiet permette di sopprimere l'output a schermo.\n");

                    return 0;
                default:
                    err_sys("Errore: Opzione inserita sconosciuta\t");
            }
        }
    
    snprintf(formato_input, MAXLENGTHSTREAM, "%%d%c", separatore);






    //printf("Inserisci un numero intero positivo: ");
    //fgets(buffer, MAXLENGTH, stdin);
    //FILE *file = fopen("stream_generator/stream.csv", "r");
    //if (file == NULL) {
    //    printf("Errore: Impossibile aprire il file.\n");
    //    return 1;
    //}

    if (strlen(path) + strlen(filename) < 2*MAXLENGTH) {
        snprintf(fullpath, 2*MAXLENGTH, "%s%s", path, filename);
    } else {
        err_sys("Errore: Il percorso completo del file è troppo lungo");
    }


    FILE* file = fopen(fullpath, "r");
    if (file == NULL) {
        printf("Errore: Impossibile aprire il file.\t");
        return 1;
    }
    



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
            - salvare risultato su file -> aggiungere tutto anche su optarg
            - commenti e pulizia codice
            - modificare README.md
    */
    
    return 0;
}
