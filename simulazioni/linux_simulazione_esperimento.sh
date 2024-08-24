#!/bin/bash
#simulazioni/simulazione_20240113/output_algoritmi/
# ams_f0_uniforme_0.0_500.0_10000.csv

simulazioni_path="simulazioni/simulazione_20240114"
mkdir -p "$simulazioni_path/output_algoritmi"
output_path="$simulazioni_path/output_algoritmi/"

numero_esecuzioni=100


file_name_dataset="dataset_fileinput.csv"

algoritmi=("naive_f0" "ams_f0" "naive_f1" "ams_f1")

start_time=$SECONDS

tail -n +2 "$simulazioni_path/$file_name_dataset" | while IFS=',' read -r file_name_input rest
do
  for algoritmo in ${algoritmi[@]}; do
    echo "Esecuzione $algoritmo per il file $file_name_input"
    temp_file=$(mktemp)
    echo "momento,distribuzione,algoritmo,stima,esecuzione,cpu_percent,max_RSS,max_RSS_unit" > $temp_file

    for ((n=1; n<=numero_esecuzioni; n++)); do
      
      output=$(/usr/bin/time -v ./$algoritmo -f $file_name_input -p $simulazioni_path/ -o ${algoritmo}_$file_name_input -d $output_path -q 2>&1)
      
      cpu_percent=$(echo "$output" | grep 'Percent of CPU' | awk -F': ' '{print $2}' | tr -d '%')
      max_resident=$(echo "$output" | grep 'Maximum resident' | awk -F': ' '{print $2}') 
      max_resident_unit=$(echo "$output" | grep 'Maximum resident' | awk -F'\\(' '{print $2}'| awk -F'\\)' '{print $1}')

      #nome_algoritmo=$(echo $algoritmo | cut -d'_' -f 1)
      momento=$(echo $algoritmo | cut -d'_' -f 2)
      distribuzione=$(echo $file_name_input | cut -d'_' -f 1)

      echo "${momento},${distribuzione},$(tail -n +2 ${output_path}${algoritmo}_${file_name_input}),${cpu_percent},${max_resident},${max_resident_unit}" >> $temp_file
    done
    mv $temp_file ${output_path}${algoritmo}_${file_name_input}


    #./$algoritmo -f $file_name_input -p $simulazioni_path/ -o ${algoritmo}_$file_name_input -d $output_path -q
    #cd telegram_bot
    #./telegram_bot.sh "Esecuzione $algoritmo per $file_name_input terminata"
    #cd ..
  done
done


cd telegram_bot

end_time=$SECONDS
execution_time=$((end_time-start_time))

if command -v bc &> /dev/null   # se bc è installato
then
    execution_time_m=$(echo "scale=2; $execution_time / 60" | bc)
    echo "Tempo di esecuzione $execution_time_m minuti ( $execution_time secondi)"
    ./telegram_bot.sh "Simulazione esperimento terminata in $execution_time_m minuti ( $execution_time secondi)"
else
    echo "Tempo di esecuzione espresso in secondi: $execution_time"
    ./telegram_bot.sh "Simulazione esperimento terminata in $execution_time secondi"
fi
