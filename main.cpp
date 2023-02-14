// Author: Grant Duchars

#include <iostream>
#include <fstream>
#include <sstream>

#include "Token.hpp"

using namespace std;

bool get_program(istream &is, stringstream &code);
int get_declaration(istream &is, stringstream &code);
bool get_id_list(istream &is, stringstream &code);
bool get_type(istream &is, stringstream &code);
bool get_compound(istream &is, stringstream &code);
bool get_statement_list(istream &is, stringstream &code);
bool get_statement(istream &is, stringstream &code);
bool get_expression_list(istream &is, stringstream &code);
bool get_expression(istream &is, stringstream &code);
bool get_simple_expression(istream &is, stringstream &code);
bool get_term(istream &is, stringstream &code);
bool get_factor(istream &is, stringstream &code);

Token tok;

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

    stringstream ss;
    tok.get(ifile);
    if (!get_program(ifile, ss))
    {
        cout << "# Unsuccessful code ==========================================";
        return 1;
    }
    cout << "# Successful code ============================================" << endl;
    cout << ss.str();
    return 0;
}

bool get_program(istream &is, stringstream &code)
{
    switch (get_declaration(is, code))
    {
    case 1:
        return get_program(is, code);
    case 0:
        return get_compound(is, code);
    default:
        return false;
    }
}

int get_declaration(istream &is, stringstream &code)
{
    if (!get_type(is, code))
        return 0;
    if (!get_id_list(is, code))
        return -1;
    if (tok.type() != SEMICOLON)
        return -1;
    code << tok.value() << "\n";
    tok.get(is);
    return 1;
}

bool get_id_list(istream &is, stringstream &code)
{
    if (tok.type() != ID)
        return false;
    code << tok.value();
    tok.get(is);
    if (tok.type() == COMMA)
    {
        code << tok.value() << " ";
        tok.get(is);
        return get_id_list(is, code);
    }
    return true;
}

bool get_type(istream &is, stringstream &code)
{
    TokenType type = tok.type();
    if (type == INTEGER || type == FLOAT || type == VOID)
    {
        code << tok.value() << " ";
        tok.get(is);
        return true;
    }
    return false;
}

bool get_compound(istream &is, stringstream &code)
{
    if (tok.type() != BEGIN)
        return false;
    code << tok.type() << "\n";
    tok.get(is);
    if (!get_statement_list(is, code))
        return false;
    if (tok.type() != END)
        return false;
    code << "\n"
         << tok.type();
    tok.get(is);
    return true;
}

bool get_statement_list(istream &is, stringstream &code)
{
    if (!get_statement(is, code))
        return false;
    if (tok.type() == SEMICOLON)
    {
        code << tok.value() << "\n";
        tok.get(is);
        return get_statement_list(is, code);
    }
    return true;
}

bool get_statement(istream &is, stringstream &code)
{
    switch (tok.type())
    {
    case ID:
        code << tok.value();
        tok.get(is);
        if (tok.type() == LPAREN)
        {
            code << tok.value();
            tok.get(is);
            if (!get_expression_list(is, code))
                return false;
            if (tok.type() != RPAREN)
                return false;
            code << tok.value() << " ";
            tok.get(is);
        }
        else if (tok.type() == ASSIGNOP)
        {
            code << tok.value() << " ";
            tok.get(is);
            return get_expression(is, code);
        }
        break;

    case IF:
        code << tok.value() << " ";
        tok.get(is);
        if (!get_expression(is, code))
            return false;
        if (tok.type() != THEN)
            return false;
        code << tok.value() << "\n";
        tok.get(is);
        if (!get_compound(is, code))
            return false;
        if (tok.type() != ELSE)
            return false;
        code << tok.value() << "\n";
        tok.get(is);
        return get_compound(is, code);

    case WHILE:
        code << tok.value() << " ";
        tok.get(is);
        if (tok.type() != LPAREN)
            return false;
        code << tok.value();
        tok.get(is);
        if (!get_expression(is, code))
            return false;
        if (tok.type() != RPAREN)
            return false;
        code << tok.value() << "\n";
        tok.get(is);
        return get_compound(is, code);

    default:
        return false;
    }
    return true;
}

bool get_expression_list(istream &is, stringstream &code)
{
    if (!get_expression(is, code))
        return false;
    if (tok.type() == COMMA)
    {
        code << tok.value() << " ";
        tok.get(is);
        return get_expression_list(is, code);
    }
    return true;
}

bool get_expression(istream &is, stringstream &code)
{
    if (!get_simple_expression(is, code))
        return false;
    if (tok.type() == RELOP)
    {
        code << tok.value() << " ";
        tok.get(is);
        return get_simple_expression(is, code);
    }
    return true;
}

bool get_simple_expression(istream &is, stringstream &code)
{
    if (!get_term(is, code))
        return false;
    if (tok.type() == ADDOP)
    {
        code << tok.value() << " ";
        tok.get(is);
        return get_simple_expression(is, code);
    }
    return true;
}

bool get_term(istream &is, stringstream &code)
{
    if (!get_factor(is, code))
        return false;
    if (tok.type() == MULOP)
    {
        code << tok.value() << " ";
        tok.get(is);
        return get_term(is, code);
    }
    return true;
}

bool get_factor(istream &is, stringstream &code)
{
    switch (tok.type())
    {
    case ID:
        code << tok.value();
        tok.get(is);
        if (tok.type() == LPAREN)
        {
            code << tok.value();
            tok.get(is);
            if (!get_expression_list(is, code))
                return false;
            if (tok.type() != RPAREN)
                return false;
            code << tok.value() << " ";
            tok.get(is);
        }
        break;
    case NUM_REAL:
        code << tok.value() << " ";
        tok.get(is);
        break;
    case NUM_INT:
        code << tok.value() << " ";
        tok.get(is);
        break;
    case LPAREN:
        code << tok.value();
        tok.get(is);
        if (!get_expression(is, code))
            return false;
        if (tok.type() != RPAREN)
            return false;
        code << tok.value() << " ";
        tok.get(is);
        break;
    default:
        return false;
    }
    return true;
}