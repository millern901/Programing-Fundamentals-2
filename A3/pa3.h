//
// Created by Nicholas Miller on 3/21/18.
//

#ifndef PA3_PA3_H
#define PA3_PA3_H

// Data Structure Files
#include <string>
#include <vector>
#include <map>
#include <set>
#include <ctype.h>

// IO Files
#include <iostream>
#include <fstream>


// Token
//**********************************************************************************************************************
//**********************************************************************************************************************

enum Token_Type {
    OPEN_PAREN,
    CLOSE_PAREN,
    DELIMITER,
    OPERATOR,
    IDENTIFIER,
    CONSTANT,
    FOR,
    BEGIN,
    END,
    NULL_,
    INVALID_NAME
};

typedef std::pair<Token_Type, std::string> Token;
typedef std::vector<Token> token_vector;


// Stack Class
//**********************************************************************************************************************
//**********************************************************************************************************************

template <class T>
class Stack {
private:
    T *arr;
    int i;

    // Size of the Stack
    int size;

    // Reallocate the array
    void reallocate();

public:
    // Stack Constructors and Destructors
    Stack();
    Stack(int);
    ~Stack();

    // Push an element to the top
    void push(T);

    // Pop the top element
    T pop();

    // Peek at the top element
    T peek();

    // Is the stack empty
    bool empty();
};


// Lexical Analyzer Class
//**********************************************************************************************************************
//**********************************************************************************************************************

class Lexical_Analyzer {
private:
    std::ifstream &file_input_stream;

    // List of all lexemes
    token_vector tokens;

    // Map of valid keywords
    const std::map<std::string, Token_Type> KEYWORDS = {{"FOR", FOR}, {"BEGIN", BEGIN}, {"END", END}};

public:
    // Scanner constructor
    Lexical_Analyzer(std::ifstream &file_input_stream);

    // Scan for lexemes
    token_vector scan_tokens();
    Token scan_token();

    // Is the text upper case or lower case
    bool is_upper_case(std::string text);
    bool is_lower_case(std::string text);

    // Determines constants and identifiers
    Token constant(char c);
    Token identifier(char c);
};


// Parser Class
//**********************************************************************************************************************
//**********************************************************************************************************************

class Parser {
private:
    // Depth of nested loop(s)
    int depth = 0;

    // sets of tokens groups
    std::set<Token> keywords;
    std::set<Token> identifiers;
    std::set<Token> constants;
    std::set<Token> operators;
    std::set<Token> delimiters;
    std::set<Token> syntax_errors;

    // Passed token vector
    token_vector tokens;
    Stack<Token> depth_check;
    Stack<Token> balanced_parenthesis;
    void print_set(std::set<Token> token_set);

public:
    // Constructor for parser
    Parser(token_vector tokens);

    // Parses tokens
    void parse_tokens();

    // Checks for depth and balanced parentheses
    void check_depth();
    void check_balanced_parenthesis();

    // Prints output lists
    void print_output();
};

#endif //PA3_PA3_H