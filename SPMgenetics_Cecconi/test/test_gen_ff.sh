#!/bin/bash

rm -f ./results/gen_ff1.txt
touch ./results/gen_ff1.txt

#FASTFLOW
#ARGS="treesNum treesHeigth nExec iterNum tol perc" < "functName.txt" run_gen_threads >> gen_threads.txt
#standard

for((j=1; j<=16; j*=2)); 
do
	for ((i=0; i<7; i+=1)); 
	do 
		make ARGS="400 5 $j 50 1.0 40.0" < "3x+sin2x_100.txt" run_gen_ff >> ./results/gen_ff1.txt
	done 
done

