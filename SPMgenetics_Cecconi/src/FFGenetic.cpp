#include "../include/FFGenetic.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <functional>

FFGenetic::FFGenetic(std::vector<Tree *> p, std::vector<Point> ps, int perc, unsigned int nEx)
:Genetic(p,ps,perc)
{
    this->nExecutors= nEx;
    pf = new ff::ParallelFor(nEx);
};

FFGenetic::~FFGenetic(){
    delete pf;
};

std::mutex mtx;
void FFGenetic::approxFunctionFF(int maxIter, double tol){
    int it=1;
    bool fit=false;    

    std::chrono::system_clock::time_point start,end,startFit,endFit,startGen,endGen;
    start = std::chrono::system_clock::now();

    startFit = std::chrono::system_clock::now();
    int chunk=this->population.size()/this->nExecutors;
    pf->parallel_for(0, this->population.size(), 1,chunk,  //1,0,      
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
    }, this->nExecutors);     
    sortFitness();
    fitnessSum();
    endFit = std::chrono::system_clock::now();
    this->fitInitElapsed=endFit-startFit;
      
    startGen = std::chrono::system_clock::now();
    nextGeneration();
    endGen = std::chrono::system_clock::now();
    this->nextGenElapsed+=endGen-startGen;

    while(it<maxIter) 
    {     
        weigths.clear(); 
        startFit = std::chrono::system_clock::now(); 
        //chunk=this->indexes.size()/this->nExecutors;    
        pf->parallel_for(0, this->indexes.size(), 1,0,//1,chunk,
        [&](const long i){ 
            double sum=this->computeFit(i);
            double n=0.0;
            if(sum!=DBL_MAX)
                n=sqrt(sum);            
            else
                n=DBL_MAX;

            fitValues[indexes[i]].second= n;                     
        }, this->nExecutors);    

        /*for(int i=0;i<indexes.size();i++){
            //fitCompStart =std::chrono::system_clock::now();
            double sum=computeFit(indexes[i]);
            //fitCompEnd=std::chrono::system_clock::now();
            //std::chrono::duration<double> compEl=fitCompEnd-fitCompStart;
            //std::cout << "___computeFit Time:" <<compEl.count()<<std::endl;
            //sthis->compFitElapsedTime+=compEl;
            
            if(sum!=DBL_MAX)
                fitValues[indexes[i]].second=sqrt(sum);                                      
            else          
                fitValues[indexes[i]].second=DBL_MAX;
        }*/

        sortFitness();
        if(this->getBestFit().second < tol){
            fit=true;
            break;
        } 
        fitnessSum();
        endFit = std::chrono::system_clock::now();
        this->fitUpdateElapsed+=endFit-startFit;        
       
        startGen = std::chrono::system_clock::now();
        nextGeneration();
        endGen = std::chrono::system_clock::now();
        this->nextGenElapsed+=endGen-startGen;
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
    sortFitness();
    end = std::chrono::system_clock::now();
    this->approxElapsed = end - start;
}