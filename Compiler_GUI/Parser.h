#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include <string>
#include <vector>
#include <sstream>
#include "Node.h"
#include <iostream>

using namespace std;


class Parser
{
public:
    Parser(string);
    Node* GetSyntaxTree();
    void PrintSyntaxTree(Node*, int);
    string GetErrorMessage();
private:
    string tokenList;
    vector<TokenRecord> tokens;
    TokenRecord currentToken;
    void initializeTokens();
    TokenRecord readToken(string value, string typeStr);
    int indx;
    Node* syntaxTree;
    TokenRecord getNextToken();
    void match(TokenType);
    Node* program();
    Node* stmtSequence();
    Node* statement();
    Node* ifstmt();
    Node* repeatstmt();
    Node* assignstmt();
    Node* readstmt();
    Node* writestmt();
    Node* exp();
    Node* comparisonop();
    Node* simpleexp();
    Node* addop();
    Node* term();
    Node* mulop();
    Node* factor();
    bool Error;
    string errorMessage;
};

#endif // PARSER_H
