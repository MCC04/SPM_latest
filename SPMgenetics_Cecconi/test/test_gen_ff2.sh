#!/bin/bash

rm -f ./results/gen_ff2.txt
touch ./results/gen_ff2.txt

#FASTFLOW
#ARGS="treesNum treesHeigth nExec iterNum tol perc" < "functName.txt" run_gen_threads >> gen_threads.txt
#less trees

for((j=1; j<=8; j*=2)); 
do
	for ((i=0; i<7; i+=1)); 
	do 
		make ARGS="200 5 $j 50 1.0 40.0" < "3x+sin2x_100.txt" run_gen_ff >> ./results/gen_ff2.txt
	done 
done

