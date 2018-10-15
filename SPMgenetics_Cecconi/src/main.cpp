#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <cstdlib>
#include "../include/ThreadGenetic.h"

#define MINHEIGTH 3

int main(int argc, char const *argv[])
{
  srand(time(0));
  bool csv=true;

std::chrono::system_clock::time_point inputProcStart, inputProcEnd;
      inputProcStart = std::chrono::system_clock::now();

  if(argc != 7){
    std::cout <<"6 parameters needed\n"<<
      "example: \n"<<
      "make ARGS=\"100 5 4 20 1.0 30.0t\" < \"3x+sin2x_100.txt\" run_gen_threads\"\n";	
    return 0;
  } 
	int ppltSize = std::atoi(argv[1]);
	int maxHeigth = std::atoi(argv[2]);
  int nExec = std::atoi(argv[3]);
  int iterNum = std::atoi(argv[4]);
  double tol = std::atof(argv[5]);
  double perc = std::atof(argv[6]);
      
   long points_no;
   std::cin >> points_no;
   std::vector<double> xs;
   std::vector<double> ys;
   std::vector<Point> pointsVect;
   double tx,ty;
   for(int i=0; i<points_no; i++)
   {      
       std::cin >> tx;
       std::cin >> ty;
       pointsVect.push_back({tx,ty});
   }

    /*RANDOM GENERATION FOR POPULATION*/ 
    std::vector<Tree *> population ;
    for(int i=0; i<ppltSize;i++)
    {
      int randHeigth = rand()%((maxHeigth - MINHEIGTH) + 1) + MINHEIGTH; 
      population.push_back(Tree::randTreeGen(randHeigth));
    }

    inputProcEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> inputElapsed = inputProcEnd - inputProcStart;
    
    if(nExec==0){ //SEQUENTIAL
      std::chrono::system_clock::time_point seqStart, seqEnd;
      seqStart = std::chrono::system_clock::now();

      Genetic g(population, pointsVect, perc);
      g.approxFunction(iterNum,tol);    
      std::vector<Tree *> newGener=g.getPopulation();

      seqEnd = std::chrono::system_clock::now();
      std::chrono::duration<double> totalSeqElapsed = seqEnd - seqStart;

      if(!csv){
        std::cout <<"results sequential"<<
        " comptime(s) "<<totalSeqElapsed.count()<<" upfittime(s) "<<g.fitElapsedTime.count()<<
        " tree_no "<<ppltSize<<" depthmax "<<maxHeigth<<
        " iterations "<<iterNum<<" points_no "<<points_no<<
        " tol "<<tol<<" fitness "<<g.getBestFit().second  << "\n";
      }
      else{
        std::cout <<totalSeqElapsed.count()<< ", " //completion time
        <<g.fitElapsedTime.count()<<  ", " //fit computation time
        <<g.nextGenElapsedTime.count()<<  ", " //fit computation time
        <<g.approxElapsedTime.count()<<  ", " //fit computation time
        <<g.compFitElapsedTime.count()<<  ", " //fit computation time
        <<inputElapsed.count()<<  ", " //fit computation time
        <<ppltSize<< ", " //tree number
        <<maxHeigth<< ", " //max tree heigth
        <<iterNum<< ", "  //max iterations
        <<points_no<< "," //points number
        <<tol<< ", "  //tolerance
        <<g.getBestFit().second  << "\n"; //best computed fitness
      }
    }
    else{//THREAD
      std::chrono::system_clock::time_point thStart, thEnd;

      ThreadGenetic tg(population, pointsVect, perc, nExec);
      thStart = std::chrono::system_clock::now();
      tg.approxFunctionTh(iterNum, tol);
      std::vector<Tree *> newThGener=tg.getPopulation();
      thEnd = std::chrono::system_clock::now();
      std::chrono::duration<double> totalThreadElapsed = thEnd - thStart;

      if(!csv){
        std::cout <<"results thread"<<
        " thr nw "<<nExec<<
        " comptime(s) "<<totalThreadElapsed.count()<<" upfittime(s) "<<tg.fitElapsedTime.count()<<
        " tree_no "<<ppltSize<<" depthmax "<<maxHeigth<<
        " iterations "<<iterNum<<" points_no "<<points_no<<
        " tol "<<tol<<" fitness "<<tg.getBestFit().second  << "\n";
      }
      else{
        std::cout 
        <<nExec<< ", " //executors number
        <<totalThreadElapsed.count()<< ", " //completion time
        <<tg.fitElapsedTime.count()<<  ", " //fit computation time
        <<tg.nextGenElapsedTime.count()<<  ", " //fit computation time
        <<tg.approxElapsedTime.count()<<  ", " //fit computation time
        <<tg.compFitElapsedTime.count()<<  ", " //fit computation time
        //<<tg.submitElapsed.count()<<  ", "
        //<<tg.waitElapsed.count()<<  ", "
        //<<tg.joinallElapsed.count()<<  ", "
        //<<tg.poolElapsed.count()<<  ", "

        <<inputElapsed.count()<<  ", " //fit computation time
        <<ppltSize<<", " //trees number
        <<maxHeigth<<", " //max tree heigth
        <<iterNum<<", " //max iterations
        <<points_no<<"," //points number
        <<tol<<", " //tolerance
        <<tg.getBestFit().second  << "\n"; //best computed fitness
      }
    }
  return 0;
}
