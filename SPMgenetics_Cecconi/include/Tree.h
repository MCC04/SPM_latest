#ifndef Tree_H
#define Tree_H

#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include "../include/Node.h"

class Tree
{
    Node *root; 
    std::vector<Node *> nodes;
    public:
    Tree(Node *r);
    ~Tree();

    void buildNodesList();
    int getNodesNumber();
    Node* getRoot();
    Node* getNode(int idx);

    int pickRandSubtree();
    int deleteSubTree(int idx);
    static Tree* randTreeGen(const int &tDepth);
    double eval(double x);
    
    void printAll();    
};
#endif 