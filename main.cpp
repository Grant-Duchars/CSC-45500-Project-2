// Author: Grant Duchars

#include <iostream>
#include <fstream>
#include <sstream>

#include "Grammar.hpp"

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

    Grammar g = Grammar(ifile);
    if (!g.parse_file())
    {
        cout << "# Unsuccessful code ==========================================";
        return -1;
    }
    cout << "# Successful code ============================================" << endl;
    cout << g.code() << endl;
    return 0;
}