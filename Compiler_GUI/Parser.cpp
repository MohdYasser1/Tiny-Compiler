#include "Parser.h"

Parser::Parser(string tokenList)
{
    this->tokenList = tokenList;
    this->indx = 0;
    initializeTokens();
    this->currentToken = getNextToken();
    this->Error = false;
    this->errorMessage = "";
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

string Parser::GetErrorMessage()
{
    return errorMessage;
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
    token.tokenval = ENDOFFILE;
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
    else if (typeStr == " ELSE")
    {
        token.tokenval = ELSE;
    }
    else if (typeStr == " ERROR")
    {
        this->Error = true;
        this->errorMessage = "Error: " + value;
    }
    return token;
}

void Parser::match(TokenType expected)
{
    if (currentToken.tokenval == expected)
    {
        currentToken = getNextToken();
    }
    else if (currentToken.tokenval == ENDOFFILE)
    {
        // Do nothing
    }
    
    else
    {
        Error = true;
        errorMessage = "Error: Expected " + tokenTypeToString(expected) + " but found " + tokenTypeToString(currentToken.tokenval);
    }
}

Node* Parser::GetSyntaxTree()
{
    /*
        A methid that generates the syntax tree
        returns the root node of the syntax tree
    */
   Node* root = program();
   if (Error){
    return NULL;
   }
   else
    return root;
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
        newTemp = newTemp->sibling;
    }
    return temp;
}

Node* Parser::statement()
{
    Node* temp = NULL;
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
        Error = true;
        errorMessage = "Error: Expected statement but found " + tokenTypeToString(currentToken.tokenval);
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
    if (currentToken.tokenval == ELSE)
    {
        match(ELSE);
        temp->optional = stmtSequence();
    }
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
    Node* temp=NULL;
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
        Error = true;
        errorMessage = "Error: Expected comparison operator but found " + tokenTypeToString(currentToken.tokenval);
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
    Node* temp = NULL;
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
        Error = true;
        errorMessage = "Error: Expected add/minus operator but found " + tokenTypeToString(currentToken.tokenval);
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
    Node* temp= NULL;
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
        Error = true;
        errorMessage = "Error: Expected mult/div operator but found " + tokenTypeToString(currentToken.tokenval);
        break;
    }
    return temp;
}

Node* Parser::factor()
{
    Node* temp=NULL;
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
        Error = true;
        errorMessage = "Error: Expected factor but found " + tokenTypeToString(currentToken.tokenval);
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
    PrintSyntaxTree(root->optional, level);
    PrintSyntaxTree(root->sibling, level);
}

string Parser::tokenTypeToString(TokenType type) {
    switch (type) {
        case IF: return "IF";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        case END: return "END";
        case REPEAT: return "REPEAT";
        case UNTIL: return "UNTIL";
        case READ: return "READ";
        case WRITE: return "WRITE";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MULT: return "MULT";
        case DIV: return "DIV";
        case EQUAL: return "EQUAL";
        case LESSTHAN: return "LESSTHAN";
        case OPENBRACKET: return "OPENBRACKET";
        case CLOSEDBRACKET: return "CLOSEDBRACKET";
        case SEMICOLON: return "SEMICOLON";
        case ASSIGN: return "ASSIGN";
        case IDENTIFIER: return "IDENTIFIER";
        case NUMBER: return "NUMBER";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}