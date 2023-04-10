#ifndef CHECKER
#define CHECKER

#include "logger.hpp"
#include "ast.hpp"
#include "symbol.hpp"
#include "stable.hpp"

//Header file for checker.cpp, please see that file
//for description of methods and variables

class Checker {
    public:
        Checker(Ast &ast);
        Ast check();

    private:
        void maincheck();
        int returncheck(Ast &func, bool retunsval, std::string type);
        void breakcheck(Ast &tree);
        void funccheck();
        void handle_predeclared(Stable &s_table);
        void const_and_rangecheck(Ast &tree);
        void if_for_check(Ast &tree);
        void paramcheck(Ast &tree, Stable &s_table);
        std::string signaturize(Ast &tree, Stable &table, int level);
        bool ispredeclared(std::string type);
        void setglobals(Stable &table);
        Ast &ast_;
        std::map<std::string,Symbol> funcmap;
        Logger *logger = nullptr;

};

#endif