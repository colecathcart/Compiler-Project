#ifndef SCANNER
#define SCANNER
#include "token.hpp"

//Header file for scanner.cpp, please see that file for
//descriptions of methods and variables
class Scanner
{
    public:
        Scanner();

        Token lex();

        void unlex();
    
    private:
        Token lastoken;
        bool reread;

};

#endif