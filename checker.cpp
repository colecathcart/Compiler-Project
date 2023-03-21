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
    funccheck();
    breakcheck(ast_);
    return ast_;
}

//Function that handles semantic checking for
//main-related errors
void Checker::maincheck(){
    int hasmain = 0;
    bool ismainfunc = false;
    for(auto i : ast_.children){
        if(i.type == "func"){
            for(auto j : i.children){
                if(j.type == "newid" && j.attr == "main"){
                    hasmain++;
                    ismainfunc = true;
                    returncheck(i,false,"");
                }
                if(j.type == "signature" && ismainfunc){
                    ismainfunc = false;
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

//Function that handles semantic checking for
//function-related errors
void Checker::funccheck(){
    for(auto i : ast_.children){
        if(i.type == "func"){

            //TODO: Handle function signature checking

            if(i.children[1].children[1].attr == "void"){
                returncheck(i, false, "");
            } else {
                int a = returncheck(i, true, i.children[1].children[1].attr);
                if(a < 1){
                    logger->error("Function with return type never returns");
                }
            }
        }
    }
}

//Function for recursively checking return values inside a function. The 
//second argument is false if return statements should be void, in
//which case the third argument is ignored. Returns the number of returns found
//in "func"
int Checker::returncheck(Ast &func, bool returnsval, std::string type){
    int hasreturn = 0;
    for(auto i : func.children){
        if(i.type == "return"){
            hasreturn++;
            if(returnsval){
                bool correctretval = false;
                for(auto j : i.children){

                    //TODO: May need to handle expressions

                    if(j.type != type && j.type != ""){
                        logger->error("Return type does not match function signature",j.where);
                    } else if(j.type == type){
                        correctretval = true;
                    }
                }
                if(!correctretval){
                    logger->error("Return type does not match function signature",i.where);
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
    if(returnsval){
        return hasreturn;
    } else {
        return 1;
    }
}

//Function for recursively checking for break statements
//outside of 'for' loops. Assumes the first AST called on
//is not itself a 'for' loop
void Checker::breakcheck(Ast &tree){
    for(auto i : tree.children){
        if(i.type == "break"){
            logger->error("Break statement found outside of for-loop",i.where);
        } else if(i.type != "for"){
            breakcheck(i);
        }
    }
}