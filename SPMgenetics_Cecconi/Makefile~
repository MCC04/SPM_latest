CC=g++

.PHONY: cleanall run_gen_threads run_gen_ff

gen_ff: ./src/FFmain.cpp node.o tree.o gen.o ffgen.o
	$(CC) ./src/FFmain.cpp -std=c++17 -DNO_DEFAULT_MAPPING -g -O2 -I. -I/usr/local/fastflow -pthread Node.o Tree.o Genetic.o FFGenetic.o -o gen_ff
	#$(CC) ./src/FFmain.cpp -std=c++11 -DNO_DEFAULT_MAPPING -g -O3 -I. -I/home/maria/fastflow -pthread Node.o Tree.o Genetic.o FFGenetic.o -o gen_ff

gen_threads: ./src/main.cpp node.o tree.o gen.o thgen.o
	$(CC)  ./src/main.cpp -std=c++17 -DNO_DEFAULT_MAPPING -g -I. -pthread Node.o Tree.o Genetic.o ThreadGenetic.o  -o gen_threads

run_gen_ff:
	./gen_ff ${ARGS} 

run_gen_threads:
	./gen_threads ${ARGS}

node.o: ./include/Node.h ./src/Node.cpp 
	$(CC) -c ./src/Node.cpp

tree.o: ./include/Tree.h ./src/Tree.cpp
	$(CC) -c ./src/Tree.cpp

gen.o: ./include/Genetic.h ./src/Genetic.cpp 
	$(CC) -c ./src/Genetic.cpp

thgen.o: ./include/ThreadGenetic.h ./src/ThreadGenetic.cpp 
	$(CC) -c ./src/ThreadGenetic.cpp

ffgen.o: ./include/FFGenetic.h ./src/FFGenetic.cpp 
	#$(CC) -O3 -I. -I/home/maria/fastflow -c ./src/FFGenetic.cpp
	$(CC) -O2 -I. -I/usr/local/fastflow -c ./src/FFGenetic.cpp 
cleanall:
	@for s in *.o ; do if [ -e $$s ] ; then rm $$s ; fi done
	@for s in *.out ; do if [ -e $$s ] ; then rm $$s ; fi done
	rm -f gen_threads.txt
	rm -f gen_ff.txt
	rm -f gen_ff
	rm -f gen_threads
	@echo "cleanup done"




	
