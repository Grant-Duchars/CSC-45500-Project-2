/**
 * Author: Grant Duchars
 */
#ifndef _GRAMMAR_HPP_
#define _GRAMMAR_HPP_

#include <string>
#include <fstream>
#include <sstream>

#include "Token.hpp"

class Grammar
{
private:
    // Private variables
    Token _tok;
    std::stringstream _code;
    std::istream &_is;
    // Private Methods
    bool get_program();
    int get_declaration();
    bool get_id_list();
    bool get_type();
    bool get_compound();
    bool get_statement_list();
    bool get_statement();
    bool get_expression_list();
    bool get_expression();
    bool get_simple_expression();
    bool get_term();
    bool get_factor();

public:
    // Constructor
    Grammar(std::istream &is) : _tok(), _code(), _is(is) {}
    // Getters
    std::string code() { return _code.str(); }
    // Public Methods
    bool parse_file();
};

#endif