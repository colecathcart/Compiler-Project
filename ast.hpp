#ifndef AST
#define AST
#include "token.hpp"
#include <string>
#include <vector>

//Header file for ast.cpp, please see that file for
//descriptions of methods and variables
class Ast
{
    public:
        Ast(std::string type_, std::string attr_, int where_);
        Ast(std::string type_);
        std::string type;
        std::string attr = "";
        int where = -1;
        std::vector<Ast> children;
};

#endif