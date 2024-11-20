#include "Scanner.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    ifstream file("input.txt");
    if (!file) {
        cout << "Error opening file" << endl;
        return 1;
    }
    ostringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();
    file.close();
    Scanner scanner(code);
    string tokenList = scanner.GetTokensList();
    ofstream outFile("tokens.txt");
    if(!outFile) {
        cout << "Error opening file" << endl;
        return 1;
    }
    outFile << tokenList;
    outFile.close();
    return 0;
}