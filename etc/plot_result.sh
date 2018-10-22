#!/bin/bash 
cat input.dat | awk '{for (i=2; i<NF; i++) printf("%s %s\n", i-1,$i);}' | gnuplot -e "set terminal png; set output 'input.png'; plot '-' w boxes"
