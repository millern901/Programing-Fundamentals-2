//
// Created by Nicholas Miller on 3/21/18.
//

#include "pa3.h"

int main() {
    // Create file stream
    std::ifstream file_stream;
    std::string file_name;

    // Open the file
    std::cout << "Please enter the name of the input file: ";
    getline(std::cin, file_name);
    file_stream.open(file_name);

    // Tokenize the file
    Lexical_Analyzer lex(file_stream);
    token_vector tokens = lex.scan_tokens();

    // Parse the tokens
    Parser parse(tokens);
    parse.parse_tokens();

    // Close the file
    file_stream.close();

    return 0;
}


// Stack Implementation
//**********************************************************************************************************************
//**********************************************************************************************************************

template <class T>
Stack<T>::Stack() : arr(new T[16]), i(0), size(16) {}

//**********************************************************************************************************************

template <class T>
Stack<T>::Stack(int size) : arr(new T[size]), i(0), size(size) {}

//**********************************************************************************************************************

template <class T>
Stack<T>::~Stack() {
    delete[] arr;
}

//**********************************************************************************************************************

template <class T>
void Stack<T>::push(T token) {
    if (i == size)
    {
        reallocate();
    }
    arr[i++] = token;
}

//**********************************************************************************************************************

template <class T>
void Stack<T>::reallocate() {
    T *new_stack = new T[size * 2];
    for (int j = 0; j < i; j++)
    {
        new_stack[j] = arr[j];
    }
    arr = new_stack;
}

//**********************************************************************************************************************

template <class T>
T Stack<T>::pop() {
    if (!i)
    {
        throw std::runtime_error("Stack is empty. ");
    }
    return arr[--i];
}

//**********************************************************************************************************************

template <class T>
T Stack<T>::peek() {
    if (!i)
    {
        throw std::runtime_error("Stack is empty.");
    }
    return arr[i - 1];
}

//**********************************************************************************************************************

template <class T>
bool Stack<T>::empty() {
    return !i;
}




// Lexical Analyzer Implementation
//**********************************************************************************************************************
//**********************************************************************************************************************

Lexical_Analyzer::Lexical_Analyzer(std::ifstream &file_input_stream): file_input_stream(file_input_stream) {}

//**********************************************************************************************************************

std::vector<Token> Lexical_Analyzer::scan_tokens() {
    if (tokens.size() == 0) {
        while (file_input_stream) {
            Token T = scan_token();
            if (T.first != NULL_) {
                tokens.push_back(T);
            }
        }
    }
    return tokens;
}

//**********************************************************************************************************************

Token Lexical_Analyzer::scan_token() {
    char c = ' ';
    while (iswspace(c)) //Ignore whitespace and only work with meaningful characters.
    {
        file_input_stream.get(c);
        if (file_input_stream.eof())
        {
            return Token(NULL_, "");
        }
    }
    switch (c) {
        // Cases for PARENTHESES
        case '(':
            return Token(OPEN_PAREN, "(");
        case ')':
            return Token(CLOSE_PAREN, ")");

        // Cases for OPERATORS
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
            if (c == '+' && file_input_stream.peek() == '+') {
                file_input_stream.get();
                return Token(OPERATOR, "++");
            }
            else if (c == '-' && file_input_stream.peek() == '-') {
                file_input_stream.get();
                return Token(OPERATOR, "--");
            }
            return Token(OPERATOR, std::string(1, c));

        // Cases for DELIMITERS
        case ';': return Token(DELIMITER, ";");
        case ',': return Token(DELIMITER, ",");

        // Cases for CONSTANTS, IDENTIFIERS, and KEYWORDS
        default:
            if (isdigit(c)) {
                return constant(c);
            } else if (isalpha(c)) {
                return identifier(c);
            }
            throw std::runtime_error("Could not parse the given token");
    }
}

//**********************************************************************************************************************

Token Lexical_Analyzer::constant(char c) {
    std::string text(1, c);
    while (file_input_stream.get(c) && isdigit(c))
    {
        text += c;
    }
    file_input_stream.unget();
    return Token(CONSTANT, text);
}

//**********************************************************************************************************************

Token Lexical_Analyzer::identifier(char c) {
    std::string text(1, c);
    while (file_input_stream.get(c) && isalpha(c))
    {
        text += c;
    }
    file_input_stream.unget();

    if (KEYWORDS.find(text) != KEYWORDS.end()) //Determine if it's a keyword
    {
        return Token(KEYWORDS.at(text), text);
    }
    if (is_upper_case(text))
    {
        return Token(INVALID_NAME, text); //Since it wasn't in keyword and it's all uppercase it is known that it is a bad token.
    }
    if (is_lower_case(text))
    {
        return Token(IDENTIFIER, text); //Variables are all lowercase.
    }
    else
    {
        return Token(INVALID_NAME, text);
    }
}

//**********************************************************************************************************************

bool Lexical_Analyzer::is_upper_case(std::string text) {
    for (size_t i = 0; i < text.size(); i++)
    {
        if (!isupper(text[i]))
        {
            return false;
        }
    }
    return true;
}

//**********************************************************************************************************************

bool Lexical_Analyzer::is_lower_case(std::string text) {
    for (size_t i = 0; i < text.size(); i++)
    {
        if (!islower(text[i]))
        {
            return false;
        }
    }
    return true;
}




// Parser Implementation
//**********************************************************************************************************************
//**********************************************************************************************************************

Parser::Parser(token_vector tokens) {
    this->tokens = tokens;
}

//**********************************************************************************************************************

void Parser::parse_tokens() {
    for (Token T : tokens) {
        switch (T.first)
        {
            case BEGIN:
            case END:
                depth_check.push(T);
                keywords.insert(T);
                break;
            case FOR:
                keywords.insert(T);
                break;
            case OPEN_PAREN:
                balanced_parenthesis.push(T);
                break;
            case CLOSE_PAREN:
                balanced_parenthesis.push(T);
                break;
            case CONSTANT:
                constants.insert(T);
                break;
            case DELIMITER:
                delimiters.insert(T);
                break;
            case IDENTIFIER:
                identifiers.insert(T);
                break;
            case OPERATOR:
                operators.insert(T);
                break;
            case INVALID_NAME:
                syntax_errors.insert(T);
                break;
            default:
                throw std::runtime_error("Not able to parse tokens.");
        }
    }

    print_output();
}

//**********************************************************************************************************************

void Parser::print_set(std::set<Token> token_set) {
    if (token_set.empty())
    {
        std::cout << "N/A" << std::endl;
    }

    for (Token T : token_set) {
        std::cout << T.second << " ";
    }

    std::cout << std::endl;
}

//**********************************************************************************************************************

void Parser::print_output() {
    check_depth();
    check_balanced_parenthesis();

    std::cout << "Keywords: "; print_set(keywords);
    std::cout << "Identifiers: "; print_set(identifiers);
    std::cout << "Constants: "; print_set(constants);
    std::cout << "Operators: "; print_set(operators);
    std::cout << "Delimiters: "; print_set(delimiters);
    std::cout << "Syntax Error(s): "; print_set(syntax_errors);
}

//**********************************************************************************************************************

void Parser::check_depth() {
    int begins = 0;
    int fors = 0;
    int count_fors = 0;
    int count_ends = 0;
    int count_begins = 0;
    while (!depth_check.empty())
    {
        Token T = depth_check.pop();
        switch (T.first)
        {
            case FOR:
                count_fors++;
                if (begins == fors - 1 && begins >= 0 && fors - 1 >= 0)
                {
                    depth = std::max(depth, begins + 1);
                    fors--;
                }
                break;
            case BEGIN:
                count_begins++;
                begins--;
                break;
            case END:
                count_ends++;
                begins++;
                fors++;
                break;
            default:
                break;
        }
    }

    std::cout << "The depth of nested loop(s) is " << depth << std::endl;
    if (begins < 0 || count_fors > count_ends)
    {
        syntax_errors.insert(Token(END, "END"));
    }
    if (begins > 0 || count_fors > count_begins)
    {
        syntax_errors.insert(Token(BEGIN, "BEGIN"));
    }

}

//**********************************************************************************************************************

void Parser::check_balanced_parenthesis() {
    int pairs = 0;
    while (!balanced_parenthesis.empty())
    {
        Token t = balanced_parenthesis.pop();
        switch (t.first)
        {
            case CLOSE_PAREN:
                pairs--;
                break;
            case OPEN_PAREN:
                pairs++;
                break;
            default:
                break;
        }
    }
    if (pairs > 0)
    {
        syntax_errors.insert(Token(OPEN_PAREN, "("));
    }
    else if (pairs < 0)
    {
        syntax_errors.insert(Token(CLOSE_PAREN, ")"));
    }
}

//**********************************************************************************************************************