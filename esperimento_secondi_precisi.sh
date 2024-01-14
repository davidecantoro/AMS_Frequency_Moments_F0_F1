#!/bin/bash
#simulazioni/simulazione_20240113/output_algoritmi/
# ams_f0_uniforme_0.0_500.0_10000.csv

simulazioni_path="simulazioni/simulazione_20240114"
mkdir -p "$simulazioni_path/output_algoritmi"
output_path="$simulazioni_path/output_algoritmi/"

file_name_dataset="dataset_fileinput.csv"

algoritmi=("naive_f0" "ams_f0" "naive_f1" "ams_f1")

start_time=$SECONDS

tail -n +2 "$simulazioni_path/$file_name_dataset" | while IFS=',' read -r file_name_input rest
do
  for algoritmo in ${algoritmi[@]}; do
    echo "Esecuzione $algoritmo per il file $file_name_input"
    #output=$(/usr/bin/time -v ./ams_f0 -f uniforme_0.0_1000.0_100000000.csv -p simulazioni/simulazione_20240113/ -o ams_test_time.csv 2>&1)
    output=$(/usr/bin/time -v ./$algoritmo -f $file_name_input -p $simulazioni_path/ -o ${algoritmo}_$file_name_input -d $output_path -q 2>&1)
    output_time=$(time ./$algoritmo -f $file_name_input -p $simulazioni_path/ -o ${algoritmo}_$file_name_input -d $output_path -q 2>&1)
    
    user_time=$(echo "$output" | grep 'User time' | awk -F': ' '{print $2}' ) 
    user_time_unit=$(echo "$output" | grep 'User time' | awk -F'\\(' '{print $2}'| awk -F'\\)' '{print $1}')
    system_time=$(echo "$output" | grep 'System time' | awk -F': ' '{print $2}' ) 
    system_time_unit=$(echo "$output" | grep 'System time' | awk -F'\\(' '{print $2}'| awk -F'\\)' '{print $1}')
    cpu_percent=$(echo "$output" | grep 'Percent of CPU' | awk -F': ' '{print $2}')
    max_resident=$(echo "$output" | grep 'Maximum resident' | awk -F': ' '{print $2}') 
    max_resident_unit=$(echo "$output" | grep 'Maximum resident' | awk -F'\\(' '{print $2}'| awk -F'\\)' '{print $1}')

    temp_file=$(mktemp)
    echo "algoritmo,stima,esecuzione,user_time,user_time_unit,sys_time,sys_time_unit,cpu_percent,max_RSS,max_RSS_unit" > $temp_file
    echo "$(tail -n +2 ${output_path}${algoritmo}_${file_name_input}),${user_time},${user_time_unit},${system_time},${system_time_unit},${cpu_percent},${max_resident},${max_resident_unit}" >> $temp_file
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
