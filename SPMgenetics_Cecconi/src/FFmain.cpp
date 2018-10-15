#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <cstdlib>
#include "../include/FFGenetic.h"

#define MINHEIGTH 3

int main(int argc, char const *argv[])
{
  srand(time(0));
    bool csv=true;

  if(argc != 7){
    std::cout <<"6 parameters needed\n"<<
      "example: \n"<<
      "make ARGS=\"100 5 4 20 1.0 30.0\" <\"3x+sin2x_100.txt\" run_gen_threads\"\n";	
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

  FFGenetic tf(population, pointsVect, perc, nExec);
  std::chrono::system_clock::time_point ffStart, ffEnd;
  ffStart = std::chrono::system_clock::now();
  tf.approxFunctionFF(iterNum,tol);
  std::vector<Tree *> newThGener=tf.getPopulation();
  ffEnd = std::chrono::system_clock::now();
  std::chrono::duration<double> totalFFElapsed = ffEnd - ffStart;

  if(!csv){
    std::cout <<"results FastFlow"<<
    " thr nw "<<nExec<<
    " comptime(s) "<<totalFFElapsed.count()<<" upfittime(s) "<<tf.fitElapsedTime.count()<<
    " tree_no "<<ppltSize<<" depthmax "<<maxHeigth<<
    " iterations "<<iterNum<<" points_no "<<points_no<<
    " tol "<<tol<<" fitness "<<tf.getBestFit().second  << "\n";
  }
  else{
    std::cout 
    <<nExec<< ", " //executors number
    <<totalFFElapsed.count()<<", " //completion time
    <<tf.approxElapsedTime.count()<< ", " //fitness comp time

    <<tf.fitElapsedTime.count()<< ", " //fitness comp time
    <<ppltSize<<", " //trees number
    <<maxHeigth<<", " //max tree heigth
    <<iterNum<<", " //max iterations
    <<points_no<<"," //points number
    <<tol<<", " //tolerance
    <<tf.getBestFit().second  << "\n"; //best computed fitness
  }
  return 0;
}
