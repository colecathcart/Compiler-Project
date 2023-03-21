#ifndef CHECKER
#define CHECKER

#include "logger.hpp"
#include "ast.hpp"

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
        Ast &ast_;
        Logger *logger = nullptr;

};

#endif