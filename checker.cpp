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
#include "symbol.hpp"
#include "stable.hpp"

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
    Stable s_table = Stable();
    signaturize(ast_, s_table, 0);
    maincheck();
    funccheck();
    breakcheck(ast_);
    const_and_rangecheck(ast_);
    //call assignment checker first

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

//Function for recursively checking for assignments
//to const values. Assumes that the first AST called on
//is not an assignment
void Checker::const_and_rangecheck(Ast &tree){
    for(auto &i : tree.children){
        if(i.type == "="){
            if(i.children[0].type == "INTEGER" || 
                i.children[0].type == "STRING"  ||
                (i.children[0].type == "ID" && (i.children[0].attr == "true" || i.children[0].attr == "false"))){
                logger->error("Can't assign to a constant",i.where);
            }
            if(i.children[0].type == "ID"){
                if(i.children[1].type == "INTEGER"){
                    std::string num = i.children[1].attr;
                    if(num.size() > 10){
                        logger->error("Integer literal out of range",i.where);
                    } else {
                        long numb = std::stol(num);
                        if(numb > 2147483647){
                            logger->error("Integer literal out of range",i.where);
                        }
                    }
                }else if(i.children[1].type == "u-"){
                    std::string num = i.children[1].children[0].attr;
                    if(num.size() > 10){
                        logger->error("Integer literal out of range",i.where);
                    } else {
                        long numb = std::stol(num);
                        if(numb > 2147483648){
                            logger->error("Integer literal out of range",i.where);
                        }
                    }
                }
            } else {
                logger->error("Can only assign to a variable",i.where);
            }
        }
        const_and_rangecheck(i);
    }
}

//Function to add signatures to all applicable nodes and manage the symbol table
std::string Checker::signaturize(Ast &tree, Stable &table, int level){
    if(tree.sig.empty()){
        if(tree.type == "INTEGER"){
            tree.sig = "int";
        }else if(tree.type == "STRING"){
            tree.sig = "string";
        }else if(tree.type == "u-" || tree.type == "!"){
            tree.sig = signaturize(tree.children[0],table,level);
        }else if(tree.type == "ID" && (tree.attr == "true" || tree.attr == "false")){
            tree.sig = "bool";
        }else if(tree.type == "var"){
            tree.sig = tree.children[1].attr;
            if(!ispredeclared(tree.sig)){
                table.lookup(tree.children[1]);
            }
            Symbol s = Symbol(tree.children[0].attr, tree.sig, level);
            table.declare(tree,s,level);
            table.lookup(tree.children[0]);
        }else if(tree.type == "="){
            tree.sig = signaturize(tree.children[0],table,level);
            std::string check = signaturize(tree.children[1],table,level);
            if(tree.sig != check){
                logger->error("Assignment type mismatch",tree.where);
            }
        }else if(tree.type == "ID"){
            if(!ispredeclared(tree.attr)){
                Symbol s = table.lookup(tree);
                tree.sig = s.sig;
            }
        }else if(tree.type == "==" || tree.type == "!=" || tree.type == "<=" || tree.type == ">=" ||
                    tree.type == "<" || tree.type == ">" || tree.type == "&&" || tree.type == "||"){
            tree.sig = "bool";
            std::string check1 = signaturize(tree.children[0],table,level);
            std::string check2 = signaturize(tree.children[1],table,level);
            if(check1 != check2){
                logger->error("Boolean comparison type mismatch",tree.where);
            }
        }else if(tree.type == "+" || tree.type == "-" || tree.type == "*" || tree.type == "/" || tree.type == "%"){
            std::string check1 = signaturize(tree.children[0],table,level);
            std::string check2 = signaturize(tree.children[1],table,level);
            if(check1 != check2){
                logger->error("Arithmetic type mismatch",tree.where);
            }
            tree.sig = check1;
        }
    }
    for(auto &i : tree.children){
        Stable s = table;
        signaturize(i,s,level + 1);
    }
    return tree.sig;
}

bool Checker::ispredeclared(std::string type){
    if(type == "true" || type == "false" ||
        type == "bool" || type == "int" ||
        type == "string" || type == "getchar" ||
        type == "halt" || type == "len" ||
        type == "printb" || type == "printc" ||
        type == "printi" || type == "prints"){
            return true;
        }
    return false;
}