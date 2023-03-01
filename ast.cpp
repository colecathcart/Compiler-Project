#include "token.hpp"
#include "ast.hpp"

Ast::Ast(std::string type_, std::string attr_, int where_){
    type = type_;
    attr = attr_;
    where = where_;
}

Ast::Ast(std::string type_){
    type = type_;
}