#include "../include/Node.h"
    
Node::Node(Node* o1, Node* o2, char op){
    this->chList.push_back(o1);
    this->chList.push_back(o2);
    o1->parent=this;
    o2->parent=this;
    o1->index=0;
    o2->index=1;
    this->optr=op;
    this->type=B;
}

Node::Node(Node *o1, char op){
    this->chList.push_back(o1);
    o1->parent=this;
    o1->index=0;
    this->optr=op;
    this->type=M;
}

Node::Node(leaf l){
    this->lf=l;
    this->type=L;
}

Node::Node(char op){
    this->optr=op;
    switch(optr){
        case 'c':
        case 's':
        case 'e':
            this->type=M;
        break;
        case '+':
        case '-':
        case '*':
        case  '/':
            this->type=B;
        break;
    }
}

Node::~Node(){
    delete &optr;
    delete &lf;
    delete &type;
    delete parent;
}

int Node::getIndex(){
    return this->index;
}

void Node::setIndex(int i){
    this->index=i;
}

void Node::eraseChild(int idx){
    this->chList.erase(this->chList.begin()+idx);
}

void Node::addChild(Node *n)
{
    if(chList.size()<2){
        this->chList.push_back(n);
        n->parent=this;
    }
    else
        std::cout << "Can't add new operator to node" << std::endl;        
}

void Node::insertChild(Node *n,int idx){
    if(this->chList.size()<2){
        if(idx==0)
            this->chList.insert(this->chList.begin(),n);
        else
            this->chList.push_back(n);
        n->parent=this;
    }
    else
        std::cout << "Can't add new operator to node" << std::endl;
}


int Node::getChNum(){
    chList.size();
}

Node* Node::getParent(){
    return this->parent;
}

void Node::setParent(Node * p){
     this->parent=p;
}

std::vector<Node*> Node::getChList(){
    return chList;
}

void Node::clearChList(){
     chList.clear();
}

NodeType Node::getType(){
    return this->type;
}

void Node::setType(NodeType t){
    this->type=t;
}

leaf Node::getLeaf(){
    return this->lf;
}

double Node::eval(double x){
    double res=0.0;
    switch(type){
        case L:
            if(lf.lt==num) res=lf.value;
            else res=x;
        break;
        case M:
            if(chList.size()>0)   
            {
                switch(optr){
                    case 'c':
                        res=cos(chList[0]->eval(x));
                    break;
                    case 's': 
                        res=sin(chList[0]->eval(x));
                    break;
                    case 'e': 
                        res=exp(chList[0]->eval(x));
                    break;
                }
            }
            else res=-1.0;
        break;
        case B:
            if(chList.size()>0)   
            {
                switch(optr){
                    case '+':
                        res=chList[0]->eval(x)+chList[1]->eval(x);
                    break;
                    case '*': 
                        res=chList[0]->eval(x)*chList[1]->eval(x);
                    break;
                    case '-': 
                        res=chList[0]->eval(x)-chList[1]->eval(x);
                    break;
                    case '/': 
                    double n2=chList[1]->eval(x);
                        if(n2!=0.0){
                            res=chList[0]->eval(x)/n2;
                        }
                        else
                            res=-1.0;
                        
                    break;
                }
            }
            else res=-1.0;
        break;
        default:
            res=-1.0;
        break;
    }
    return res;
}

char Node::randNode(int type){
    if(type==1){//mono
        int oper=rand() % 3 + 1;
        switch(oper){
            case 1: return 'c'; 
            case 2: return 's'; 
            case 3: return 'e'; 
        } 
    }

    if(type==2){//binary
        int oper=rand() % 4 + 1;
        switch(oper){
            case 1: return '+'; 
            case 2: return '-'; 
            case 3: return '*'; 
            case 4: return '/'; 
        }
    }
}

leaf Node::randLeaf(){
    int lType=rand() % 2 + 1;
    if(lType==1){
        double randVal= (double) (rand() % 6) ; //for the moment vals are 0-5
        leaf constVal({num,randVal});
        return constVal;
    } 
    else{
        leaf variable({var,0});
        return variable;
    }
}
