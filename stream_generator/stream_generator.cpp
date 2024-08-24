#include <iostream>
#include <random>       // pseudo number generator
#include <string>
#include <getopt.h>     // per le opzioni da linea di comando, se non funziona usare unistd.h
#include <fstream>      // lavorare con i file
#include <cfloat>       // FLT_MAX
#include <climits>      // INT_MAX 
#include <algorithm>    // stringa in minuscolo
#include <cstring>      // controllo sulla stringa
#include <regex>        // regex per controllare l'input da utente

#define MAXLENGTH 50
#define MAXLENGTHINPUT 20
#define MAXLENGTHDISTRIBUTION 13 // esponenziale + null
#define MAXLENGTHEXTENSION 5

void err_sys(const char* x) {   // gestione del messaggio di errore con uscita forzata
    errno = 1;      //: Operation not permitted
    perror(x); 
    exit(1); 
}

// enum delle possibili distribuzioni
enum Distribuzione { UNIFORME, ESPONENZIALE, POISSON };



// ----------------------- PSEUDO NUMBER GENERATOR -----------------------

int uniforme(std::default_random_engine& generator, double a, double b, int x) {
    std::uniform_real_distribution<double> distribuzione(a, b);
    double numero = distribuzione(generator);
    //return static_cast<int>(numero * std::pow(10, x));
    int risultato = static_cast<int>(std::round(numero * std::pow(10, x)));
    return risultato;
}


int esponenziale(std::default_random_engine& generator, double lambda, int x) {
    std::exponential_distribution<double> distribuzione(lambda);
    double numero = distribuzione(generator);
    return static_cast<int>(numero * std::pow(10, x));
}


double poisson(std::default_random_engine& generator, double lambda) {
    std::poisson_distribution<int> distribuzione(lambda);
    int risultato = distribuzione(generator);
    return risultato;
}   
// -----------------------------------------------------------------------

int main(int argc, char *argv[]) {
    int opt;
    int x = 0;      // cifre decimali da mantenere, per uniforme ed esponenziale

    // Controllo input utente
    char buffer[MAXLENGTHDISTRIBUTION];
    char buffer_filename[MAXLENGTH];
    char formato_input[MAXLENGTHINPUT];
    char resto_input[MAXLENGTH] = "";
    float a_f = 0.0f;
    std::regex filename_regex("^[\\w._,-]+$");
    char buffer_b[MAXLENGTH];
    char buffer_a[MAXLENGTH];
    char buffer_l[MAXLENGTH];

    // --- PARAMETRI DI DEFAULT ---
    Distribuzione distribuzione = UNIFORME;
    double lambda = 10.0;  // esponenziale e poisson
    double a = 0.0;  // uniforme
    double b = 100.0;  // uniforme
    int n = 1000;  // lunghezza stream
    std::string filename = "stream";  // nome 
    char file_extension[MAXLENGTHEXTENSION] = "csv";    // formato

    std::default_random_engine generator(3454256);  // seed

    // controlli input utente sulla docs
    // --- OPTIONS ---
    while ((opt = getopt(argc, argv, "hd:l:a:b:n:f:x:e:")) != -1) {
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
                    err_sys("Errore: La distribuzione inserita non è valida\t");
                }
                break;
            case 'l':

                strncpy(buffer_l, optarg, sizeof(buffer_l) - 1);
                buffer_l[sizeof(buffer_l) - 1] = '\0'; 

                snprintf(formato_input, MAXLENGTHINPUT, "%%f%%s");

                if (sscanf(buffer_l, formato_input, &b, resto_input) != 1 || lambda < 0.0f || lambda > FLT_MAX || resto_input[0] != '\0') {
                    err_sys("Errore: Non hai inserito un numero float positivo per lambda\t");
                }
                sscanf(buffer_l, "%lf", &lambda);
 
                break;

            case 'a':

                char buffer_a[MAXLENGTH];
                strncpy(buffer_a, optarg, sizeof(buffer_a) - 1);
                buffer_a[sizeof(buffer_a) - 1] = '\0'; 

                snprintf(formato_input, MAXLENGTHINPUT, "%%f%%s");

                if (sscanf(buffer_a, formato_input, &a, resto_input) != 1 || a < 0.0f || a > FLT_MAX || resto_input[0] != '\0') {
                    err_sys("Errore: Non hai inserito un numero float positivo per a\t");
                }
                sscanf(buffer_a, "%lf", &a);

                break;
            case 'b':


                strncpy(buffer_b, optarg, sizeof(buffer_b) - 1);
                buffer_b[sizeof(buffer_b) - 1] = '\0'; 


                //sprintf(formato_input, "%%f%%%ds", MAXLENGTH-1);
                snprintf(formato_input, MAXLENGTHINPUT, "%%f%%s");

                if (sscanf(buffer_b, formato_input, &b, resto_input) != 1 || b <= a || b < 0.0f || b > FLT_MAX || resto_input[0] != '\0') {
                    err_sys("Errore: Non hai inserito un numero float positivo per b oppure b non valido\t");
                }
                sscanf(buffer_b, "%lf", &b);

                break;
                

            case 'n':
                snprintf(formato_input, MAXLENGTHINPUT, "%%d%%%ds", MAXLENGTH-1);

                if (sscanf(optarg, formato_input, &n, resto_input) != 1 || n < 0 || n > INT_MAX || resto_input[0] != '\0') {
                    err_sys("Errore: Non hai inserito un numero intero positivo per n\t");
                }
                break;
            case 'f':
                if (std::regex_match(optarg, filename_regex) && (strlen(optarg) < MAXLENGTH) ) {
                    strncpy(buffer_filename, optarg, MAXLENGTH-1);
                    buffer_filename[MAXLENGTH-1] = '\0';
                    filename = buffer_filename;
                } else {
                    err_sys("Errore: Nome del file non valido o troppo lungo\t");
                }
                break;

            case 'x':
                snprintf(formato_input, MAXLENGTHINPUT, "%%d%%%ds", MAXLENGTH-1);
                if (sscanf(optarg, formato_input, &x, resto_input) != 1 || x < 0 || x > INT_MAX || resto_input[0] != '\0') {
                    err_sys("Errore: Non hai inserito un numero intero positivo per n\t");
                }


                break;
            case 'e':
            if (strlen(optarg) < MAXLENGTHEXTENSION) {
                strncpy(file_extension, optarg, MAXLENGTHEXTENSION-1);
                file_extension[MAXLENGTHEXTENSION-1] = '\0';
            } else {
                err_sys("Errore: Estensione del file troppo lunga\t");

            }
            break;


            case 'h':
                std::cout << "Utilizzo: ./generate_stream [-d distribuzione] [-l lambda] [-a min] [-b max] [-n lunghezza] [-h] [-x cifre] [-f file] [-e estensione]\n"
                          << "Il seguente programma genera uno stream di numeri pseudo-casuali, salvando il risultato in un file in formato CSV.\n"
                          << "ATTENZIONE: Il seguente programma fornisce in output un file CSV di numeri interi, quindi per conservare le cifre decimali bisogna utilizzare l'opzione x\n"
                          << "Le opzioni disponibili sono le seguenti:\n"
                          << "  -h                Messaggio di aiuto\n"
                          << "  -d distribuzione  Permette di specificare una distribuzione da usare: uniforme, esponenziale, poisson. Default = uniforme\n"
                          << "  -l lambda         Permette di specificare il parametro lambda usato per le distribuzioni esponenziale e Poisson. Default = 10\n"
                          << "  -a min            Permette di specificare il limite inferiore per la distribuzione uniforme. Default = 0\n"
                          << "  -b max            Permette di specificare il limite superiore per la distribuzione uniforme. Default = 100\n"
                          << "  -n lunghezza      Permette di specificare la lunghezza dello stream. Default = 200\n"
                          << "  -x cifre          Permette di specificare il numero di cifre decimali da mantenere. Default = 0\n"
                          << "  -f file           Permette di specificare il nome del file CSV fino ad un massimo di 49 caratteri. Default = stream\n"
                          << "  -e estensione     Permette di specificare l'estensione del file fino ad un massimo di 4 caratteri. Default = CSV\n"
                          << "  NOTA - caratteri non accettati: spazi, stringhe vuote, stringhe con solo spazi, caratteri speciali diversi da virgola, trattino e punto\n";
                return 0;
            case '?':
                err_sys("Errore: Opzione inserita sconosciuta\t");

        }
    }

    // apertura file
    std::ofstream file(filename + "." + file_extension);
    if (!file) {
        err_sys("Errore: Impossibile aprire il file\t");
    }


    // --- GENERAZIONE STREAM ---
    for (int i = 0; i < n; i++) {       // nota: fare switch-for oppure for-switch è computazionalmente ininfluente
        switch (distribuzione) {
            case UNIFORME:
                file << uniforme(generator, a, b, x) << ";\n";      // genero numero e salvo su file
                break;
            case ESPONENZIALE:
                file << esponenziale(generator, lambda, x) << ";\n";
                break;
            case POISSON:
                file << poisson(generator, lambda) << ";\n";
                break;
     
        }
    }


    // chiusura file
    file.close();
    return 0;
}
