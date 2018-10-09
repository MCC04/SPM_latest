#!/bin/bash

rm -f ./results/gen_threads3.txt
touch ./results/gen_threads3.txt

#THREADS
#ARGS="treesNum treesHeigth nExec iterNum tol perc" < "functName.txt" run_gen_threads >> gen_threads.txt
#more points

for((j=1; j<=16; j*=2)); 
do
	for ((i=0; i<7; i+=1)); 
	do 
		make ARGS="400 5 $j 50 1.0 40.0" < "3x+sin2x_200.txt" run_gen_threads >> ./results/gen_threads3.txt
	done 
done

