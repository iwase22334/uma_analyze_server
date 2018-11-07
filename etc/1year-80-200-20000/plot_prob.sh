#!/bin/bash 
gnuplot -e "set view 10,30,1,1; set isosample 10; splot 'probmap.dat' matrix w l; pause -1"
