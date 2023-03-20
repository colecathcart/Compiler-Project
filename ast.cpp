/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// Feb 28, 2023
/// Class to represent and contain information about an AST.
/// Based loosely off Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8 
///
#include "token.hpp"
#include "ast.hpp"

//Default constructor
//
//std::string type_: The type of this AST node
//
//std::string attr_: The attribute of this AST node (if applicable)
//
//int where: The line number of this node
//
//std::vector<Ast> children: a public list of all children of this node
//
Ast::Ast(std::string type_, std::string attr_, int where_){
    type = type_;
    attr = attr_;
    where = where_;
}

//Alternate constructor
Ast::Ast(std::string type_){
    type = type_;
}