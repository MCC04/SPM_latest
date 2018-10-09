#ifndef ThreadGenetic_H
#define ThreadGenetic_H

#include "../include/Genetic.h"
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadGenetic : public Genetic{    

    protected:
        unsigned int nExecutors;
        unsigned int busy;
        bool needToStop;
        std::mutex mtx; 
        std::mutex queueMutex;
        std::condition_variable workDone;
        std::condition_variable cv_tasks;      
        std::queue<std::function<void()>> tasks;
        std::vector<std::thread> threads;        

        void builder(int start,int end);
        void updater(int start,int end);
        virtual void submit(std::function<void()> action);
        virtual void joinAll();
        virtual void waitFinished();

    public:
        ThreadGenetic(std::vector<Tree *> p, std::vector<Point> ps, int perc, unsigned int nEx);
        ~ThreadGenetic();

        virtual void approxFunctionTh(int maxIter, double tol);
};
#endif