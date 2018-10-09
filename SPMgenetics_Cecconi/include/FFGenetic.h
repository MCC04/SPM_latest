#ifndef FFGenetic_H
#define FFGenetic_H
#include <vector>
#include <ff/parallel_for.hpp>
#include "../include/Genetic.h"

class FFGenetic: public Genetic{
    private:
        int nw;
        ff::ParallelFor *pf;
    public:
        FFGenetic(std::vector<Tree *> p, std::vector<Point> ps, int perc, unsigned int nEx);
        ~FFGenetic();

        virtual void approxFunctionFF(int maxIter, double tol);
};
#endif