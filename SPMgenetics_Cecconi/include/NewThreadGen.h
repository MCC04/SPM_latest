#ifndef ThreadGenetic_H
#define ThreadGenetic_H
#include "../include/Genetic.h"
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class NewThreadGen : public Genetic{    

    protected:
        unsigned int nExecutors;
        std::mutex mtx; 

        void builder(int start,int end);
        void updater(int start,int end);

    public:
        NewThreadGen(std::vector<Tree *> p, std::vector<Point> ps, int perc, unsigned int nEx);
        ~NewThreadGen();

        virtual void approxFunctionTh(int maxIter, double tol);
};
#endif