#include "Parser.h"

Parser::Parser(string tokenList)
{
    this->tokenList = tokenList;
    this->indx = 0;
    initializeTokens();
    this->currentToken = getNextToken();
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
    return token;
}

TokenRecord Parser::readToken(string value, string typeStr)
{
    TokenRecord token;
    if (typeStr == " IF")
    {
        token.tokenval = IF;
    }
    else if (typeStr == " THEN")
    {
        token.tokenval = THEN;
    }
    else if (typeStr == " END")
    {
        token.tokenval = END;
    }
    else if (typeStr == " REPEAT")
    {
        token.tokenval = REPEAT;
    }
    else if (typeStr == " UNTIL")
    {
        token.tokenval = UNTIL;
    }
    else if (typeStr == " READ")
    {
        token.tokenval = READ;
    }
    else if (typeStr == " WRITE")
    {
        token.tokenval = WRITE;
    }
    else if (typeStr == " PLUS")
    {
        token.tokenval = PLUS;
    }
    else if (typeStr == " MINUS")
    {
        token.tokenval = MINUS;
    }
    else if (typeStr == " MULT")
    {
        token.tokenval = MULT;
    }
    else if (typeStr == " DIV")
    {
        token.tokenval = DIV;
    }
    else if (typeStr == " EQUAL")
    {
        token.tokenval = EQUAL;
    }
    else if (typeStr == " LESSTHAN")
    {
        token.tokenval = LESSTHAN;
    }
    else if (typeStr == " OPENBRACKET")
    {
        token.tokenval = OPENBRACKET;
    }
    else if (typeStr == " CLOSEDBRACKET")
    {
        token.tokenval = CLOSEDBRACKET;
    }
    else if (typeStr == " SEMICOLON")
    {
        token.tokenval = SEMICOLON;
    }
    else if (typeStr == " ASSIGN")
    {
        token.tokenval = ASSIGN;
    }
    else if (typeStr == " IDENTIFIER")
    {
        token.tokenval = IDENTIFIER;
        token.stringval = value;
    }
    else if (typeStr == " NUMBER")
    {
        token.tokenval = NUMBER;
        token.numval = stoi(value);
    }
    else if (typeStr == " ERROR")
    {
        token.tokenval = ERROR;
        token.errorMessage = value;
    }
    return token;
}

void Parser::match(TokenType expected)
{
    if (currentToken.tokenval == expected)
    {
        currentToken = getNextToken();
    }
}

Node* Parser::GetSyntaxTree()
{
    /*
        A methid that generates the syntax tree
        returns the root node of the syntax tree
    */
    return program();
}

Node* Parser::program()
{
    return stmtSequence();
}

Node* Parser::stmtSequence()
{
    Node* temp = statement();
    Node* newTemp = temp;
    while (currentToken.tokenval == SEMICOLON)
    {
        // newTemp = new Node(";");
        match(SEMICOLON);
        newTemp->sibling = statement();
        newTemp = temp->sibling;
    }
    return temp;
}

Node* Parser::statement()
{
    Node* temp;
    switch (currentToken.tokenval)
    {
    case IF:
        temp = ifstmt();
        break;
    case REPEAT:
        temp = repeatstmt();
        break;
    case IDENTIFIER:
        temp = assignstmt();
        break;
    case READ:
        temp = readstmt();
        break;
    case WRITE:
        temp = writestmt();
        break;
    default:
        break;
    }
    return temp;
}

Node* Parser::ifstmt()
{
    match(IF);
    Node* temp = new Node("if");
    temp->type = "stmt";
    temp->leftChild = exp();
    match(THEN);
    temp->rightChild = stmtSequence();
    match(END);
    return temp;
}

Node* Parser::repeatstmt()
{
    match(REPEAT);
    Node* temp = new Node("repeat");
    temp->type = "stmt";
    temp->leftChild = stmtSequence();
    match(UNTIL);
    temp->rightChild = exp();
    return temp;
}

Node* Parser::assignstmt()
{
    Node* temp = new Node("assign\n("+currentToken.stringval+")");
    temp->type = "stmt";
    match(IDENTIFIER);
    match(ASSIGN);
    temp->leftChild = exp();
    return temp;
}

Node* Parser::readstmt()
{
    match(READ);
    Node* temp = new Node("read\n("+currentToken.stringval+")");
    temp->type = "stmt";
    match(IDENTIFIER);
    return temp;
}

Node* Parser::writestmt()
{
    match(WRITE);
    Node* temp = new Node("write");
    temp->type = "stmt";
    temp->leftChild = exp();
    return temp;
}

Node* Parser::exp()
{
    Node* temp = simpleexp();
    if (currentToken.tokenval == LESSTHAN || currentToken.tokenval == EQUAL)
    {
        Node* newTemp = comparisonop();
        newTemp->leftChild = temp;
        newTemp->rightChild = simpleexp();
        temp = newTemp;
    }
    return temp;
}

Node* Parser::comparisonop()
{
    Node* temp;
    switch (currentToken.tokenval)
    {
    case LESSTHAN:
        temp = new Node("op\n(<)");
        temp->type = "exp";
        match(LESSTHAN);
        break;
    case EQUAL:
        temp = new Node("op\n(=)");
        temp->type = "exp";
        match(EQUAL);
        break;
    default:
        break;
    }
    return temp;
}

Node* Parser::simpleexp()
{
    Node* temp = term();
    while (currentToken.tokenval == PLUS || currentToken.tokenval == MINUS)
    {
        Node* newTemp = addop();
        newTemp->leftChild = temp;
        newTemp->rightChild = term();
        temp = newTemp;
    }
    return temp;
}

Node* Parser::addop()
{
    Node* temp;
    switch (currentToken.tokenval)
    {
    case PLUS:
        temp = new Node("op\n(+)");
        temp->type = "exp";
        match(PLUS);
        break;
    case MINUS:
        temp = new Node("op\n(-)");
        temp->type = "exp";
        match(MINUS);
        break;
    default:
        break;
    }
    return temp;
}

Node* Parser::term()
{
    Node* temp = factor();
    while (currentToken.tokenval == MULT || currentToken.tokenval == DIV)
    {
        Node* newTemp = mulop();
        newTemp->leftChild = temp;
        newTemp->rightChild = factor();
        temp = newTemp;
    }
    return temp;
}

Node* Parser::mulop()
{
    Node* temp;
    switch (currentToken.tokenval)
    {
    case MULT:
        temp = new Node("op\n(*)");
        temp->type = "exp";
        match(MULT);
        break;
    case DIV:
        temp = new Node("op\n(/)");
        temp->type = "exp";
        match(DIV);
        break;
    default:
        break;
    }
    return temp;
}

Node* Parser::factor()
{
    Node* temp;
    switch (currentToken.tokenval)
    {
    case OPENBRACKET:
        match(OPENBRACKET);
        temp = exp();
        match(CLOSEDBRACKET);
        break;
    case NUMBER:
        temp = new Node("const\n("+to_string(currentToken.numval)+")");
        temp->type = "exp";
        match(NUMBER);
        break;
    case IDENTIFIER:
        temp = new Node("id\n("+currentToken.stringval+")");
        temp->type = "exp";
        match(IDENTIFIER);
        break;
    default:
        break;
    }
    return temp;
}   

void Parser::PrintSyntaxTree(Node* root, int level)
{
    if (root == NULL)
    {
        return;
    }
    for (int i = 0; i < level; i++)
    {
        cout << "  ";
    }
    cout << root->value << endl;
    PrintSyntaxTree(root->leftChild, level+1);
    PrintSyntaxTree(root->rightChild, level+1);
    PrintSyntaxTree(root->sibling, level);
}