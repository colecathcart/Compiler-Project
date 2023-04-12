#ifndef GENERATOR
#define GENERATOR

#include "logger.hpp"
#include "ast.hpp"
#include "symbol.hpp"
#include "stable.hpp"

//Header file for generator.cpp. Please see that file for function descriptions
class Generator {

    public:
        Generator(Ast &ast);
        void generate();

    private:
        void before();
        void toplevel();
        void outputter(Ast &ast, int level, std::string setsemi);
        void eqhandler(Ast &ast);
        void argshandler(Ast &ast);
        void after();
        bool is_eq(std::string t);
        std::string lzremover(std::string num);
        Ast &ast_;
        Logger *logger = nullptr;
};

#endif