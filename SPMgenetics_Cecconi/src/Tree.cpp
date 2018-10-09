#include "../include/Tree.h"

Tree::Tree(Node *r)
{
    if(r!=nullptr){
        root=r;
        Tree::buildNodesList();         
    }
};

Tree::~Tree()
{
    delete Tree::root;
    delete[]  &nodes;
};


Node* Tree::getRoot(){
    return this->root;
}

void Tree::buildNodesList(){     
    if(this->root==nullptr)
        std::cout<< "!!!-buildChildList: EMPTY ROOT!"<< std::endl;
    else
    {
        std::stack<Node *> nodesStack;
        nodesStack.push(this->root);
        nodes.clear();

        while(!nodesStack.empty()){
            Node * temp=nodesStack.top();
            nodesStack.pop();

            if(temp!=nullptr)
            {        
                nodes.push_back(temp);        
                for(auto item:temp->getChList())
                    nodesStack.push(item);
            }
            else
                std::cout<< "!!!-buildChildList: null temp node in stack!"<< std::endl;
    }
    if(nodes.size()>1)
        nodes.erase(nodes.begin());
    else
        std::cout<< "!!!!!-buildChildList: can't erase root, size<1"<<std::endl;
    }         
}


double Tree::eval(double x){     
    if(this->root==nullptr){
        std::cout<< "!!!-buildChildList: EMPTY ROOT!"<< std::endl;
        return -1.0;
    }
    double res = this->root->eval(x);
    return res;
}

Node * Tree::getNode(int idx){
    if(nodes.size()>1){ 
        if(idx>this->nodes.size()-1 || idx<0) 
            return nullptr; 
        //else warning
    
        Node * temp=this->nodes[idx];
        return temp;
    }
    //else warning
};

int Tree::getNodesNumber(){
    return nodes.size();
}

int Tree::deleteSubTree(int idx){
    if(getNodesNumber()>1){ 
        if(idx>this->getNodesNumber()-1 || idx<0) 
            return -1; 
    
        std::stack<Node *> nodesStack;
        nodesStack.push(this->nodes[idx]);
        int i=0;
        while(!nodesStack.empty()){   
            Node * temp=nodesStack.top();
            nodesStack.pop();

            if(temp!=nullptr)
            {                  
                for(auto item:temp->getChList())
                    nodesStack.push(item);
                temp=nullptr;
                i+=1;
            }
            else
                std::cout<< "!!!-buildChildList: null temp node in stack!"<< std::endl;
        }
        return i;
    }
}

Tree * Tree::randTreeGen(const int &tDepth){
    int rnd=rand() % 2 + 1;
    Node *root = new Node(Node::randNode(rnd));
    root->level=0;
    std::stack<Node *> nodesStack; 
    std::stack<Node *> endStack;
    nodesStack.push(root);

    int i=0;
    Node *temp;
    while(!nodesStack.empty()){   
        temp=nodesStack.top();
        nodesStack.pop();
        if(temp!=nullptr){
            if(temp->level<tDepth-2){
                if(temp->getType()==M){
                    rnd=rand() % 2 + 1;
                    Node *t=new Node(Node::randNode(rnd));
                    i+=1;
                    t->level=i;
                    t->setIndex(0);
                    temp->addChild(t);
                    nodesStack.push(t);                    
                }
                else if(temp->getType()==B){   
                    rnd=rand() % 3 + 1;                
                    Node *t2;
                    if(rnd==3){
                        leaf l=Node::randLeaf();  
                        t2=new Node(l);
                    }
                    else
                        t2=new Node (Node::randNode(rnd)); 
                    
                    i+=1;
                    t2->level=i;
                    t2->setIndex(0);
                    temp->addChild(t2);
                    nodesStack.push(t2);

                    rnd=rand() % 2 + 1;
                    Node *t1 =new Node(Node::randNode(rnd));
                    t1->level=i;
                    t1->setIndex(1);
                    temp->addChild(t1);
                    nodesStack.push(t1);
                } 
            }
            else
                endStack.push(temp);         
        }             
    }    

    while(!endStack.empty()){
        temp=endStack.top();
        endStack.pop();   
    
        if(temp!=nullptr && temp->getType()!=L)
        {        
            leaf l=Node::randLeaf();  
            Node *t2= new Node(l);
            t2->setIndex(0);
            temp->addChild(t2);
            if(temp->getType()==B){
                l=Node::randLeaf(); 
                Node *t3=new Node(l);
                t3->setIndex(1);
                temp->addChild(t3);
            }
        }  
    }
    Tree *t=new Tree(root);
    return t;
}

int Tree::pickRandSubtree(){
    int nodesNum=this->getNodesNumber();
    int rnd= (rand() % nodesNum); 
    Node *n =this->getNode(rnd);//this never returns a leaf

    while(n==nullptr || n->getType()==L){   
        rnd= (rand() % nodesNum) ;
        n=this->getNode(rnd);
    }

    return rnd;
}

void Tree::printAll(){
    //cout << "TREE is printing:" << endl ;
    //cout << "ROOT ";
    //root->printAll(0);
}
