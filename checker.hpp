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
        void const_and_rangecheck(Ast &tree);
        std::string signaturize(Ast &tree, Stable &table, int level);
        bool ispredeclared(std::string type);
        Ast &ast_;
        Logger *logger = nullptr;

};

#endif