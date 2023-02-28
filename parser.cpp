#include "scanner.hpp"
#include "token.hpp"
#include "ast.hpp"
#include "parser.hpp"

Parser::Parser(Scanner &scanner) : scanner_{ scanner }{
}

bool Parser::is_statement_start(Token s){
    //TODO
    return true;
}

Ast Parser::parse(){
    Ast ast = statements();
    return ast;
}
    
Ast Parser::statements(){
    Ast ast = Ast("stmts");
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