#ifndef SYMBOL
#define SYMBOL

#include <string>
#include <vector>

//Header file for symbol.cpp, please see that file for
//descriptions of methods and variables
class Symbol {

    public:
        Symbol() = default;
        Symbol(std::string name_, std::string sig_, std::vector<std::string> formals_);
        Symbol(std::string name_, std::string sig_);
        std::string name;
        std::string sig;
        std::vector<std::string> formals;

};
#endif