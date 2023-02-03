#include "token.hpp"
#include <string>

//Class to represent and contain information about a token.
//Current implementation is only for the scanner, at minimum
//the 'type' will be changed to an enum later for better functionality
//
//string type: the type of token as outlined in the GoLF specs
//
//string attr: the lexeme, or an empty string when there is no lexeme
//
//int line: the line no. that the token was detected at
//
Token::Token(std::string name, std::string lexeme, int where){
    type = name;
    attr = lexeme;
    line = where;
}