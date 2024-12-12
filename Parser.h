#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Node
{
public:
    string value;
    Node *left;
    Node *right;
    Node(string value)
    {
        this->value = value;
        this->left = NULL;
        this->right = NULL;
    }
};

class Parser
{
public:
    Parser(string);
    void Parse();
    Node* GetParseTree();
private:
    string tokenList;
    vector<TokenRecord> tokens;
    TokenRecord currentToken;
    void initializeTokens();
    TokenRecord readToken(string value, string typeStr);
    int indx;
    Node* parseTree;
    string getNextToken();
};

#endif // PARSER_H