#ifndef PARSER
#define PARSER
#include "scanner.hpp"
#include "ast.hpp"
#include "logger.hpp"
#include <string>

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
        Ast declarations();
        Ast statements();
        Ast declaration();
        Ast statement();
        Ast expression();
        bool is_statement_start(Token s);
        bool is_declaration_start(Token s);
        
};

#endif