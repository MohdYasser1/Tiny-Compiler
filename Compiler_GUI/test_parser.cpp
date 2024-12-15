#include "Scanner.h"
#include "Parser.h"

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    cout << "Compiler Project" << endl;
    // Open the input file
    ifstream file("./input.txt");
    if (!file)
    {
        cout << "Error opening file: " << endl;
        return 1;
    }


    // Read the contents of the file into a string
    ostringstream buffer;
    buffer << file.rdbuf();
    string code = buffer.str();
    file.close();

    // Initialize the scanner with the input code
    Scanner scanner(code);

    // Get the list of tokens
    string tokenList = scanner.GetTokensList();

    // Initialize the parser with the token list
    Parser parser(tokenList);

    // Get the syntax tree
    Node* syntaxTree = parser.GetSyntaxTree();

    if (syntaxTree == NULL)
    {
        cout << parser.GetErrorMessage() << endl;
        return 1;
    }
    // Print the syntax tree
    parser.PrintSyntaxTree(syntaxTree, 0);

    return 0;
}