#ifndef Node_H
#define Node_H

#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

enum NodeType {L, M, B, ND};
enum LeafType{var,num};
struct leaf {
  LeafType lt;
  double value;
};

class Node
{  
    std::vector<Node *> chList;
    char optr;
    leaf lf;
    NodeType type;
    Node *parent;
    int index;
   
    public: 
        int level;
        Node(Node *o1, Node *o2, char opr);
        Node(Node *o1, char opr);
        Node(char opr);
        Node(leaf l);
        ~Node();

        void addChild(Node *n);
        void insertChild(Node *n,int idx);
        void eraseChild(int idx);
        int getChNum();
        std::vector<Node *> getChList();
        void clearChList();
        
        NodeType getType();
        void setType(NodeType t);
        leaf getLeaf();
        int getIndex();
        void setIndex(int i);
        Node* getParent();
        void setParent(Node *p);

        double eval(double x);

        static char randNode(int chNum);
        static leaf randLeaf();   
};
#endif