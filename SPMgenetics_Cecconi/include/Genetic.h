#ifndef Genetic_H
#define Genetic_H
#include <algorithm>
#include <vector>
#include <math.h> 
#include <numeric>
#include <stdlib.h>
#include <chrono>
#include <float.h>
#include "../include/Tree.h"

struct Point{
    double x;
    double y;
};

class Genetic{
    protected:
        std::vector<Tree *> population;
        std::vector<Point> points;
        std::vector<std::pair<int, double>>fitValues;          
        std::vector<double> weigths;
        std::vector<double> indexes;
        int percentage;
        double fitSum;

        void computeWeights();
        std::pair<int, int> Select();        

    public:

        std::chrono::duration<double> fitElapsedTime; 
        std::chrono::duration<double> nextGenElapsedTime;

        Genetic() ;
        Genetic(std::vector<Tree *> p, std::vector<Point> ps, int perc);
        ~Genetic();

        virtual void approxFunction(int maxIter, double tol);
        virtual void nextGeneration();
        virtual double computeFit(const int i);
        virtual void Mutation(Tree &t);
        virtual void Crossover(Tree &t1,Tree &t2);

        std::vector<Tree *> getPopulation(){
            return population;
        }
      
        std::vector<std::pair<int, double>> getFits(){
            return fitValues;
        }

        std::pair<int, double> getBestFit(){
            return fitValues.front();;
        }
};
#endif 