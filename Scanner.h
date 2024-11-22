#ifndef SCANNER_H
#define SCANNER_H

#include <string>

using namespace std;

typedef enum
{
    IF,
    THEN,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    PLUS,
    MINUS,
    MULT,
    DIV,
    EQUAL,
    LESSTHAN,
    OPENBRACKET,
    CLOSEDBRACKET,
    SEMICOLON,
    ASSIGN,
    IDENTIFIER,
    NUMBER,
    ERROR
} TokenType;

typedef struct
{
    TokenType tokenval;
    string stringval;
    int numval;
    string errorMessage;
} TokenRecord;

class Scanner
{
public:
    Scanner(string);
    TokenRecord GetNextToken();
    string GetTokensList();

private:
    string code;
    int indx;
};

#endif // SCANNER_H