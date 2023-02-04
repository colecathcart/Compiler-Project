/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///
/// This is the main method which acts as the driver of the compiler
/// program. Currently its only function is to call the methods from
/// scanner.cpp and print the tokens to standart output.
/// The implementation is based off Dr. Aycock's example code 
/// in python, shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page.
///
/// This implementation is written purely in C++, without the use
/// of tools (lex, flex, etc)

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "scanner.hpp"
#include "logger.hpp"

int main(int argc, char* argv[]){

    Logger* logger = Logger::getLogger();

    if(argc < 2){
        logger->error("Need to specify 1 command-line arg for input file");
        return 1;
    }

    if(argc > 2){
        logger->error("Too many command-line args (need 1 for input file)");
        return 1;
    }

    std::ifstream file(argv[1]);

    if(!file){
        logger->error("Could not find or open specified file");
        return 1;
    }

    //for testing of scanner
    Scanner scanner = Scanner(file);
    Token returntoken = scanner.lex();
    while(returntoken.type != "EOF"){
        printf("Token: type=[%s], attr=[%s], at line %d\n",returntoken.type.c_str(), returntoken.attr.c_str(), returntoken.line);
        returntoken = scanner.lex();
        
    }

    file.close();

}