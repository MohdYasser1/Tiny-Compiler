#include "Scanner.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    // Check if input file parameter is provided
    if (argc < 2)
    {
        cout << "Input file is not provided" << endl;
        return 1;
    }

    // Open the input file
    ifstream file(argv[1]);
    if (!file)
    {
        cout << "Error opening file: " << argv[1] << endl;
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

    // Write the tokens to an output file
    ofstream outFile("tokens.txt");
    if (!outFile)
    {
        cout << "Error opening output file" << endl;
        return 1;
    }
    outFile << tokenList;
    outFile.close();

    cout << "Token list has been written to tokens.txt" << endl;
    return 0;
}
