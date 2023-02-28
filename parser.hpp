#ifndef PARSER
#define PARSER
#include "scanner.hpp"
#include "ast.hpp"
#include <string>

class Parser {

    public:
        Parser(Scanner &scanner);

        Ast parse();
    
    private:
        Scanner &scanner_;
        Ast statements();
        Ast statement();
        Ast expression();
        bool is_statement_start(Token s);
};

#endif