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
        void returncheck(Ast &func, bool retunsval, std::string type);
        Ast &ast_;
        Logger *logger = nullptr;

};

#endif