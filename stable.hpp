#ifndef STABLE
#define STABLE

#include "logger.hpp"
#include "ast.hpp"
#include "symbol.hpp"
#include <string>
#include <map>

//Header file for stable.cpp, please see that file for
//descriptions of methods and variables
class Stable{

    public:
        Stable();
        void declare(Ast &t, Symbol &sym, int level);
        Symbol lookup(Ast &t);
        std::map<std::string, Symbol> symboltable;

        void print_table();

    private:
        
        Logger *logger = nullptr;
};
#endif