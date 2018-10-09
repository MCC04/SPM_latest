#!/bin/bash

rm -f ./results/gen_seq2.txt
touch ./results/gen_seq2.txt

#SEQUENTIAL
#ARGS="treesNum treesHeigth nExec iterNum tol perc" < "functName.txt" run_gen_threads >> gen_threads.txt
#less trees

for ((i=0; i<7; i+=1)); 
do 
	make ARGS="200 5 0 50 1.0 40.0" < "3x+sin2x_100.txt" run_gen_threads >> ./results/gen_seq2.txt
done

