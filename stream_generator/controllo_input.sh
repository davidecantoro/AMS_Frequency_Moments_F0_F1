#!/bin/bash

# Array delle distribuzioni
distribuzioni=("uniforme" "esponenziale" "poisson" "EspOnenzIale" "esponenzialedasdasdasdas")

# Array dei parametri lambda
lambdas=("1.0" "fsdfsd" "10.0das")

# Array dei limiti per la distribuzione uniforme
limiti_inferiori=("1.0" "fsdfsd" "10.0das")
limiti_superiori=("1.0" "fsdfsd" "10.0das")

# Lunghezza dello stream di numeri
lunghezza=200

# Percorso all'eseguibile
programma="./test4"

# Prova tutte le combinazioni
for distribuzione in "${distribuzioni[@]}"; do
    if [ "$distribuzione" = "uniforme" ]; then
        for a in "${limiti_inferiori[@]}"; do
            for b in "${limiti_superiori[@]}"; do
                echo "Distribuzione: $distribuzione, Min: $a, Max: $b"
                file="stream_${distribuzione}_${a}_${b}.csv"
                $programma -d $distribuzione -a $a -b $b -n $lunghezza -f $file
            done
        done
    #elif [ "$distribuzione" = "esponenziale" ] || [ "$distribuzione" = "poisson" ]; then
    else
        for lambda in "${lambdas[@]}"; do
            echo "Distribuzione: $distribuzione, Lambda: $lambda"
            file="stream_${distribuzione}_${lambda}.csv"
            $programma -d $distribuzione -l $lambda -n $lunghezza -f $file
        done
    fi
done
