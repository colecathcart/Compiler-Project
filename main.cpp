/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///
/// This is the main method which acts as the driver of the compiler
/// program. Currently its only function is to call the method 'parse()' from
/// parser.cpp and print a textual representation of the AST to standart output.
/// The implementation is based off Dr. Aycock's example code 
/// in python, shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8
///
/// This implementation is written purely in C++, without the use
/// of tools (lex, flex, etc)
///
/// FOR MILESTONE 2:
///     The main function prints the contents of each token to standard output.
///     the syntax is: <node type> [<node attribute, if applicable>] @ line <line#, is applicable>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "scanner.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "checker.hpp"

//utility function to print out ast for ms3
void astprinter(Ast &ast, std::string tab){
    std::string ast_str = tab + ast.type;
    if (!ast.attr.empty()){
        ast_str += " [" + ast.attr + "]";
    }

    if (!ast.sig.empty()){
        ast_str += " sig = " + ast.sig;
    }
    if (ast.where > 0){
        ast_str += " @ line " + std::to_string(ast.where);
    }
    printf("%s\n",ast_str.c_str());
    for(auto child : ast.children){
        astprinter(child, tab + "\t");
    }
}

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
    /*
    Scanner scanner = Scanner(file);
    Token returntoken = scanner.lex();
    while(returntoken.type != "EOF"){
        printf("Token: type=[%s], attr=[%s], at line %d\n",returntoken.type.c_str(), returntoken.attr.c_str(), returntoken.line);
        returntoken = scanner.lex();
        
    }
    */

    //for testing of parser
    /*
    Scanner scanner = Scanner(file);
    Parser parser = Parser(scanner);
    Ast ast = parser.parse();
    astprinter(ast, "");
    */

    //for testing of semantic checker
    Scanner scanner = Scanner(file);
    Parser parser = Parser(scanner);
    Ast ast = parser.parse();
    Checker checker = Checker(ast);
    ast = checker.check();
    astprinter(ast, "");
    file.close();

}