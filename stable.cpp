/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// March 21, 2023
/// Class to represent and contain information about symbol table
/// Based loosely off Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8 
///
#include "stable.hpp"
#include "logger.hpp"
#include "symbol.hpp"
#include "ast.hpp"
#include <string>
#include <map>

//Deafult constructor
Stable::Stable(){
    logger = Logger::getLogger();
}

//Function to insert a new variable into the symbol table.
//Handles errors relating to variable reassignment.
void Stable::declare(Ast &t, Symbol &sym){
    if(symboltable.find(t.attr) != symboltable.end()){
        logger->error("Variable in-scope reassigned",t.where);
    } else {
        symboltable[t.attr] = sym;
    }
}

//Fnction to lookup a symbol in the table. Handles
//errors relating to nonexistent variable/function calls.
Symbol Stable::lookup(Ast &t){
    if(symboltable.find(t.attr) == symboltable.end()){
        logger->error("Variable or function called before assignment",t.where);
    }
    return symboltable[t.attr];
    
}