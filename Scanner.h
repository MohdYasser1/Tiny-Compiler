#ifndef SCANNER_H
#define SCANNER_H

#include <string>

using namespace std;

typedef enum {
    IF,
    THEN,
    ELSE,
    END,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EQUAL,
    LESS_THAN,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    SEMICOLON,
    ASSIGNMENT,
    IDENTIFIER,
    NUMBER
} TokenType;

typedef struct {
    TokenType tokenval;
    string stringval;
    int numval;
} TokenRecord;

class Scanner {
    public:
        Scanner(string);
        TokenRecord GetNextToken();
        string GetTokensList();
    private:
        string code;
};


#endif // SCANNER_H