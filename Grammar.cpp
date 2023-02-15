/**
 * Author: Grant Duchars
 */
#include <iostream>
#include <fstream>
#include <sstream>

#include "Grammar.hpp"

// Indentation Helper Methods
std::string indentation;
void indent()
{
    indentation.append("    ");
}
void unindent()
{
    if (indentation.length() != 0)
        indentation.erase(indentation.end() - 4, indentation.end());
}
// End Indentation Helpers

/**
 * Parses the file given to the class constructor
 * @returns False if the file's content fails the grammar
 */
bool Grammar::parse_file()
{
    _tok.get(_is);
    indentation = "";
    return get_program();
}

/**
 * Program -> Declaration Program | Compound
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_program()
{
    switch (get_declaration())
    {
    case 1:
        // Check for and append Program
        return get_program();
    case 0:
        // Check for and append Compound
        _code << "\n";
        return get_compound();
    default:
        return false;
    }
}

/**
 * Declaration -> Type Id_List SEMICOLON
 * @returns 0 on failed Type, -1 on failed Id_List or SEMICOLON, 1 on passed grammar
 */
int Grammar::get_declaration()
{
    // Check for and append Type
    if (!get_type())
        return 0;
    // Check for and append Id_List
    if (!get_id_list())
        return -1;
    // Check for SEMICOLON
    if (_tok.type() != SEMICOLON)
        return -1;
    // Append SEMICOLON
    _code << _tok.value() << "\n";
    _tok.get(_is);
    return 1;
}

/**
 * Id_List -> ID | ID COMMA Id_List
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_id_list()
{
    // Check for ID
    if (_tok.type() != ID)
        return false;
    // Append ID
    _code << _tok.value();
    _tok.get(_is);
    // Check for COMMA
    if (_tok.type() == COMMA)
    {
        // Append COMMA
        _code << _tok.value() << " ";
        _tok.get(_is);
        return get_id_list();
    }
    return true;
}

/**
 * Type -> INTEGER | FLOAT | VOID
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_type()
{
    // Check for INTEGER, FLOAT, or VOID
    TokenType type = _tok.type();
    if (type == INTEGER || type == FLOAT || type == VOID)
    {
        // Append INTEGER, FLOAT, or VOID
        _code << _tok.value() << " ";
        _tok.get(_is);
        return true;
    }
    return false;
}

/**
 * Compound -> BEGIN Statement_List END
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_compound()
{
    // Check for BEGIN
    if (_tok.type() != BEGIN)
        return false;
    // Append BEGIN
    _code << indentation << _tok.value() << "\n";
    indent();
    _tok.get(_is);
    // Check for and append Statement_List
    if (!get_statement_list())
        return false;
    // Check for END
    if (_tok.type() != END)
        return false;
    // Append END
    unindent();
    _code << "\n"
          << indentation
          << _tok.value();
    unindent();
    _tok.get(_is);
    return true;
}

/**
 * Statement_List -> Statement | Statement SEMICOLON Statement_List
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_statement_list()
{
    // Check for and append Statement
    if (!get_statement())
        return false;
    // Check for SEMICOLON
    if (_tok.type() == SEMICOLON)
    {
        // Append SEMICOLON
        _code << _tok.value() << "\n";
        _tok.get(_is);
        // Check for and append Statement_List
        return get_statement_list();
    }
    return true;
}

/**
 * Statement -> ID | ID LPAREN Expression_List RPAREN |
 * ID ASSIGNOP Expression | IF Expression THEN Compound
 * ELSE Compound | WHILE LPAREN Expression RPAREN Compound |
 * Compound
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_statement()
{
    switch (_tok.type())
    {
    case ID:
        // Append ID
        _code << indentation << _tok.value();
        _tok.get(_is);
        // Check for LPAREN
        if (_tok.type() == LPAREN)
        {
            // Append LPAREN
            _code << _tok.value();
            _tok.get(_is);
            // Check for and append Expression_List
            if (!get_expression_list())
                return false;
            // Check for RPAREN
            if (_tok.type() != RPAREN)
                return false;
            // Append RPAREN
            _code << _tok.value() << " ";
            _tok.get(_is);
        }
        // Check for ASSIGNOP
        else if (_tok.type() == ASSIGNOP)
        {
            // Append ASSIGNOP
            _code << " " << _tok.value() << " ";
            _tok.get(_is);
            // Check for and append Expression
            return get_expression();
        }
        break;

    case IF:
        // Append IF
        _code << indentation << _tok.value() << " ";
        _tok.get(_is);
        // Check for and append Expression
        if (!get_expression())
            return false;
        // Check for THEN
        if (_tok.type() != THEN)
            return false;
        // Append THEN
        _code << " " << _tok.value() << "\n";
        indent();
        _tok.get(_is);
        // Check for and append Compound
        if (!get_compound())
            return false;
        // Check for ELSE
        if (_tok.type() != ELSE)
            return false;
        // Append ELSE
        _code << "\n"
              << indentation << _tok.value() << "\n";
        indent();
        _tok.get(_is);
        // Check for and append Compound
        return get_compound();

    case WHILE:
        // Append WHILE
        _code << indentation << _tok.value() << " ";
        _tok.get(_is);
        // Check for LPAREN
        if (_tok.type() != LPAREN)
            return false;
        // Append LPAREN
        _code << _tok.value();
        _tok.get(_is);
        // Check for and append Expression
        if (!get_expression())
            return false;
        // Check for RPAREN
        if (_tok.type() != RPAREN)
            return false;
        // Append RPAREN
        _code << _tok.value() << "\n";
        indent();
        _tok.get(_is);
        // Check for and append Compound
        return get_compound();

    default:
        return false;
    }
    return true;
}

/**
 * Expression_List -> Expression | Expression COMMA Expression_List
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_expression_list()
{
    // Check for and append Expression
    if (!get_expression())
        return false;
    // Check for COMMA
    if (_tok.type() == COMMA)
    {
        // Append COMMA
        _code << _tok.value() << " ";
        _tok.get(_is);
        // Check for and append Expression_List
        return get_expression_list();
    }
    return true;
}

/**
 * Expression -> Simple_Expression | Simple_Expression RELOP Simple_Expression
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_expression()
{
    // Check for and append Simple_Expression
    if (!get_simple_expression())
        return false;
    // Check for RELOP
    if (_tok.type() == RELOP)
    {
        // Append RELOP
        _code << " " << _tok.value() << " ";
        _tok.get(_is);
        // Check for and append Simple_Expression
        return get_simple_expression();
    }
    return true;
}

/**
 * Simple_Expression -> Term | Term ADDOP Simple_Expression
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_simple_expression()
{
    // Check for and append Term
    if (!get_term())
        return false;
    // Check for ADDOP
    if (_tok.type() == ADDOP)
    {
        // Append ADDOP
        _code << " " << _tok.value() << " ";
        _tok.get(_is);
        // Check for and append Expression
        return get_simple_expression();
    }
    return true;
}

/**
 * Term -> Factor | Factor MULOP Term
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_term()
{
    // Check for and append Factor
    if (!get_factor())
        return false;
    // Check for MULOP
    if (_tok.type() == MULOP)
    {
        // Append MULOP
        _code << " " << _tok.value() << " ";
        _tok.get(_is);
        // Check for and append Term
        return get_term();
    }
    return true;
}

/**
 * Factor -> ID | ID LPAREN Expression_List RPAREN |
 * NUM_REAL | NUM_INT | LPAREN Expression RPAREN
 * @returns False if the file's content fails the grammar
 */
bool Grammar::get_factor()
{
    switch (_tok.type())
    {
    case ID:
        // Append ID
        _code << _tok.value();
        _tok.get(_is);
        // Check for LPAREN
        if (_tok.type() == LPAREN)
        {
            // Append LPAREN
            _code << _tok.value();
            _tok.get(_is);
            // Check for and append Expression_List
            if (!get_expression_list())
                return false;
            // Check for RPAREN
            if (_tok.type() != RPAREN)
                return false;
            // Append RPAREN
            _code << _tok.value();
            _tok.get(_is);
        }
        break;
    case NUM_REAL:
        // Append NUM_REAL
        _code << _tok.value();
        _tok.get(_is);
        break;
    case NUM_INT:
        // Append NUM_INT
        _code << _tok.value();
        _tok.get(_is);
        break;
    case LPAREN:
        // Append LPAREN
        _code << _tok.value();
        _tok.get(_is);
        // Check for and append Expression
        if (!get_expression())
            return false;
        // Check for RPAREN
        if (_tok.type() != RPAREN)
            return false;
        // Append RPAREN
        _code << _tok.value();
        _tok.get(_is);
        break;
    default:
        return false;
    }
    return true;
}