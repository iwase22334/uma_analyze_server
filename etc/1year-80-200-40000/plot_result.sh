#!/bin/bash 
trap 'rm ./tmp.txt' EXIT
N=$(head -n 1 result.dat | wc -w);for i in `seq 2 $((N-1))`;do awk '{print $'$i'}' < result.dat | tr '\n' '\t'; echo ;done | nl > tmp.txt
gnuplot -e "set terminal png; set output 'result$i.png'; plot for[col=2:$((N-1))] 'tmp.txt' using 1:col w lines"
