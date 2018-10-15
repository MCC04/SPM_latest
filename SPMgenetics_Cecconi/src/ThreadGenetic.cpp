#include "../include/ThreadGenetic.h"

ThreadGenetic::ThreadGenetic(std::vector<Tree*> population, std::vector<Point> points, int perc,unsigned int nEx)
  :Genetic(population,points,perc){
    nExecutors=nEx;    
    needToStop=false;
    busy=0;
    //this->submitElapsed=std::chrono::seconds::zero();
    //this->waitElapsed=std::chrono::seconds::zero();
    //this->joinallElapsed=std::chrono::seconds::zero();

    /*std::chrono::system_clock::time_point start, end, startPoolLoop,endPoolLoop;
    start = std::chrono::system_clock::now();
    for(int i=0; i<nExecutors;i++)
    {
        startPoolLoop = std::chrono::system_clock::now();
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
        endPoolLoop = std::chrono::system_clock::now();
        std::chrono::duration<double> el = endPoolLoop-startPoolLoop;
        std::cout <<i<< "___Pool Loop iteration: " <<el.count()<<std::endl; 
    }
    end = std::chrono::system_clock::now();
    this->poolElapsed=end-start;
    std::cout << "___Pool Time:" <<poolElapsed.count()<<std::endl;
    */
}

ThreadGenetic::~ThreadGenetic(){
}

/*void ThreadGenetic::waitFinished()
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
}*/

/*void ThreadGenetic::approxFunctionTh(int maxIter, double tol){
    int it=1;
    bool fit=false;

    std::chrono::system_clock::time_point start, end, startFit,endFit,startGen,endGen,poolStart,poolEnd;
    start = std::chrono::system_clock::now();
    startFit = std::chrono::system_clock::now(); 
           
    int chunk=population.size() / nExecutors;   
    int rem=population.size()%nExecutors;
    for(int k=0 ; k<population.size(); k+=chunk){
        auto w=std::bind(&ThreadGenetic::builder,this, k, k+chunk);     
        poolStart= std::chrono::system_clock::now(); 
        submit(w);  
        poolEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> el = poolEnd - poolStart;
        std::cout <<it<< "___Submit init Time:" <<el.count()<<std::endl;  
        this->submitElapsed+=el;                
    }
    if(rem>0){
        auto w=std::bind(&ThreadGenetic::builder,this, population.size()-rem, population.size()-1);
        poolStart= std::chrono::system_clock::now(); 
        submit(w);  
        poolEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> el = poolEnd - poolStart;
        std::cout <<it<< "___Submit Init Time:" <<el.count()<<std::endl;  
        this->submitElapsed+=el; 
    }      

    poolStart= std::chrono::system_clock::now(); 
    waitFinished();
    poolEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> el = poolEnd - poolStart;
    std::cout <<it<< "___Wait Time:" <<el.count()<<std::endl;  
    this->waitElapsed+=el;

    endFit = std::chrono::system_clock::now();
    el = endFit - startFit;
    std::cout <<it<< "___Fits Init Time:" <<el.count()<<std::endl;

    while(it<maxIter)
    {    
        startFit = std::chrono::system_clock::now(); 
        weigths.clear();         
        
        int chunk=indexes.size() / nExecutors;  
        int rem=indexes.size()%nExecutors;   
        for(int k=0 ; k<indexes.size(); k+=chunk){
            auto w = std::bind(&ThreadGenetic::updater,this, k, k+chunk);           
            poolStart= std::chrono::system_clock::now(); 
            submit(w);  
            poolEnd = std::chrono::system_clock::now();
            std::chrono::duration<double> el = poolEnd - poolStart;
            std::cout <<it<< "___Submit Time:" <<el.count()<<std::endl;  
            this->submitElapsed+=el; 
        }
        if(rem>0){
            auto w = std::bind(&ThreadGenetic::updater,this, indexes.size()-rem, indexes.size()-1);  
            poolStart= std::chrono::system_clock::now(); 
            submit(w);  
            poolEnd = std::chrono::system_clock::now();
            std::chrono::duration<double> el = poolEnd - poolStart;
            std::cout <<it<< "___Submit Time:" <<el.count()<<std::endl;  
            this->submitElapsed+=el; 
        }   
        poolStart= std::chrono::system_clock::now(); 
        waitFinished();
        poolEnd = std::chrono::system_clock::now();
        std::chrono::duration<double> el = poolEnd - poolStart;
        std::cout <<it<< "___Wait Time:" <<el.count()<<std::endl;  
        this->waitElapsed+=el;

                
                 
        std::sort(fitValues.begin(),fitValues.end(),
            [](std::pair<int, double>&i, std::pair<int, double>&j){ return i.second < j.second;});
        if(this->getBestFit().second < tol){
            fit=true;
            break;
        }         
        this->fitSum=std::accumulate(fitValues.begin(),fitValues.end(),0.0,
            [](double &a, std::pair<int,double> &b){return a + b.second;});      

        /*endFit = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedfit = endFit - startFit;//std::cout <<"ITER:"<<it<< "___Fit Thread Time:" <<elapsedfit.count()<<std::endl;
        nextGeneration();
        it+=1; 
        */

/*
        endFit = std::chrono::system_clock::now();
        //std::chrono::duration<double> el = endFit - startFit; //std::cout <<it<< "___Fit Seq Time:" <<el.count()<<std::endl; 
        el = endFit - startFit; 
        std::cout <<it<< "___Fit Update Time:" <<el.count()<<std::endl;    
        this->fitElapsedTime+=el;  

        startGen =std::chrono::system_clock::now();
        nextGeneration();
        endGen=std::chrono::system_clock::now();
        el =  endGen -startGen ;
        std::cout <<it<< "___Next Gen Time:" <<el.count()<<std::endl;  
        this->nextGenElapsedTime+=el;
        it+=1;




    }    
    poolStart= std::chrono::system_clock::now(); 
    joinAll(); 
    poolEnd = std::chrono::system_clock::now();
    el = poolEnd - poolStart;
    std::cout <<it<< "___Joinall Time:" <<el.count()<<std::endl;  
    this->joinallElapsed+=el; 
    

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
}*/

void ThreadGenetic::approxFunctionTh(int maxIter, double tol){
    int it=1;
    bool fit=false;

    std::chrono::system_clock::time_point start, end, startFit,endFit,startGen,endGen,poolStart,poolEnd,
    fitCompStart,fitCompEnd;
    start = std::chrono::system_clock::now();
    startFit = std::chrono::system_clock::now(); 
           
    int chunk=population.size() / nExecutors;   
    int rem=population.size()%nExecutors;

 std::cout <<it<< "°°°°°°chunk: " <<chunk<<"---rem: " <<rem<<std::endl; 


std::vector<std::thread> poool;
 /*   for(int i=0; i<this->nExecutors-1; i++)
        pool.push_back(std::thread(&ThreadGenetic::builder,this, k, k+chunk));
    pool.push_back(std::thread(ThreadForest::worker,this,(this->nw-1)*delta, this->treeNum,x_vals,y_vals,points_no));
    
    for(int i=0; i<this->nExecutors; i++)
        pool[i].join();


*/

int delta = population.size() / nExecutors;  
for(int k=0 ; k<nExecutors-1; k+=1){
        poool.push_back(std::thread(&ThreadGenetic::builder, this, k*delta, (k+1)*delta));
        std::cout <<std::endl<< "°°°°°°start: " <<k*delta<< "---end: " <<(k+1)*delta<<std::endl;

}
        std::cout <<std::endl<< "°°°°°°start: " <<(nExecutors-1)*delta<< "---end: " <<population.size()<<std::endl;

        poool.push_back(std::thread(&ThreadGenetic::builder, this, (nExecutors-1)*delta, population.size()));


 for(int i=0; i<this->nExecutors; i++)
        poool[i].join();
/*int numThrs=0;

    for(int k=0 ; k+chunk<population.size(); k+=chunk){
        //auto w=std::bind(&ThreadGenetic::builder,this, k, k+chunk);     
            std::cout <<std::endl<< "start: " <<k<< "---end: " <<k+chunk<<std::endl;

        poolStart= std::chrono::system_clock::now(); 
        //submit(w);  
        poool.push_back(std::thread(&ThreadGenetic::builder, this, k, k+chunk));

        poolEnd = std::chrono::system_clock::now();
        numThrs+=1;
        std::chrono::duration<double> el = poolEnd - poolStart;
        std::cout <<it<< "___Submit init Time:" <<el.count()<<std::endl;  
        this->submitElapsed+=el;                
    }*/
    //if(rem>0){
       /* if(population.size()-chunk+1 < population.size()-1){
        //auto w=std::bind(&ThreadGenetic::builder,this, population.size()-rem, population.size()-1);
        poolStart= std::chrono::system_clock::now(); */
        //submit(w);  
        /*std::cout <<std::endl<< "°°°°°°start: " <<population.size()-rem<< "---end: " <<population.size()-1<<std::endl;
        poool.push_back(std::thread(&ThreadGenetic::builder,this, population.size()-rem,population.size()-1));*/

       /* std::cout <<std::endl<< "°°°°°°start: " <<population.size()-chunk+1<< "---end: " <<population.size()-1<<std::endl;
        poool.push_back(std::thread(&ThreadGenetic::builder,this, population.size()-chunk+1, population.size()-1));

        poolEnd = std::chrono::system_clock::now();
        numThrs+=1;
        std::chrono::duration<double> el = poolEnd - poolStart;
        std::cout <<it<< "___Submit Init Time:" <<el.count()<<std::endl;  
        this->submitElapsed+=el; 
    }      
std::cout <<it<< "___numThrs:" <<numThrs<<std::endl; */

   /* poolStart= std::chrono::system_clock::now(); 
    waitFinished();
    poolEnd = std::chrono::system_clock::now();*/
    std::chrono::duration<double> el = poolEnd - poolStart;
    //std::cout <<it<< "___Wait Time:" <<el.count()<<std::endl;  
    //this->waitElapsed+=el;

    endFit = std::chrono::system_clock::now();
    el = endFit - startFit;
    std::cout <<it<< "___Fits Init Time:" <<el.count()<<std::endl;

    std::cout <<this->fitValues.size()<< std::endl;

    for (int i =0; i < this->fitValues.size();i+=1){
        std::cout << this->fitValues[i].second << ", " ;
    }
std::cout << std::endl;



    nextGeneration();
   /* std::cout <<std::endl<<"#######"<<std::endl<<this->indexes.size()<< std::endl;

    for (int i =0; i < this->indexes.size();i+=1){
        std::cout << this->indexes[i]<< ", " ;
    }*/

    while(it<maxIter)
    {    
        startFit = std::chrono::system_clock::now(); 
        weigths.clear();  
        poool.clear();    
        
        /*for(int i=0;i<indexes.size();i++){

            fitCompStart =std::chrono::system_clock::now();
            double sum=computeFit(indexes[i]);
            fitCompEnd=std::chrono::system_clock::now();
            std::chrono::duration<double> compEl=fitCompEnd-fitCompStart;
            //std::cout << "___computeFit Time:" <<compEl.count()<<std::endl;
            this->compFitElapsedTime+=compEl;
            
            if(sum!=DBL_MAX)
                fitValues[indexes[i]].second=sqrt(sum);                                      
            else          
                fitValues[indexes[i]].second=DBL_MAX;
        }     */

        int deltaIdx = this->indexes.size()/nExecutors;  
        std::cout << "**deltaUpdate: " <<deltaIdx<<std::endl;
        for(int k=0 ; k<nExecutors-1; k+=1){
            poool.push_back(std::thread(&ThreadGenetic::updater, this, k*deltaIdx, (k+1)*deltaIdx));
            std::cout <<std::endl<< "°°°°°°start: " <<k*deltaIdx<< "---end: " <<(k+1)*deltaIdx<<std::endl;

        }
        std::cout <<std::endl<< "°°°°°°start: " <<(nExecutors-1)*deltaIdx<< "---end: " <<this->indexes.size()<<std::endl;

        poool.push_back(std::thread(&ThreadGenetic::updater, this, (nExecutors-1)*deltaIdx, this->indexes.size()));

 for(int i=0; i<this->nExecutors; i++)
        poool[i].join();


        std::sort(fitValues.begin(),fitValues.end(),
            [](std::pair<int, double>&i, std::pair<int, double>&j){ return i.second < j.second;});

        if(this->getBestFit().second < tol){
            fit=true;
            break;
        }       
        this->fitSum=std::accumulate(fitValues.begin(),fitValues.end(),0.0,
            [](double &a, std::pair<int,double> &b){return a + b.second;});     

        endFit = std::chrono::system_clock::now();
        //std::chrono::duration<double> el = endFit - startFit; //std::cout <<it<< "___Fit Seq Time:" <<el.count()<<std::endl; 
        el = endFit - startFit; 
        std::cout <<it<< "___Fit Update Time:" <<el.count()<<std::endl;    
        this->fitElapsedTime+=el;  

        startGen =std::chrono::system_clock::now();
        nextGeneration();
        endGen=std::chrono::system_clock::now();
        el =  endGen -startGen ;
        std::cout <<it<< "___Next Gen Time:" <<el.count()<<std::endl;  
        this->nextGenElapsedTime+=el;
        it+=1;
    }    
   /* poolStart= std::chrono::system_clock::now(); 
    joinAll(); 
    poolEnd = std::chrono::system_clock::now();
    el = poolEnd - poolStart;
    std::cout <<it<< "___Joinall Time:" <<el.count()<<std::endl;  
    this->joinallElapsed+=el; */
    

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
   // std::cout <<std::endl<< "start: " <<start<< "---end: " <<end<<std::endl;
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
