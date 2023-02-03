/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
/// Class to represent and contain information about a token.
/// Current implementation is only for the scanner, at minimum
/// the 'type' will be changed to an enum later for better functionality

#include "token.hpp"
#include <string>

//Token constructor
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