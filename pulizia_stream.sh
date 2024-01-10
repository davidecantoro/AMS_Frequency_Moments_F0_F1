#!/bin/bash

#############################################################################################################################
#       Uso: ./pulizia_stream.sh [file_input] [file_output]                                                                 #
#       Se non vengono passati argomenti, verranno utilizzati dei valori di default per file_input e file_output            #
#       Se viene passato un solo argomento, verrà usato lo stesso argomento sia come file_input che come file_output        #
#       Se vengono passati due argomenti, verranno utilizzati come file_input e file_output rispettivamente                 #
#       Viene inoltre effettuato un controllo sul numero di argomenti, il numero degli argomenti deve essere <= 2           #
#############################################################################################################################



separatore=";"
file_input="stream_generator/stream.csv"
file_output="stream.csv"

# controllo argomenti
if [ "$#" -eq 1 ]; then     # 1 argomento => usa lo stesso argomento sia per input che per output
    file_input=$1
    file_output=$1

elif [ "$#" -eq 2 ]; then   # 2 argomenti => input e output
    file_input=$1
    file_output=$2

elif [ "$#" -gt 2 ]; then   # argomenti > 2 => mesaggio di errore e uscita dallo script
    echo "Errore: Hai inserito troppi argomenti"
    exit 1
fi

echo "Inizio pulizia del file $file_input"

tr -cd "0-9\n$separatore" < "$file_input" > "$file_output"

echo "Pulizia del file terminata, file in ouput: $file_output"
