/// CPSC 411 Term Project: Scanner Class
/// Cole Cathcart, 30043556
/// Feb 28, 2023
///
/// This is the checker class which contains all the functionality
/// of the compiler's semantic checker. Its methods are called by main.cpp.
/// The implementation is based off  Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8 
///

#include "ast.hpp"
#include "token.hpp"
#include "checker.hpp"
#include "logger.hpp"

//Default constructor
//
//Ast ast: The AST to perform semantic checking and editing on
//
Checker::Checker(Ast &ast) : ast_{ast}{
    logger = Logger::getLogger();
}

//Top-level function called externally to perform semantic checking on
//the entire AST. Traverses the AST several times. Each sub-function called
//will throw errors directly and returns no value
Ast Checker::check(){
    maincheck();
    return ast_;
}

//Function that handles semantic checking 
//for main-related errors
void Checker::maincheck(){
    int hasmain = 0;
    for(auto i : ast_.children){
        if(i.type == "func"){
            for(auto j : i.children){
                if(j.type == "newid" && j.attr == "main"){
                    hasmain++;
                    returncheck(i,false,"");
                }
                if(j.type == "signature"){
                    if(j.children[0].children.size() > 0){
                        logger->error("Main function cannot have arguments");
                    }
                    if(j.children[1].attr != "void"){
                        logger->error("Main function cannot have a return type");
                    }
                }
            }
        }
    }
    if(hasmain == 0){
        logger->error("Missing main function");
    } else if(hasmain > 1){
        logger->error("Multiple main functions declared");
    }
}

//Function for recursively checking return values inside a function. The 
//second argument is false if return statements should be void, in
//which case the third argument is ignored
void Checker::returncheck(Ast &func, bool returnsval, std::string type){
    for(auto i : func.children){
        if(i.type == "return"){
            if(returnsval){
                for(auto j : i.children){
                    if(j.type != type){
                        logger->error("Return type does not match function signature",j.where);
                    }
                }
            } else {
                 for(auto j : i.children){
                    if(j.type != ""){
                        logger->error("Non-void return in void function",j.where);
                    }
                }
            }
        } else {
            returncheck(i, returnsval, type);
        }
    }
}