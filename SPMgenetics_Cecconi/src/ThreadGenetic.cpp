#include "../include/ThreadGenetic.h"

ThreadGenetic::ThreadGenetic(std::vector<Tree*> population, std::vector<Point> points, int perc,unsigned int nEx)
  :Genetic(population,points,perc){
    nExecutors=nEx;    
    needToStop=false;
    busy=0;

    for(int i=0; i<nExecutors;i++)
    {
        threads.push_back(std::thread([&]{
            while (true)
            {
                std::unique_lock<std::mutex> qLock(queueMutex);
                cv_tasks.wait(qLock, [this]{ 
                    return  needToStop || !tasks.empty() ; 
                });
                
                if (!tasks.empty())
                {
                    ++busy; 
                    auto task = tasks.front();                        
                    tasks.pop(); 
                    qLock.unlock();

                    task(); 

                    qLock.lock(); 
                    --busy;
                    workDone.notify_one();                    
                }
                else  if(tasks.empty() && needToStop)
                {
                    qLock.unlock();
                    break;                        
                }
            }
        }));
    }
}

ThreadGenetic::~ThreadGenetic(){
}

void ThreadGenetic::waitFinished()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    workDone.wait(lock, [this](){ return tasks.empty() && (busy==0); });
}

void ThreadGenetic::submit(std::function<void()> action)
{
    std::unique_lock<std::mutex> qLock(queueMutex);
    tasks.push(action);
    qLock.unlock();
    cv_tasks.notify_one();      
}

void ThreadGenetic::joinAll()
{
    std::unique_lock<std::mutex> lk(queueMutex);
    needToStop = true;
    lk.unlock();
    cv_tasks.notify_all();    
    
    for (std::thread& t : threads)
        if (t.joinable())
            t.join();
}

void ThreadGenetic::approxFunctionTh(int maxIter, double tol){
    int it=1;
    bool fit=false;

    std::chrono::system_clock::time_point start, end, startFit,endFit;
    start = std::chrono::system_clock::now();
           
    int chunk=population.size() / nExecutors;   
    int rem=population.size()%nExecutors;
    for(int k=0 ; k<population.size(); k+=chunk){
        auto w=std::bind(&ThreadGenetic::builder,this, k, k+chunk);      
        submit(w);                    
    }
    if(rem>0){
        auto w=std::bind(&ThreadGenetic::builder,this, population.size()-rem, population.size()-1);
        submit(w);
    }      

    while(it<maxIter)
    {     
        weigths.clear();
        startFit = std::chrono::system_clock::now(); 
        
        int chunk=indexes.size() / nExecutors;  
        int rem=indexes.size()%nExecutors;   
        for(int k=0 ; k<indexes.size(); k+=chunk){
            auto w = std::bind(&ThreadGenetic::updater,this, k, k+chunk);           
            submit(w);
        }
        if(rem>0){
            auto w = std::bind(&ThreadGenetic::updater,this, indexes.size()-rem, indexes.size()-1);  
            submit(w);
        }          
        waitFinished();        
                 
        std::sort(fitValues.begin(),fitValues.end(),
            [](std::pair<int, double>&i, std::pair<int, double>&j){ return i.second < j.second;});
        if(this->getBestFit().second < tol){
            fit=true;
            break;
        }         
        this->fitSum=std::accumulate(fitValues.begin(),fitValues.end(),0.0,
            [](double &a, std::pair<int,double> &b){return a + b.second;});      

        endFit = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedfit = endFit - startFit;//std::cout <<"ITER:"<<it<< "___Fit Thread Time:" <<elapsedfit.count()<<std::endl;
        nextGeneration();
        it+=1; 
    }    
    joinAll();

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

void ThreadGenetic::builder(int start, int end){
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

void ThreadGenetic::updater(int start, int end){
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
