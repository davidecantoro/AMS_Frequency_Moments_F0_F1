#!/bin/bash

DATA=$(date +%Y%m%d)
mkdir -p "rng_$DATA"

# Array delle distribuzioni
distribuzioni=("uniforme" "esponenziale" "poisson")
#distribuzioni=("poisson")

# Array dei parametri lambda
lambdas=(1.0 0.1 5.0 10.0)

# Array dei limiti per la distribuzione uniforme
limiti_inferiori=(0.0 0.5 1.0)
limiti_superiori=(1.2 1.5 2.0)

# Lunghezza dello stream di numeri
lunghezza=200

# Percorso all'eseguibile
programma="./generate_stream"

# Prova tutte le combinazioni
for distribuzione in "${distribuzioni[@]}"; do
    if [ "$distribuzione" = "uniforme" ]; then
        for a in "${limiti_inferiori[@]}"; do
            for b in "${limiti_superiori[@]}"; do
                echo "Distribuzione: $distribuzione, Min: $a, Max: $b"
                file="stream_${distribuzione}_${a}_${b}"
                $programma -d $distribuzione -a $a -b $b -n $lunghezza -f $file
                mv "${file}.csv" "rng_$DATA/${file}.csv"
            done
        done
    elif [ "$distribuzione" = "poisson" ]; then
        for lambda in "${lambdas[@]}"; do
            echo "Distribuzione: $distribuzione, Lambda: $lambda"
            file="stream_${distribuzione}_${lambda}"
            $programma -d $distribuzione -l $lambda -n $lunghezza -f $file
            mv "${file}.csv" "rng_$DATA/${file}.csv"
        done
    elif [ "$distribuzione" = "esponenziale" ]; then
        for lambda in "${lambdas[@]}"; do
            echo "Distribuzione: $distribuzione, Lambda: $lambda"
            file="stream_${distribuzione}_${lambda}"
            $programma -d $distribuzione -l $lambda -n $lunghezza -f $file -x 3
            mv "${file}.csv" "rng_$DATA/${file}.csv"
        done
    fi
done
