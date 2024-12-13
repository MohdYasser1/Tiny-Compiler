#include "Parser.h"

Parser::Parser(string tokenList)
{
    this->tokenList = tokenList;
    this->indx = 0;
    initializeTokens();
}

void Parser::initializeTokens()
{
    // Get the next token from the token list
    // and return the token value
    istringstream iss(tokenList);
    string line;
    while(getline(iss, line))
    {
        istringstream lineStream(line);
        string value, typeStr;
        if(getline(lineStream, value, ',') && getline(lineStream, typeStr))
        {
            tokens.push_back(readToken(value, typeStr));
        }
    }
}

TokenRecord Parser::getNextToken()
{
    if(indx < tokens.size())
    {
        TokenRecord token = tokens[indx];
        indx++;
        return token;
    }
    TokenRecord token;
    token.tokenval = ERROR;
    token.errorMessage = "No more tokens";
}

TokenRecord Parser::readToken(string value, string typeStr)
{
    TokenRecord token;
    if (typeStr == "IF")
    {
        token.tokenval = IF;
    }
    else if (typeStr == "THEN")
    {
        token.tokenval = THEN;
    }
    else if (typeStr == "END")
    {
        token.tokenval = END;
    }
    else if (typeStr == "REPEAT")
    {
        token.tokenval = REPEAT;
    }
    else if (typeStr == "UNTIL")
    {
        token.tokenval = UNTIL;
    }
    else if (typeStr == "READ")
    {
        token.tokenval = READ;
    }
    else if (typeStr == "WRITE")
    {
        token.tokenval = WRITE;
    }
    else if (typeStr == "PLUS")
    {
        token.tokenval = PLUS;
    }
    else if (typeStr == "MINUS")
    {
        token.tokenval = MINUS;
    }
    else if (typeStr == "MULT")
    {
        token.tokenval = MULT;
    }
    else if (typeStr == "DIV")
    {
        token.tokenval = DIV;
    }
    else if (typeStr == "EQUAL")
    {
        token.tokenval = EQUAL;
    }
    else if (typeStr == "LESSTHAN")
    {
        token.tokenval = LESSTHAN;
    }
    else if (typeStr == "OPENBRACKET")
    {
        token.tokenval = OPENBRACKET;
    }
    else if (typeStr == "CLOSEDBRACKET")
    {
        token.tokenval = CLOSEDBRACKET;
    }
    else if (typeStr == "SEMICOLON")
    {
        token.tokenval = SEMICOLON;
    }
    else if (typeStr == "ASSIGN")
    {
        token.tokenval = ASSIGN;
    }
    else if (typeStr == "IDENTIFIER")
    {
        token.tokenval = IDENTIFIER;
        token.stringval = value;
    }
    else if (typeStr == "NUMBER")
    {
        token.tokenval = NUMBER;
        token.numval = stoi(value);
    }
    else if (typeStr == "ERROR")
    {
        token.tokenval = ERROR;
        token.errorMessage = value;
    }
    return token;
}

Node* Parser::GetSyntaxTree()
{
    /*
        A methid that generates the syntax tree
        returns the root node of the syntax tree
    */
    return program(syntaxTree);
}

Node* Parser::program(Node* node)
{
     stmtSequence(node);
}

Node* Parser::stmtSequence(Node* node)
{
    statement(node);
    Node* temp;
    Node* newTemp;
    temp =
}
