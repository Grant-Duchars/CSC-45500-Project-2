// Author: Grant Duchars

#include <iostream>
#include <fstream>

#include "Token.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "USAGE: ./project2 <file>" << endl;
        return -1;
    }

    ifstream ifile(argv[1]);
    if (!ifile)
    {
        cerr << "ERROR: Could not open file: " << argv[1] << endl;
        return -1;
    }

    Token tok;

    tok.get(ifile);
}