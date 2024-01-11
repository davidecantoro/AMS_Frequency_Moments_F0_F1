#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>     // per optarg
#include <string.h>
#include <errno.h>
#include <regex.h>      // libreria per implementare le regex


#define MAXLENGTHSTREAM 20
#define MAXLENGTH 50

void err_sys(const char* x) {   // gestione del messaggio di errore con uscita forzata
    errno = 1;      //: Operation not permitted
    perror(x); 
    exit(1); 
}


int main(int argc, char *argv[]) {

    

    int* distinct_items = NULL;
    int m_len = 0;
    int a_i;
    char formato_input[MAXLENGTHSTREAM];
    bool presente = false;  
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
    

    // calcolo del tempo
    clock_t t_0, t_f;
    double delta_t = 0.0;

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

    
    if (strlen(path) + strlen(filename) < 2*MAXLENGTH) {
        snprintf(fullpath, 2*MAXLENGTH, "%s%s", path, filename);
    } else {
        err_sys("Errore: Il percorso completo del file è troppo lungo");
    }

    
    FILE* file = fopen(fullpath, "r");

    if (file == NULL) {
        err_sys("Errore: Impossibile aprire il file.\t");

    }


    t_0 = clock();

    /*
         F_k = \sum_{i=1}^{n} m_i^k
         Visto che k=0, si può usare come valore della somma degli m_i^k il valore len che indica la lunghezza del vettore contenente gli elementi distinti
    */
    while (fscanf(file, formato_input, &a_i) == 1) {

        if (a_i >= 0 && a_i <= INT_MAX) {

            // se l'elemento a_i è un nuovo elemento (non è presente in distinct_items) lo inserisco in distinct_items
            
            presente = false;
            for (int k = 0; k < m_len; k++) {

              
                // controllo presenza in distinct_items
                if (distinct_items[k] == a_i) {     
                    presente = true;
                    break;
                }
            }


            // inserimento in distinct_items
            if (!presente) {
                m_len++;
                distinct_items = realloc(distinct_items, m_len * sizeof(int));
                distinct_items[m_len - 1] = a_i;
            }

        } else {
            printf("Errore: Letto valore sconosciuto, il valore letto verrà scartato\n");
        }
    }

    t_f = clock();
    delta_t = ((double) (t_f - t_0) / CLOCKS_PER_SEC);
    
    fclose(file);
    free(distinct_items);

    if (!quiet) {
        printf("Naive Frequency Moments - momento di ordine 0 \n");    
        printf("Distinct item reali: %d\n", m_len);
        printf("Tempo di esecuzione: %f [s]\n",delta_t);
    }
    

    
    return 0;
}