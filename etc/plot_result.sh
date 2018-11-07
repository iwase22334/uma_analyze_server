#!/bin/bash 
i=0
for read line ;
do
  echo $line | awk '{for (i=2; i<NF; i++) printf("%s %s\n", i-1,$i);}' | gnuplot -e "set terminal png; set output 'result$i.png'; plot '-' w boxes"
  i++
done < result.dat
