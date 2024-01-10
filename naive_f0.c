#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAXLENGTH 20


int main() {
    FILE* file = fopen("stream_generator/stream.csv", "r");
    if (file == NULL) {
        printf("Errore: Impossibile aprire il file.\n");
        return 1;
    }

    int* distinct_items = NULL;
    int m_len = 0;
    int a_i;
    char separatore = ';';
    char formato_input[MAXLENGTH];
    sprintf(formato_input, "%%d%c", separatore);
    bool presente = false;  

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
    
    fclose(file);
    free(distinct_items);

    printf("Distinct item reali: %d\n", m_len);

    

    return 0;
}