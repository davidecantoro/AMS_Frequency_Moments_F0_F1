#!/bin/bash

###########################################################
# Generazione stream
#
#lunghezza_stream=(200 1000 5000 10000)
# - distribuzioni=("uniforme" "esponenziale" "poisson")
# - Per la distribuzione uniforme: a = 0.0, b = (100.0 per lunghezza_stream 200, 200.0 per lunghezza_stream 1000 e 5000, 500.0 per lunghezza_stream 10000)
# - Per le distribuzione esponenziale e poisson: lambda=1.0
###########################################################


DATA=$(date +%Y%m%d)
mkdir -p "simulazioni/simulazione_$DATA"
generate_stream="./stream_generator/generate_stream"


if [ ! -f "dataset_fileinput.csv" ]; then
  echo "file,distribuzione,parametro1,parametro2,lunghezza" > dataset_fileinput.csv
fi

# Parametri
lunghezza_stream=(200 1000 5000 10000)
distribuzioni=("uniforme" "esponenziale" "poisson")

# Esecuzione del programma con diversi parametri
for lunghezza in ${lunghezza_stream[@]}; do
  for distribuzione in ${distribuzioni[@]}; do
    if [ $distribuzione == "uniforme" ]; then
      if [ $lunghezza -eq 200 ]; then
        a=0.0
        b=100.0
      elif [ $lunghezza -eq 1000 ] || [ $lunghezza -eq 5000 ]; then
        a=0.0
        b=200.0
      else
        a=0.0
        b=500.0
      fi
      file_name="uniforme_${a}_${b}_${lunghezza}"
      $generate_stream -d $distribuzione -a $a -b $b -n $lunghezza -f $file_name
      mv $file_name.csv "simulazioni/simulazione_$DATA"
      echo "Distribuzione=$distribuzione, a=$a, b=$b, lunghezza=$lunghezza"
      echo "$file_name.csv,$distribuzione,$a,$b,$lunghezza" >> dataset_fileinput.csv
    else
      lambda=1.0
      if [ $distribuzione == "esponenziale" ]; then
        x=3
        file_name="${distribuzione}_${lambda}_${lunghezza}_10hat${x}"
        $generate_stream -d $distribuzione -l $lambda -x $x -n $lunghezza -f $file_name
        mv $file_name.csv "simulazioni/simulazione_$DATA"
        echo "Distribuzione $distribuzione, lambda=$lambda, lunghezza=$lunghezza"
        echo "$file_name.csv,$distribuzione,$lambda,$x,$lunghezza" >> dataset_fileinput.csv
      else
        file_name="${distribuzione}_${lambda}_${lunghezza}"
        $generate_stream -d $distribuzione -l $lambda -n $lunghezza -f $file_name
        mv $file_name.csv "simulazioni/simulazione_$DATA"
        echo "Distribuzione $distribuzione, lambda=$lambda, lunghezza=$lunghezza"
        echo "$file_name.csv,$distribuzione,$lambda,,$lunghezza" >> dataset_fileinput.csv
      fi
        
    fi
  done
done

mv dataset_fileinput.csv "simulazioni/simulazione_$DATA"