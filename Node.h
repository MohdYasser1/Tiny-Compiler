#ifndef NODE_H
#define NODE_H


#include <string>

using namespace std;

class Node
{
public:
    string value;
    string type;
    Node* sibling;
    Node* leftChild;
    Node* rightChild;
    Node(string value)
    {
        this->value = value;
        this->type = "";
        this->sibling = NULL;
        this->leftChild = NULL;
        this->rightChild = NULL;
    }
};

#endif // NODE_H