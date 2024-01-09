#!/bin/bash
input="stream_generator/stream.csv"

distinct_item=$(sort $input | uniq | wc -l)

clear

echo "Distinct item reali: $distinct_item"

./eseguibile