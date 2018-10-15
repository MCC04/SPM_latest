#include "../include/Genetic.h"

Genetic::Genetic() : percentage(0) {};

Genetic::Genetic(std::vector<Tree *> p, std::vector<Point> ps, int perc)
{
    this->population=p;
    this->points=ps;
    this->percentage=((int)ceil(population.size()*perc/100.0)); 
    this->fitSum=0;
    this->fitElapsedTime=std::chrono::seconds::zero();
    this->nextGenElapsedTime=std::chrono::seconds::zero();
    this->compFitElapsedTime=std::chrono::seconds::zero();
} 

Genetic::~Genetic() {}

void Genetic::approxFunction(int maxIter, double tol){
    int it=1;
    bool fit=false;

    std::chrono::system_clock::time_point start, end, startFit,endFit,startGen,endGen,fitCompStart,fitCompEnd;
    start = std::chrono::system_clock::now();
    startFit = std::chrono::system_clock::now();   

    for(int i=0;i<population.size();i++){

        fitCompStart =std::chrono::system_clock::now();
        double sum= computeFit( i);
        fitCompEnd=std::chrono::system_clock::now();
        std::chrono::duration<double> compEl=fitCompEnd-fitCompStart;
        //std::cout << "___computeFit Time:" <<compEl.count()<<std::endl;
        this->compFitElapsedTime+=compEl;

        if(sum!=DBL_MAX)
            fitValues.push_back(std::make_pair(i,sqrt(sum)));
        else
            fitValues.push_back(std::make_pair(i,DBL_MAX));
    }      
    endFit = std::chrono::system_clock::now();
    std::chrono::duration<double> el = endFit - startFit;
    std::cout <<it<< "___Fits Init Time:" <<el.count()<<std::endl;
        

    nextGeneration();


    while(it<maxIter) 
    {
        startFit = std::chrono::system_clock::now(); 
        weigths.clear();      
        
        for(int i=0;i<indexes.size();i++){

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
        }      
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
    this->approxElapsedTime = end - start;
}

double Genetic::computeFit(const int i){
    double sum=0.0;
    for(int j=0;j<points.size();j++){
        try{
            double fx = population[i]->eval(points[j].x);
            if(!isfinite(std::abs(fx)) || !isnormal(std::abs(fx)) || isnan(std::abs(fx))){
                sum=DBL_MAX;   
                break;
            }
            else{
                double sub= points[j].y - fx ;
                sum+= pow(sub,2) ;
            }            
        }
        catch(int e ){
            std::cout <<"Exception:"<<e<< std::endl;
            break;
        }               
    }  
    return sum;     
}

void Genetic::nextGeneration(){
    indexes.clear();

    //std::chrono::system_clock::time_point start, end;
    //start = std::chrono::system_clock::now();

    computeWeights();
    int gOp;
    for(int i=0; i<percentage;i++){//first= tree index; second= corresponding fit index
        std::pair<int,int> idxs=Select();       
        if(idxs.first>-1){
            weigths[idxs.second]=0;
            gOp=rand() % 2 + 1;
            if(gOp==1){
                Mutation(*population[idxs.first]);//std::cout <<"GEN_Count:"<<i<<"  Mutation on: "<< idxs.first <<std::endl;                
                indexes.push_back(idxs.second);
            }           
            else{
                std::pair<int,int> idxs2=Select();                                     
                if(idxs2.first>-1 && idxs.first!=idxs2.first)   
                {
                    weigths[idxs2.second]=0; 
                    Crossover(*population[idxs.first],*population[idxs2.first]);//std::cout <<"GEN_Count:"<<i<<"  Crossover on: "<< idxs.first <<std::endl;                        
                    indexes.push_back(idxs.second);
                    indexes.push_back(idxs2.second);
                } 
                //else std::cout << "WARN: idxs must be diff for Crossover!"<<std::endl;
            }//std::cout <<"GEN_Count:"<<i<<"  Crossover on: "<< idxs.first <<" and " << idxs2.first << std::endl;
        }
        //else std::cout << "WARN: Invalid selected index, value < 0!"<<std::endl;   
    } 
   // end = std::chrono::system_clock::now();
   // std::chrono::duration<double> el=end - start;
   // this->nextGenElapsedTime += el;  //std::cout << "GEN_elapsed:" << this->nextGenElapsedTime.count() << std::endl;
   // std::cout << "___NextGen elapsed:" << el.count() << std::endl;
}

void Genetic::computeWeights(){
    for (int i=0; i<fitValues.size();i++ )
        this->weigths.push_back(this->fitSum-fitValues[i].second);    
}

std::pair <int,int> Genetic::Select(){//returns (treeIndex, fitIndex)
    double cumulativeSum;
    double weigthSum=std::accumulate(weigths.begin(),weigths.end(),0.0);
    double randFit = rand() % (int) weigthSum ;

    for (int i =0; i<weigths.size();i++) {
        cumulativeSum += weigths[i];
        if (randFit < cumulativeSum)  
            return std::make_pair(fitValues[i].first,i);
    }
    return std::make_pair(-1,-1);
}

/******************
* GENETIC OPERATORS
*******************/
void Genetic::Mutation(Tree &t){
    int rnd=t.pickRandSubtree();
    Node *n=t.getNode(rnd);
    Node *par=n->getParent();
    int idx=n->getIndex();
        
    if(par!=nullptr && idx>=0){
        par->eraseChild(idx);
        int d = t.deleteSubTree(rnd);
        Tree *newTree=Tree::randTreeGen(d); 

        newTree->getRoot()->setIndex(idx);
        par->insertChild(newTree->getRoot(),idx);
        t.buildNodesList();
    }
    //else 
        //std::cout << "WARN: In Mutation, invalid subtree or null parent!"<<std::endl;
}

void Genetic::Crossover(Tree &t1,Tree &t2){
        int r1=t1.pickRandSubtree();
        int r2=t2.pickRandSubtree();

        Node *n1=t1.getNode(r1);
        int idx1=n1->getIndex();
        Node *n2=t2.getNode(r2);
        int idx2=n2->getIndex();
        Node *par1=n1->getParent(); 
        Node *par2=n2->getParent(); 

        par1->eraseChild(idx1);
        n2->setIndex(idx1);
        par1->insertChild(n2,idx1);
        par2->eraseChild(idx2);
        n1->setIndex(idx2);        
        par2->insertChild(n1,idx2);

        //UPDATE TREE
        t1.buildNodesList();  
        t2.buildNodesList();  
}
