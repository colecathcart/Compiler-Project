#ifndef TOKEN
#define TOKEN
#include <string>

//Header file for token.cpp, please see that file for descriptions
//of methods and variables
class Token
{
    public:
        Token(std::string type, std::string attr, int line);
        std::string type;
        std::string attr;
        int line;
    
};

#endif