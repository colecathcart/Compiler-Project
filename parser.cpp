/// CPSC 411 Term Project: Scanner Class
/// Cole Cathcart, 30043556
/// Feb 28, 2023
///
/// This is the parser class which contains all the functionality
/// of the compiler parser. Its methods are called by main.cpp.
/// The implementation is based off  Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8 
///

#include "scanner.hpp"
#include "token.hpp"
#include "ast.hpp"
#include "parser.hpp"

//Default constructor
//
//Scanner scanner: The scanner to use for getting tokens
//
Parser::Parser(Scanner &scanner) : scanner_{ scanner }{
    logger = Logger::getLogger();
}

//A method that automatically handles both lexing and checking that the lexed token
//matches the expected value. Exits on an error if the net token was unexpected
Token Parser::expect(std::string expected){
    Token next = scanner_.lex();
    if(next.type != expected){
        if(next.type == ";" && next.attr == ""){
            logger->error("Unexpected newline", next.line);
        }
        std::string errmsg = "Expected " + expected + ", got " + next.type ;
        logger->error(errmsg, next.line);
    }
    return next;
}

//Helper function to determine if a token is a declaration start
bool Parser::is_declaration_start(Token s){
    if(s.type == "KEYWORD" && (s.attr == "var"|| s.attr == "func")){
        return true;
    }
    return false;
}

//Helper function to determine if  token is a statement start
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
    } else if(is_expression_start(s)){
        return true;
    }
    return false;
}

//Handler for var declarations. Assumes the "var" keyword was previously found
Ast Parser::var(){
    Token id = expect("ID");
    Token type = expect("ID");
    //expect(";");

    Ast var = Ast("var","",id.line);
    var.children.push_back(Ast{"newid",id.attr,id.line});
    var.children.push_back(Ast{"typeid",type.attr,type.line});
    return var;
}

//Handler for function parameters, AKA formals
Ast Parser::formal(Token &id, Token &type){
    Ast formal = Ast("formal");
    formal.children.push_back(Ast{"newid",id.attr,id.line});
    formal.children.push_back(Ast{"typeid",type.attr,type.line});
    return formal;
}

//Handler for function signature
Ast Parser::sig(Ast &formals, Token &returntype){
    Ast sig = Ast("signature");
    sig.children.push_back(formals);
    sig.children.push_back(Ast{"typeid",returntype.attr,returntype.line});
    return sig;
}

//Handler for blocks. Assumes the '{' open-brace was previously found
Ast Parser::block(){
    Ast block = Ast("block");
    Token next = scanner_.lex();
    if(next.type == ";"){
        Token t = scanner_.lex();
        if(t.type == "}"){
            block.children.push_back(Ast{"emptystmt"});
            return block;
        } else {
            scanner_.unlex();
        }
    }
    while(is_statement_start(next)){
        scanner_.unlex();
        Ast s = statement();
        //printf("Node: %s\n",s.type.c_str());
        block.children.push_back(s);
        //expect(";");
        next = scanner_.lex();
    }
    if(next.type != "}"){
        //logger->error("Missing closing bracket",next.line);
        std::string errmsg = "Expected }, got " + next.type ;
        logger->error(errmsg, next.line);
    }
    //expect(";");
    return block;
}

//Handler for function declarations
Ast Parser::func(){
    Token id = expect("ID");
    Ast func = Ast("func", "", id.line);
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
        //next = scanner_.lex();
        logger->error("Unexpected newline", next.line);
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

//Top-level function called to parse the entire input file
Ast Parser::parse(){
    Ast ast = declarations();
    return ast;
}

//Handler for top-level declarations
Ast Parser::declarations(){
    Ast ast = Ast("program");
    while(is_declaration_start(scanner_.lex())){
        scanner_.unlex();
        Ast child = declaration();
        ast.children.push_back(child);
        if(scanner_.lex().type != ";"){
            scanner_.unlex();
        }
    }
    scanner_.unlex();
    return ast;
}

//Handler for a single declaration
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

//Handler for if statements. Assumes the "if" keyword was previously found
Ast Parser::ifstmt(){
    Ast expr = expression();
    Ast if_ = Ast("if","",expr.where);
    if_.children.push_back(expr);
    expect("{");
    if_.children.push_back(block());
    Token t = scanner_.lex();
    if(t.attr == "else"){
        Ast els = Ast("else","",t.line);
        t = scanner_.lex();
        if(t.attr == "if"){
            els.children.push_back(ifstmt());
        } else {
            scanner_.unlex();
            expect("{");
            els.children.push_back(block());
        }
        if_.children.push_back(els);
    } else {
        scanner_.unlex();
    }
    return if_;
}

////Handler for for statements. Assumes the "for" keyword was previously found
Ast Parser::forstmt(){
    Token t = scanner_.lex();
    Ast for_ = Ast("for","",t.line);
    if(t.type != "{"){
        scanner_.unlex();
        Ast condition = expression();
        for_.children.push_back(condition);
    } else {
        scanner_.unlex();
        for_.children.push_back(Ast{"ID","true",t.line});
    }
    expect("{");
    Ast block_ = block();
    for_.children.push_back(block_);
    return for_;
}

//Handler for a single statement
Ast Parser::statement(){
    Token next = scanner_.lex();
    if(next.type == "KEYWORD"){
        if(next.attr == "break"){
            //expect(";");
            return Ast{"break","",next.line};
        } else if(next.attr == "return"){
            Ast retstmt = Ast("return","",next.line);
            next = scanner_.lex();
            if(is_expression_start(next)){
                scanner_.unlex();
                Ast e = expression();
                retstmt.children.push_back(e);
            } else {
                scanner_.unlex();
            }
            //expect(";");
            return retstmt;
        } else if(next.attr == "if"){
            return ifstmt();
        } else if(next.attr == "for"){
            return forstmt();
        } else if(next.attr == "var"){
            return var();
        }
    } else if(next.type == ";" && next.attr  == ";"){
        //scanner_.unlex();
        return Ast{"emptystmt"};
    } else if(next.type == "{"){
        return block();
    } else if(is_expression_start(next)){
        scanner_.unlex();
        Ast expr = expression();
        Token t = scanner_.lex();
        if(t.type == "="){
            Ast assign = Ast("=","",t.line);
            assign.children.push_back(expr);
            assign.children.push_back(expression());
            return assign;
        } else {
            scanner_.unlex();
            return expr;
        }
    }
    return Ast{""};
}

//Helper function to determine if this token is an expression start
bool Parser::is_expression_start(Token s){
    if(s.attr == "!" || s.attr == "-" || s.attr == "("){
        return true;
    } else if (s.type == "ID" || s.type == "INTEGER" || s.type == "STRING"){
        return true;
    }
    return false;
}

//Handler for a single expression not including binary-op expressions
Ast Parser::singlexpr(){
    Token t = scanner_.lex();
    Ast expr = Ast("");
    if(t.attr == "-" || t.attr == "!"){
        Ast unaryop = Ast("u"+t.attr, "", t.line);
        unaryop.children.push_back(expression());
        expr = unaryop;
    } else if(t.attr == "("){
        Ast exrop = Ast{"expr_operand","",t.line};
        Ast operand = expression();
        exrop.children.push_back(operand);
        expect(")");
        t = scanner_.lex();
        if(t.type == "("){
            t = scanner_.lex();
            Ast args{"arguments"};
            while(is_expression_start(t)){
                scanner_.unlex();
                Ast argument = expression();
                args.children.push_back(argument);
                t = scanner_.lex();
                if(t.attr == ","){
                    t = scanner_.lex();
                }
            }
            scanner_.unlex();
            expect(")");
            exrop.children.push_back(args);
        } else {
            scanner_.unlex();
        }
        expr = exrop;
    } else if(t.type == "ID"){
        Ast operand = Ast("ID", t.attr, t.line);
        t = scanner_.lex();
        if(t.attr == "("){
            t = scanner_.lex();
            Ast args{"arguments"};
            while(is_expression_start(t)){
                scanner_.unlex();
                Ast argument = expression();
                args.children.push_back(argument);
                t = scanner_.lex();
                if(t.attr == ","){
                    t = scanner_.lex();
                }
            }
            scanner_.unlex();
            expect(")");
            operand.children.push_back(args);
        } else {
            scanner_.unlex();
        }
        expr = operand;
    } else if(t.type == "INTEGER" || t.type == "STRING"){
        Ast operand = Ast(t.type, t.attr, t.line);
        t = scanner_.lex();
        if(t.attr == "("){
            t = scanner_.lex();
            Ast args{"arguments"};
            while(is_expression_start(t)){
                scanner_.unlex();
                Ast argument = expression();
                args.children.push_back(argument);
                t = scanner_.lex();
                if(t.attr == ","){
                    t = scanner_.lex();
                }
            }
            scanner_.unlex();
            expect(")");
            operand.children.push_back(args);
        } else {
            scanner_.unlex();
        }
        expr = operand;
    }
    return expr;
}

//Handler for expressions including binary-op expressions
Ast Parser::expression(){
    Ast expr = singlexpr();
    Token t = scanner_.lex();
    std::vector<Ast> nodes;
    while(isbinoperator(t.type)){
        nodes.push_back(expr);
        nodes.push_back(Ast{t.type, t.attr, t.line});
        expr = singlexpr();
        t = scanner_.lex();
    }
    nodes.push_back(expr);
    scanner_.unlex();
    return expressiontree(nodes);
}

//Helper function that returns an Ast with proper 
//precedence of operators in a binary-op expression
Ast Parser::expressiontree(std::vector<Ast> nodes){
    //printf("Size: %ld\n",nodes.size());
    
    if(nodes.size() < 1){
        return Ast{""};
    }
    if(nodes.size() < 2){
        return nodes[0];
    }
    int lowest_p = 0;
    for(int i = 0; i < (int)nodes.size(); i++){
        if(precedence(nodes[i].type) <= precedence(nodes[lowest_p].type)){
            lowest_p = i;
        }
    }
    Ast top = Ast(nodes[lowest_p].type, nodes[lowest_p].attr, nodes[lowest_p].where);
    std::vector<Ast> leftree = {nodes.begin(), nodes.begin()+(lowest_p)};
    std::vector<Ast> rightree = {nodes.begin() + (lowest_p + 1), nodes.end()};
    top.children.push_back(expressiontree(leftree));
    top.children.push_back(expressiontree(rightree));
    return top;
}

//Helper function to determine if this string is a binary operator
int Parser::isbinoperator(std::string op){
    if(op == "*" ||
        op == "/" ||
        op == "%" ||
        op == "+" ||
        op == "-" ||
        op == "==" ||
        op == "!=" ||
        op == "<" ||
        op == "<=" ||
        op == ">" ||
        op == ">=" ||
        op == "&&" ||
        op == "||"){
            return true;
        }
        return false;
}

//Helper to determine the operator precedence level of this string
int Parser::precedence(std::string op){
    if(op == "*" || op == "/" || op == "%"){
        return 5;
    } else if(op == "+" || op == "-"){
        return 4;
    } else if(op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">="){
        return 3;
    } else if (op == "&&"){
        return 2;
    } else if (op == "||"){
        return 1;
    }
    return 6;
}