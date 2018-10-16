#ifndef FFGenetic_H
#define FFGenetic_H
#include <vector>
#include <ff/parallel_for.hpp>
#include "../include/Genetic.h"

class FFGenetic: public Genetic{
    private:
        ff::ParallelFor *pf;
    protected:
        int nExecutors;
        void builder(int start,int end);
        void updater(int start,int end);
    public:
        FFGenetic(std::vector<Tree *> p, std::vector<Point> ps, int perc, unsigned int nEx);
        ~FFGenetic();

        virtual void approxFunctionFF(int maxIter, double tol);
};
#endif