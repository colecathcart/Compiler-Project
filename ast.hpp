#ifndef AST
#define AST
#include "token.hpp"
#include <string>
#include <vector>

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