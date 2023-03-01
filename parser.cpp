#include "scanner.hpp"
#include "token.hpp"
#include "ast.hpp"
#include "parser.hpp"

Parser::Parser(Scanner &scanner) : scanner_{ scanner }{
    logger = Logger::getLogger();
}

Token Parser::expect(std::string expected){
    Token next = scanner_.lex();
    if(next.type != expected){
        std::string errmsg = "Expected " + expected + ", got " + next.type ;
        logger->error(errmsg, next.line);
    }
    return next;
}

bool Parser::is_declaration_start(Token s){
    if(s.type == "KEYWORD" && (s.attr == "var"|| s.attr == "func")){
        return true;
    }
    return false;
}

bool Parser::is_statement_start(Token s){
    if(s.type == "KEYWORD" && (
        s.attr == "break" || 
        s.attr == "return" || 
        s.attr == "if" ||
        s.attr == "for" ||
        s.attr == "var" 
        )){
        return true;
    } //unfinished
    return true;
}

Ast Parser::var(){
    Token id = expect("ID");
    Token type = expect("ID");

    if(type.attr != "int" && type.attr != "bool" && type.attr != "string"){
        std::string errmsg = "Expected int, bool, or string, got " + type.attr;
        logger->error(errmsg, type.line);
    }

    Ast var = Ast("var","",id.line);
    var.children.push_back(Ast{"newid",id.attr,id.line});
    var.children.push_back(Ast{"typeid",type.attr,type.line});
    return var;
}

Ast Parser::func(){

}

Ast Parser::parse(){
    Ast ast = declarations();
    return ast;
}

Ast Parser::declarations(){
    Ast ast = Ast("program");
    while(is_declaration_start(scanner_.lex())){
        scanner_.unlex();
        Ast child = declaration();
        ast.children.push_back(child);
    }
    scanner_.unlex();
    return ast;
}

Ast Parser::declaration(){
    Token s = scanner_.lex();
    if(s.attr == "var"){
        return var();
    } else if(s.attr == "func"){
        return func();
    }
}
    
Ast Parser::statements(){
    Ast ast = Ast("program");
    while(is_statement_start(scanner_.lex())){
        scanner_.unlex();
        Ast child = statement();
        ast.children.push_back(child);
    }
    scanner_.unlex();
    return ast;
}
Ast Parser::statement(){

}
Ast Parser::expression(){

}