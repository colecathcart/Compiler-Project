/// CPSC 411 Term Project: Scanner Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///
/// This is the scanner class which contains all the functionality
/// of the compiler scanner. Its methods are called by main.cpp.
/// The implementation is based off  Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page.
///

#include "token.hpp"
#include "scanner.hpp"
#include "logger.hpp"

//Default constructor
//
//std::ifstream &f: the file to be read from
//
Scanner::Scanner(std::ifstream &f) : reread{false}, lastoken{"","",0}, file{f} 
{
    logger = Logger::getLogger();
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