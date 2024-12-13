#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include <string>
#include <vector>
#include <sstream>
#include "Node.h"

using namespace std;


class Parser
{
public:
    Parser(string);
    Node* GetSyntaxTree();
private:
    string tokenList;
    vector<TokenRecord> tokens;
    TokenRecord currentToken;
    void initializeTokens();
    TokenRecord readToken(string value, string typeStr);
    int indx;
    Node* syntaxTree;
    TokenRecord getNextToken();
    Node* program(Node*);
    Node* stmtSequence(Node*);
    Node* statement(Node*);
    Node* ifstmt(Node*);
    Node* repeatstmt(Node*);
    Node* assignstmt(Node*);
    Node* readstmt(Node*);
    Node* writestmt(Node*);
    Node* exp(Node*);
    Node* comparisonop(Node*);
    Node* simpleexp(Node*);
    Node* addop(Node*);
    Node* term(Node*);
    Node* mulop(Node*);
    Node* factor(Node*);
};

#endif // PARSER_H
