#ifndef PARSER
#define PARSER
#include "scanner.hpp"
#include "ast.hpp"
#include "logger.hpp"
#include <string>

//Header file for parser.cpp, please see that file for
//descriptions of methods and variables
class Parser {

    public:
        Parser(Scanner &scanner);

        Ast parse();
    
    private:
        Scanner &scanner_;
        Logger *logger = nullptr;
        Token expect(std::string expected);
        Ast var();
        Ast func();
        Ast formal(Token &id, Token &type);
        Ast sig(Ast &formals, Token &returntype);
        Ast block();
        Ast declarations();
        Ast declaration();
        Ast statement();
        Ast singlexpr();
        Ast expression();
        Ast ifstmt();
        Ast forstmt();
        Ast expressiontree(std::vector<Ast> nodes);
        bool is_statement_start(Token s);
        bool is_declaration_start(Token s);
        bool is_expression_start(Token s);
        int precedence(std::string op);
        int isbinoperator(std::string op);
        
};

#endif