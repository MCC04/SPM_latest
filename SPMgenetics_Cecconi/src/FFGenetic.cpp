#include "../include/FFGenetic.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <functional>

FFGenetic::FFGenetic(std::vector<Tree *> p, std::vector<Point> ps, int perc, unsigned int nEx)
:Genetic(p,ps,perc)
{
    this->nw= nEx;
    pf = new ff::ParallelFor(nEx);
};

FFGenetic::~FFGenetic(){};

std::mutex mtx;

void FFGenetic::approxFunctionFF(int maxIter, double tol){
    int it=1;
    bool fit=false;    

    std::chrono::system_clock::time_point start, end, startFit,endFit;
    start = std::chrono::system_clock::now();
             
    pf->parallel_for(0, this->population.size(),  1,0,          
    [&](const long i){            
        double sum=this->computeFit(i);
        double n=0.0;
        if(sum!=DBL_MAX)
            n=sqrt(sum);           
        else 
            n=DBL_MAX;
          
        std::unique_lock<std::mutex> lk(mtx);
        fitValues.push_back(std::make_pair(i,n));
        lk.unlock();            
    }, this->nw);          

    while(it<maxIter) 
    {     
        weigths.clear(); 
        startFit = std::chrono::system_clock::now();       
        pf->parallel_for(0, this->indexes.size(), 1,0,
        [&](const long i){ 
            double sum=this->computeFit(i);
            double n=0.0;
            if(sum!=DBL_MAX)
                n=sqrt(sum);            
            else
                n=DBL_MAX;

            fitValues[indexes[i]].second= n;                     
        }, this->nw);       

        std::sort(fitValues.begin(),fitValues.end(),
            [](std::pair<int, double>&i, std::pair<int, double>&j){ return i.second < j.second;});

        if(this->getBestFit().second < tol){
            fit=true;
            break;
        } 

        this->fitSum=std::accumulate(fitValues.begin(),fitValues.end(),0.0,
            [](double &a, std::pair<int,double> &b){return a + b.second;});

        endFit = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedfit = endFit - startFit; //std::cout <<"ITER:"<<it<< "___FF Time:" <<elapsedfit.count()<<std::endl;
        nextGeneration();
        it+=1;            
    }
    if(!fit){
        for(int i=0;i<indexes.size();i++){
            double sum=computeFit(indexes[i]);
            if(sum!=DBL_MAX)
                fitValues[indexes[i]].second=sqrt(sum);                                        
            else                    
                fitValues[indexes[i]].second=DBL_MAX;
        }
    }    
    std::sort(fitValues.begin(),fitValues.end(),
        [](std::pair<int, double>&i, std::pair<int, double>&j){ return i.second < j.second;});

    end = std::chrono::system_clock::now();
    this->fitElapsedTime = end - start;
}
