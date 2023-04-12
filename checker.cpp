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
    handle_predeclared(s_table);
    setglobals(s_table);
    signaturize(ast_, s_table, 0);
    maincheck();
    funccheck();
    breakcheck(ast_);
    const_and_rangecheck(ast_);
    if_for_check(ast_);
    paramcheck(ast_,s_table);

    return ast_;
}

//Function for setting global variables and 
void Checker::setglobals(Stable &table){
    for(auto &i : ast_.children){
        if(i.type == "var"){
            i.sig = i.children[1].attr;
            if(i.sig != "bool" && i.sig != "int" && i.sig != "string"){
                logger->error("Cannot declare a variable type as a non-type",i.where);
            }
            Symbol s = Symbol(i.children[0].attr, i.sig, 1);
            table.declare(i.children[0],s,1);
        } else if(i.type == "func"){
            std::string name = i.children[0].attr;
            std::string sig = i.children[1].children[1].attr;
            std::vector<std::string> formals;
            for(auto &i : i.children[1].children[0].children){
                formals.push_back(i.children[1].attr);
            }
            if(formals.size() < 1){
                formals.push_back("");
            }
            Symbol s = Symbol(name, sig, 1, formals);
            table.declare(i,s,1);
            funcmap.insert({s.name,s});
        }
    }
}

//Function responsible for ensuring function calls are made with proper arguments
void Checker::paramcheck(Ast &tree, Stable &table){
    for(auto &func : tree.children){ 
        if(func.children.size() > 0 && func.children[0].type == "arguments"){  
            if(func.type != "ID"){
                logger->error("Cant call something that isn't a function",func.where);
            }
            //printf("Node attr: %s\n",func.attr.c_str());
            Symbol s = funcmap[func.attr];
            if(func.children[0].children.size() != s.formals.size() && !(func.children[0].children.size() == 0 || s.formals[0] == "")){
                logger->error("Improper number of arguments in function call",func.where);
            }
            for(size_t i = 0; i < func.children[0].children.size(); i++){
                if(func.children[0].children[i].sig != s.formals[i]){
                    logger->error("Incorrect argument types in function call",func.where);
                }
            }
        }
        paramcheck(func, table);
    }
}

//Function responsible for adding all predeclared functions and types into the symbol table
void Checker::handle_predeclared(Stable &s_table){
    Symbol int_ = Symbol("int","int",-1);
    Symbol bool_ = Symbol("bool","bool",-1);
    Symbol string_ = Symbol("string","string",-1);
    Symbol true_ = Symbol("true", "bool", -1);
    Symbol false_ = Symbol("false", "bool", -1);
    Symbol getchar_ = Symbol("getchar", "int", -1);
    Symbol halt_ = Symbol("halt", "void", -1);
    Symbol len_ = Symbol("len", "int", -1, std::vector<std::string>{"string"});
    Symbol printb_ = Symbol("printb", "void", -1, std::vector<std::string>{"bool"});
    Symbol printi_ = Symbol("printi", "void", -1, std::vector<std::string>{"int"});
    Symbol printc_ = Symbol("printc", "void", -1, std::vector<std::string>{"int"});
    Symbol prints_ = Symbol("prints", "void", -1, std::vector<std::string>{"string"});
    
    s_table.declare(ast_,int_,-1);
    s_table.declare(ast_,bool_,-1);
    s_table.declare(ast_,string_,-1);
    s_table.declare(ast_,true_,-1);
    s_table.declare(ast_,false_,-1);
    s_table.declare(ast_,getchar_,-1);
    s_table.declare(ast_,halt_,-1);
    s_table.declare(ast_,len_,-1);
    s_table.declare(ast_,printb_,-1);
    s_table.declare(ast_,printi_,-1);
    s_table.declare(ast_,printc_,-1);
    s_table.declare(ast_,prints_,-1);

    funcmap.insert({getchar_.name,getchar_});
    funcmap.insert({halt_.name,halt_});
    funcmap.insert({len_.name,len_});
    funcmap.insert({printb_.name,printb_});
    funcmap.insert({printi_.name,printi_});
    funcmap.insert({printc_.name,printc_});
    funcmap.insert({prints_.name,prints_});
}

//Function that handles if and for loop expression checking
void Checker::if_for_check(Ast &tree){
    for(auto i : tree.children){
        if(i.type == "if" || i.type == "for"){
            if(i.children[0].sig != "bool"){
                logger->error("If and for loops must have a boolean expression type",i.where);
            }
        }
        if_for_check(i);
    }
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

                    if(j.sig != type && j.type != ""){
                        logger->error("Return type does not match function signature",j.where);
                    } else if(j.sig == type){
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
            hasreturn += returncheck(i, returnsval, type);
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
    //table.print_table();
    if(tree.sig.empty()){
        if(tree.type == "INTEGER"){
            tree.sig = "int";
        }else if(tree.type == "STRING"){
            tree.sig = "string";
        }else if(tree.type == "u-" || tree.type == "!"){
            tree.sig = signaturize(tree.children[0],table,level);
        }else if(tree.type == "expr_operand"){
            tree.sig = signaturize(tree.children[0],table,level);
        }else if(tree.type == "var" && level > 1){
            //printf("in VAR\n");
            tree.sig = tree.children[1].attr;
            if(tree.sig != "bool" && tree.sig != "int" && tree.sig != "string"){
                logger->error("Cannot declare a variable type as a non-type",tree.where);
            }
            Symbol s = Symbol(tree.children[0].attr, tree.sig, level);
            table.declare(tree.children[0],s,level);
        }else if(tree.type == "="){
            if(tree.children[0].children.size() > 0 && tree.children[0].children[0].type == "arguments"){
                logger->error("Cannot assign to a non-variable",tree.where);
            }
            tree.sig = signaturize(tree.children[0],table,level);
            std::string check = signaturize(tree.children[1],table,level);
            if(tree.sig != check){
                logger->error("Assignment type mismatch",tree.where);
            }
            if(tree.children[1].attr == "int" || tree.children[1].attr == "bool" || tree.children[1].attr == "string"){
                logger->error("Cant use a type here",tree.where);
            }
            //table.print_table();
            Symbol s = table.lookup(tree.children[0]);
            //printf("Symbol name: %s\n",s.name.c_str());
            if(s.formals.size() > 0){
                logger->error("Cannot assign to a non-variable");
            }
        }else if(tree.type == "ID"){
            //printf("in ID\n");
            //printf("tree attr: %s\n",tree.attr.c_str());
            
            Symbol s = table.lookup(tree);
            tree.sig = s.sig;
        }else if(tree.type == "func" && level > 1){
            std::string name = tree.children[0].attr;
            std::string sig = tree.children[1].children[1].attr;
            std::vector<std::string> formals;
            for(auto &i : tree.children[1].children[0].children){
                formals.push_back(i.children[1].attr);
            }
            if(formals.size() < 1){
                formals.push_back("");
            }
            Symbol s = Symbol(name, sig, level, formals);
            table.declare(tree,s,level);
            funcmap.insert({s.name,s});
        } else if(tree.type == "signature"){
            for(auto &i : tree.children[0].children){
                Symbol s = Symbol(i.children[0].attr, i.children[1].attr, level);
                if(s.sig != "bool" && s.sig != "int" && s.sig != "string"){
                    logger->error("Cannot declare a function parameter type as a non-type",i.where);
                }
                table.declare(tree, s, level);
            }
        }else if(tree.type == "==" || tree.type == "!=" || tree.type == "&&" || tree.type == "||"){
            tree.sig = "bool";
            std::string check1 = signaturize(tree.children[0],table,level);
            std::string check2 = signaturize(tree.children[1],table,level);
            if(check1 != check2){
                logger->error("Boolean comparison type mismatch",tree.where);
            }
        }else if(tree.type == "<" || tree.type == ">" || tree.type == ">=" || tree.type == "<="){
            tree.sig = "bool";
            std::string check1 = signaturize(tree.children[0],table,level);
            std::string check2 = signaturize(tree.children[1],table,level);
            if(check1 != check2){
                logger->error("Boolean comparison type mismatch",tree.where);
            }
            if(check1 == "bool"){
                logger->error("Comparison of non-ordered type",tree.where);
            }
        }else if(tree.type == "+" || tree.type == "-" || tree.type == "*" || tree.type == "/" || tree.type == "%"){
            std::string check1 = signaturize(tree.children[0],table,level);
            std::string check2 = signaturize(tree.children[1],table,level);
            if(check1 != check2){
                logger->error("Arithmetic type mismatch",tree.where);
            }
            if(check1 == "bool"){
                logger->error("Cannot perform arithmetic on boolean types",tree.where);
            }
            tree.sig = check1;
        }
    }
    // Make copy of table for children to share
    Stable s = table;
    for(auto &i : tree.children){
        //s.print_table();
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