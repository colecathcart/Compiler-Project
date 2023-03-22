/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// March 21, 2023
/// Class to represent and contain information about a symbol for
/// use in a symbol table. Based loosely off Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8 
///
#include "symbol.hpp"
#include <string>
#include <vector>

//Deafult constructor
//
//std::string name: the name of the symbol, either the type or attribute of its corresponding AST node
//
//std::string sig: the type signature of the sybol
//
//std::vector<std::string> formals: An optionally-empty list of formals signatures for functions
//
Symbol::Symbol(std::string name_, std::string sig_, std::vector<std::string> formals_){
    name = name_;
    sig = sig_;
    formals = formals_;
}

Symbol::Symbol(std::string name_, std::string sig_){
    name = name_;
    sig = sig_;
}