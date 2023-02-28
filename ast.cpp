#include "token.hpp"
#include "ast.hpp"

Ast::Ast(std::string type, std::string attr, int where){
    type = type;
    attr = attr;
    where = where;
}

Ast::Ast(std::string type){
    type = type;
}