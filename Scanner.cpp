#include <Scanner.h>

Scanner::Scanner(const string code) {
    this->code = code;
    this->indx = 0;
}

TokenRecord Scanner::GetNextToken() {
    TokenRecord token;

    // STATE 0:
    // Skip whitespace and comments
    while (indx < code.length() && (isspace(code[indx] || code[indx] == '{' || code[indx] == '\n'))) {
        if (code[indx] == '{') {
            while (indx < code.length() && code[indx] != '}') {
                indx++;
            }
        }
        indx++;
    }

    // STATE 1:
    // Check for numbers
    if (isdigit(code[indx])) {
        token.tokenval = NUMBER;
        int start = indx;
        int end = indx+1;
        indx++;
        while (indx < code.length() && isdigit(code[indx])) {
            end++;
            indx++;
        }
        if (!isspace(code[indx])) {
            token.tokenval = ERROR;
            token.errorMessage = "Invalid number format";
        }
        token.numval = stoi(code.substr(start, end-start));
        return token;
    }

    // STATE 2:
    // Check for identifiers and reserved words
    if (isalpha(code[indx])) {
        int start = indx;
        int end = indx+1;
        indx++;
        while (indx < code.length() && isalnum(code[indx])) {
            end++;
            indx++;
        }
        string ident = code.substr(start, end-start);
        if (ident == "if") {
            token.tokenval = IF;
        } else if (ident == "then") {
            token.tokenval = THEN;
        } else if (ident == "else") {
            token.tokenval = ELSE;
        } else if (ident == "end") {
            token.tokenval = END;
        } else if (ident == "repeat") {
            token.tokenval = REPEAT;
        } else if (ident == "until") {
            token.tokenval = UNTIL;
        } else if (ident == "read") {
            token.tokenval = READ;
        } else if (ident == "write") {
            token.tokenval = WRITE;
        } else {
            token.tokenval = IDENTIFIER;
            token.stringval = ident;
        }
        return token;
    }

    // STATE 3:
    // Check for symbols
    switch (code[indx]) 
    {
    case ':':
        if (code[indx+1] == '=') {
            token.tokenval = ASSIGN;
            indx += 2;
        } else {
            token.tokenval = ERROR;
            token.errorMessage = "Invalid symbol";
            indx++;
        }
        break;

    case '+':
        token.tokenval = PLUS;
        indx++;
        break;
    case '-':
        token.tokenval = MINUS;
        indx++;
        break;
    case '*':
        token.tokenval = MULT;
        indx++;
        break;
    case '/':
        token.tokenval = DIV;
        indx++;
        break;
    case '=':
        token.tokenval = EQUAL;
        indx++;
        break;
    case '<':
        token.tokenval = LESSTHAN;
        indx++;
        break;
    case '(':
        token.tokenval = OPENBRACKET;
        indx++;
        break;
    case ')':
        token.tokenval = CLOSEDBRACKET;
        indx++;
        break;
    case ';':
        token.tokenval = SEMICOLON;
        indx++;
        break;
    
    default:
        token.tokenval = ERROR;
        token.errorMessage = "Invalid symbol";
        break;
    } 
    return token;
}