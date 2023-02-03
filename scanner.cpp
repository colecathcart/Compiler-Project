/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///

#include "token.hpp"
#include "scanner.hpp"

//Default constructor
Scanner::Scanner() : reread{ false }, lastoken{ "", "", 0 }
{
}

//Main function of the scanner. Finds the next character from
//the file and performs operations to detect if it is a legal
//token (returns that token), a warning (prints to standard error), 
//or an error (prints to standard error and exits)
Token Scanner::lex(){
    if(reread == true){
        reread = false;
        return lastoken;
    }

    return lastoken;
}

//Returns the previous token
void Scanner::unlex(){
    reread = true;
}