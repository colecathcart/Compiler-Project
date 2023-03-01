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
    } else if(s.type == "{" ||
        s.type == ";" ||
        s.type == "ID" ||
        s.type == "INTEGER" ||
        s.type == "STRING"){
            return true;
    }
    return false;
}

Ast Parser::var(){
    Token id = expect("ID");
    Token type = expect("ID");
    expect(";");

    Ast var = Ast("var","",id.line);
    var.children.push_back(Ast{"newid",id.attr,id.line});
    var.children.push_back(Ast{"typeid",type.attr,type.line});
    return var;
}

Ast Parser::formal(Token &id, Token &type){
    Ast formal = Ast("formal");
    formal.children.push_back(Ast{"newid",id.attr,id.line});
    formal.children.push_back(Ast{"typeid",type.attr,type.line});
    return formal;
}

Ast Parser::sig(Ast &formals, Token &returntype){
    Ast sig = Ast("signature");
    sig.children.push_back(formals);
    sig.children.push_back(Ast{"typeid",returntype.attr,returntype.line});
    return sig;
}

Ast Parser::block(){
    Ast block = Ast("block");
    Token next = scanner_.lex();
    while(is_statement_start(next)){
        scanner_.unlex();
        block.children.push_back(statement());
        next = scanner_.lex();
    }
    if(next.type != "}"){
        logger->error("Missing closing bracket",next.line);
    }
    expect(";");
    return block;
}

Ast Parser::func(){
    Ast func = Ast("func");
    Token id = expect("ID");
    func.children.push_back(Ast{"newid",id.attr,id.line});
    expect("(");
    Ast formals = Ast("formals");
    Token next = scanner_.lex();
    while(next.type == "ID") {
        Token type = expect("ID");
        formals.children.push_back(formal(next, type));
        next = scanner_.lex();
        if(next.type != ","){
            break;
        }
        next = scanner_.lex();
        if(next.type != "ID" && next.type != ";" && next.type != ")"){
            logger->error("Missing formal", next.line);
        }
    }
    if(next.type == ";"){
        next = scanner_.lex();
    }
    if(next.type != ")"){
        logger->error("Improper function signature", next.line);
    }
    next = scanner_.lex();
    Token returntype = Token("void","void",-1);
    if(next.type == "ID"){
        returntype = next;
        next = scanner_.lex();
    }
    func.children.push_back(sig(formals, returntype));
    if(next.type != "{"){
        //std::string errmsg = "Expected '{', got " + next.type;
        logger->error("Unexpected newline", next.line);
    }
    func.children.push_back(block());
    return func;
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
    //should never reach
    return Ast("","",0);
}

Ast Parser::ifstmt(){
    return Ast{"if"};
}

Ast Parser::forstmt(){
    return Ast{"for"};
}
    
Ast Parser::statement(){
    Token next = scanner_.lex();
    if(next.type == "KEYWORD"){
        if(next.attr == "break"){
            expect(";");
            return Ast{"break","",next.line};
        } else if(next.attr == "return"){
            Ast retstmt = Ast("return","",next.line);
            next = scanner_.lex();
            if(is_expression_start(next)){
                retstmt.children.push_back(expression());
            }
            expect(";");
            return retstmt;
        } else if(next.attr == "if"){
            return ifstmt();
        } else if(next.attr == "for"){
            return forstmt();
        }
    } else if(next.type == ";"){
        return Ast{"emptystmt"};
    } else if(next.type == "{"){
        return block();
    } else if(is_expression_start(next)){
        return expression();
    }
    return Ast{""};
}

bool Parser::is_expression_start(Token s){
    return true;
}

Ast Parser::expression(){
    return Ast{"expr"};
}