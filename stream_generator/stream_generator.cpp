#include <iostream>
#include <random>       // pseudo number generator
#include <string>
#include <getopt.h>     // per le opzioni da linea di comando, se non funziona usare unistd.h
#include <fstream>      // lavorare con i file
#include <cfloat>       // FLT_MAX
#include <climits>      // INT_MAX 
#include <algorithm>    // stringa in minuscolo
#include <cstring>      // controllo sulla stringa

#define MAXLENGTH 50
#define MAXLENGTHDISTRIBUTION 13 // esponenziale + null

void err_sys(const char* x) { 
    perror(x); 
    exit(1); 
}

// enum delle possibili distribuzioni
enum Distribuzione { UNIFORME, ESPONENZIALE, POISSON };



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

int main(int argc, char *argv[]) {
    int opt;
    int x = 0;      // cifre decimali da mantenere, per uniforme ed esponenziale

    // Controllo input utente
    char buffer[MAXLENGTHDISTRIBUTION];
    char formato_input[20];
    char resto_input[MAXLENGTH] = "";
    float a_f = 0.0f;

    // --- PARAMETRI DI DEFAULT ---
    Distribuzione distribuzione = UNIFORME;  // distribuzione di default
    double lambda = 1.0;  // esponenziale e poisson
    double a = 0.0;  // inf default uniforme
    double b = 100.0;  // sup default uniforme
    int n = 200;  // lunghezza stream
    std::string filename = "stream.csv";  // nome file

    std::default_random_engine generator(3454256);  // seed

     /*
        Controlla l'input:
        - che dopo i numeri (a,b,lambda ed n) non ci siano lettere o simili
        - che la lunghezza stream sia intero e positivo
        - che a,b e lambda siano float e positivi
        - che float e int (a,b,lambda ed n) siano più piccoli del massimo valore possibile
    */
    // --- OPTIONS ---
    while ((opt = getopt(argc, argv, "hd:l:a:b:n:f:x:")) != -1) {
        switch (opt) {
            case 'd':

                // copio 12 caratteri + null
                strncpy(buffer, optarg, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                std::transform(buffer, buffer + strlen(buffer), buffer, ::tolower);     // tutto in minuscolo

                if (strcmp(buffer, "uniforme") == 0) {
                    distribuzione = UNIFORME;
                } else if (strcmp(buffer, "esponenziale") == 0) {
                    distribuzione = ESPONENZIALE;
                } else if (strcmp(buffer, "poisson") == 0) {
                    distribuzione = POISSON;
                }else {
                    std::cout << "Errore: distribuzione " << optarg << " non valida \n";
                    return 1;
                }
                break;
            case 'l':
                sprintf(formato_input, "%%f%%%ds", MAXLENGTH-1);
                if (sscanf(optarg, formato_input, &lambda) != 1 || lambda < 0.0f || lambda > FLT_MAX || resto_input[0] != '\0') {
                    errno = 1;      //: Operation not permitted
                    err_sys("Errore: Non hai inserito un numero float positivo\t");
                }
                break;

            case 'a':
                sprintf(formato_input, "%%f%%%ds", MAXLENGTH-1);
                if (sscanf(optarg, formato_input, &a) != 1 || a < 0.0f || a > FLT_MAX || resto_input[0] != '\0') {
                    errno = 1;      //: Operation not permitted
                    err_sys("Errore: Non hai inserito un numero float positivo per a\t");
                }
                break;
            case 'b':
                sprintf(formato_input, "%%f%%%ds", MAXLENGTH-1);
                if (sscanf(optarg, formato_input, &b) != 1 || b < 0.0f || b > FLT_MAX || resto_input[0] != '\0') {
                    errno = 1;      //: Operation not permitted
                    err_sys("Errore: Non hai inserito un numero float positivo per b\t");
                }
                break;

            case 'n':
                sprintf(formato_input, "%%d%%%ds", MAXLENGTH-1);
                if (sscanf(optarg, formato_input, &n) != 1 || n < 0 || n > INT_MAX || resto_input[0] != '\0') {
                    errno = 1;      //: Operation not permitted
                    err_sys("Errore: Non hai inserito un numero intero positivo per n\t");
                }
                break;
            case 'f':
                filename = optarg;
                break;

            case 'x':
                sprintf(formato_input, "%%d%%%ds", MAXLENGTH-1);
                if (sscanf(optarg, formato_input, &x) != 1 || x < 0 || x > INT_MAX || resto_input[0] != '\0') {
                    errno = 1;      //: Operation not permitted
                    err_sys("Errore: Non hai inserito un numero intero positivo per n\t");
                }
                break;


            case 'h':
                std::cout << "Utilizzo: ./random [-d distribuzione] [-l lambda] [-a min] [-b max] [-n lunghezza] [-f file]\n"
                          << "Il seguente programma genera uno stream di numeri pseudo-casuali, salvando il risultato in un file in formato CSV\n"
                          << "ATTENZIONE: Il seguente programma fornisce in output un file CSV di numeri interi, quindi per conservare le cifre decimali bisogna utilizzare l'opzione x\n"
                          << "Le opzioni disponibili sono le seguenti:\n"
                          << "  -h                Messaggio di aiuto\n"
                          << "  -d distribuzione  Permette di specificare una distribuzione da usare: uniforme, esponenziale, poisson. Se non specificato, verra\' utilizzata la distribuzione uniforme\n"
                          << "  -l lambda         Permette di specificare il parametro lambda usato per le distribuzioni esponenziale e Poisson\n"
                          << "  -a min            Permette di specificare il limite inferiore per la distribuzione uniforme\n"
                          << "  -b max            Permette di specificare il limite superiore per la distribuzione uniforme\n"
                          << "  -n lunghezza      Permette di specificare la lunghezza dello stream\n"
                          << "  -x cifre          Permette di specificare il numero di cifre decimali da mantenere\n"
                          << "  -f file           Permette di specificare il nome del file CSV\n";
                return 0;
            case '?':
                std::cout << "Opzione sconosciuta: " << (char)optopt << "\n";
                return 1;
        }
    }

    // apertura file
    std::ofstream file(filename);
    if (!file) {
        std::cout << "Impossibile aprire il file: " << filename << "\n";
        return 1;
    }



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



    // chiusura file
    file.close();
    return 0;
}
