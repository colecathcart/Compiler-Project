/// CPSC 411 Term Project: Scanner Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///
/// This is the scanner class which contains all the functionality
/// of the compiler scanner. Its methods are called by main.cpp.
/// The implementation is based off  Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page.
///

#include "token.hpp"
#include "scanner.hpp"
#include "logger.hpp"

//Default constructor
//
//std::ifstream &f: the file to be read from
//
Scanner::Scanner(std::ifstream &f) : reread{false}, lastoken{"","",0}, file{f} {

    logger = Logger::getLogger();
    line = 1;
}

//Helper function to check for whitespace
bool iswhitespace(char ch){

    if(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r'){
        return true;
    }
    return false;

}

//helper function to check for non-ascii characters
bool isnotascii(char ch){
    return false;
}

//checks for operators and punctuation that are not 
//potentially part of a multi-char operator
bool issingleton(char ch){
    if(ch == '+' ||
    ch == '-' ||
    ch == '*' ||
    ch == '%' ||
    ch == ',' ||
    ch == ';' ||
    ch == '(' ||
    ch == ')' ||
    ch == '{' ||
    ch == '}') {
        return true;
    }
    return false;
}

//Main function of the scanner. Finds the next character from
//the file and performs operations to detect if it is a legal
//token (returns that token), requires a warning (prints to standard error), 
//requires an error (prints to standard error and exits), or should be ignored.
//If reread has been set to true, simply returns the last calculated token
Token Scanner::lex(){
    if(reread == true){
        reread = false;
        return lastoken;
    }

    while(true){
        char ch = file.get();

        //check for eof
        if(ch == EOF){
            lastoken = Token("EOF", "EOF", line);

        //check for non-ascii characters
        }else if(isnotascii(ch)){
            logger->warning("Skipping non-ASCII character");
            continue;

        //check for ascii-NUL
        } else if(ch == '\0'){
            logger->warning("Skipping NUL character");
            continue;

        //check for whitespace
        } else if(iswhitespace(ch)){
            if(ch == '\n'){
                line++;
            }
            continue;

        //check for singletons
        } else if(issingleton(ch)){
            std::string s(1,ch);
            lastoken = Token(s, s, line);

        //check for '!' or '!='
        } else if(ch == '!'){
            ch = file.get();
            if(ch == '='){
                lastoken = Token("!=","!=",line);
            } else {
                file.unget();
                lastoken = Token("!","!",line);
            }

        //check for '=' or '=='
        } else if(ch == '='){
            ch = file.get();
            if(ch == '='){
                lastoken = Token("==","==",line);
            } else {
                file.unget();
                lastoken = Token("=","=",line);
            }

        //check for '>' or '>='
        } else if(ch == '>'){
            ch = file.get();
            if(ch == '='){
                lastoken = Token(">=",">=",line);
            } else {
                file.unget();
                lastoken = Token(">",">",line);
            }

        //check for '<' or '<='
        } else if(ch == '<'){
            ch = file.get();
            if(ch == '='){
                lastoken = Token("<=","<=",line);
            } else {
                file.unget();
                lastoken = Token("<","<",line);
            }
        
        //check for '&&'
        } else if(ch == '&'){
            ch = file.get();
            if(ch == '&'){
                lastoken = Token("&&","&&",line);
            } else {
                logger->error("Bitwise AND not supported in GoLF");
            }
        
        //check for '||'
        } else if(ch == '|'){
            ch = file.get();
            if(ch == '|'){
                lastoken = Token("||","||",line);
            } else {
                logger->error("Bitwise OR not supported in GoLF");
            }
        
        //check for '/' or comment (//...)
        } else if(ch == '/'){
            ch = file.get();
            if(ch == '/'){
                while(ch != '\n' && ch != EOF){
                    ch = file.get();
                }
                file.unget();
                continue;
            } else {
                file.unget();
                lastoken = Token("/","/",line);
            }
        }
    }

    return lastoken;
}

//'Undoes' the previous lex (for use by the parser)
void Scanner::unlex(){
    reread = true;
}