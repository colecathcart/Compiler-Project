#ifndef SCANNER
#define SCANNER
#include "token.hpp"
#include "logger.hpp"
#include <fstream>


//Header file for scanner.cpp, please see that file for
//descriptions of methods and variables
class Scanner
{
    public:
        Scanner(std::ifstream &f);

        Token lex();

        void unlex();
    
    private:
        bool reread;
        Token lastoken;
        std::ifstream &file;
        Logger *logger = nullptr;

};

#endif