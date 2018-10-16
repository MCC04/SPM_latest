#include "../include/NewThreadGen.h"

NewThreadGen::NewThreadGen(std::vector<Tree*> population, std::vector<Point> points, int perc,unsigned int nEx)
  :Genetic(population,points,perc){
    nExecutors=nEx;    
}

NewThreadGen::~NewThreadGen(){
}

void NewThreadGen::approxFunctionTh(int maxIter, double tol){
    int it=1;
    bool fit=false;

    std::chrono::system_clock::time_point start,end,startFit,endFit,startGen,endGen,startFitComp,endFitComp;
    start = std::chrono::system_clock::now();

    startFit = std::chrono::system_clock::now(); 
    int delta = population.size() / nExecutors;        
    std::vector<std::thread> pool; //POOL      
    for(int k=0 ; k<nExecutors-1; k+=1){
        pool.push_back(std::thread(&NewThreadGen::builder, this, k*delta, (k+1)*delta));
    }
    pool.push_back(std::thread(&NewThreadGen::builder, this, (nExecutors-1)*delta, population.size()));

    for(int i=0; i<this->nExecutors; i++)
        pool[i].join(); //JOIN POOL

    sortFitness();
    fitnessSum();
    endFit = std::chrono::system_clock::now();
    this->fitInitElapsed = endFit - startFit;       

    startGen = std::chrono::system_clock::now();
    nextGeneration();
    endGen = std::chrono::system_clock::now();
    this->nextGenElapsed += endGen - startGen;

    while(it<maxIter)
    {    
        startFit = std::chrono::system_clock::now(); 
        weigths.clear();  
        pool.clear();
        int deltaIdx = this->indexes.size()/nExecutors;  
        for(int k=0 ; k<nExecutors-1; k+=1){ //POOL
            pool.push_back(std::thread(&NewThreadGen::updater, this, k*deltaIdx, (k+1)*deltaIdx));
        }
        pool.push_back(std::thread(&NewThreadGen::updater, this, (nExecutors-1)*deltaIdx, this->indexes.size()));

        for(int i=0; i<this->nExecutors; i++)
            pool[i].join(); //JOIN POOL

        sortFitness();
        if(this->getBestFit().second < tol){
            fit=true;
            break;
        }       
        fitnessSum();    
        endFit = std::chrono::system_clock::now();   
        this->fitUpdateElapsed+=endFit - startFit;  

        startGen =std::chrono::system_clock::now();
        nextGeneration();
        endGen=std::chrono::system_clock::now(); 
        this->nextGenElapsed+=endGen -startGen;
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

void NewThreadGen::builder(int start, int end){
    for(int i=start;i<end;++i){
        double sum=computeFit(i);
        double n;
        if(sum!=DBL_MAX)
            n=sqrt(sum);                             
        else
            n=DBL_MAX;
        
        std::unique_lock<std::mutex> lk(mtx);
        fitValues.push_back(std::make_pair(i,n));
        lk.unlock();
    }
}

void NewThreadGen::updater(int start, int end){
    for(int i=start;i<end;++i){
        double sum=computeFit(indexes[i]);
        double n=0.0;
        if(sum!=DBL_MAX)
            n=sqrt(sum);
        else
            n=DBL_MAX;
     
        fitValues[indexes[i]].second=n;
    }
}  