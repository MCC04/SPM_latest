#!/bin/bash

rm -f ./results/gen_seq3.txt
touch ./results/gen_seq3.txt

#SEQUENTIAL
#ARGS="treesNum treesHeigth nExec iterNum tol perc functName.txt" run_gen_threads >> gen_threads.txt
#more points

for ((i=0; i<7; i+=1)); 
do 
	make ARGS="400 5 0 50 1.0 40.0" < "3x+sin2x_200.txt" run_gen_threads >> ./results/gen_seq3.txt
done 
