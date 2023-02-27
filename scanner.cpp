/// CPSC 411 Term Project: Scanner Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///
/// This is the scanner class which contains all the functionality
/// of the compiler scanner. Its methods are called by main.cpp.
/// The implementation is based off  Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8 
///

#include "token.hpp"
#include "scanner.hpp"
#include "logger.hpp"
#include <ctype.h>

//Default constructor
//
//std::ifstream &f: the file to be read from
//
//bool reread: boolean set by unlex() so that lex() returns the previous token
//
//Token lastoken: the last token returned by lex
//
//Logger::getLogger logger: the logger object (see logger.cpp)
//
//int line: the current line number
//
//bool ins_semi: boolean set by lex() if the next token should be a semicolon, as per GoLF specs
//
//bool maybe_semi: boolean set by lex() to aid in deciding value of ins_semi
//
//
//bool open_bracket: boolean set to determine when it might be permissible to add a ';' before
//  a closing bracket '}', set to true if the scanner encounters a '{'
//
Scanner::Scanner(std::ifstream &f) : reread{false}, lastoken{"","",0}, file{f} {

    logger = Logger::getLogger();
    line = 1;
    ins_semi = false;
    maybe_semi = false;
    open_bracket = false;

}

//Helper function to check for whitespace
bool iswhitespace(char ch){

    if(ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r'){
        return true;
    }
    return false;

}

//helper function that checks for operators and punctuation that are not 
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

//helper function that checks if a given string
//matches one of the keywords as defined by the GoLF specs
bool iskeyword(std::string word){
    if(word == "if" ||
    word == "for" ||
    word == "else" ||
    word == "var" ||
    word == "func" ||
    word == "break" ||
    word == "return"){
        return true;
    }
    return false;
}

//helper function that checks if the character following a '\'
//in a string is legal as per the GoLF specs ('\' is not included when using)
bool islegalescape(char ch){
    if(ch == 'b' ||
    ch == 'f' ||
    ch == 'n' ||
    ch == 'r' ||
    ch == 't' ||
    ch == '\\' ||
    ch == '"'){
        return true;
    }
    return false;
}

//Main function of the scanner. Finds the next character from
//the file and performs operations to detect if it is a legal
//token (returns that token), requires a warning (prints to standard error), 
//requires an error (prints to standard error and exits), or should be ignored.
//If reread has been set to true, simply returns the last calculated token. If
//ins_semi is set to true, outputs a semi-colon with no attribute as the next token.
//Please see token.cpp for a description of the token class. Some of the logic of this function
//is based on Dr. Aycock's python example video (link is in the description at the top of this file)
Token Scanner::lex(){

    //check for reread from unlex()
    if(reread == true){
        reread = false;
        return lastoken;
    }

    //check if next token should be a semicolon
    if(maybe_semi){
        char check = file.get();
        if(check == EOF || check == '\n'){
            ins_semi = true;
        }
        maybe_semi = false;
        file.unget();
    }

    //main loop. Note that while this is an infinite loop,
    //it always terminates after successfully creating 1 token,
    //and so must be called repeatedly from another file to scan
    //an entire file (see main.cpp)
    while(true){
        char ch = file.get();

        //return semi-colon token
        if(ins_semi == true){
            lastoken = Token(";","",line);
            ins_semi = false;
            file.unget();
        
        //check for eof
        }else if(ch == EOF){
            lastoken = Token("EOF", "EOF", line);
            open_bracket = false;
            
        //check for non-ascii characters
        }else if(!isascii(ch)){
            logger->warning("Skipping non-ASCII character",line);
            continue;

        //check for ascii-NUL
        } else if(ch == '\0'){
            logger->warning("Skipping NUL character",line);
            continue;

        //check for whitespace
        } else if(iswhitespace(ch)){
            if(ch == '\n'){
                line++;
            }
            continue;

        //check for singletons
        } else if(issingleton(ch)){
            if(ch == '}'){
                open_bracket = false;
                if(lastoken.type != ";" && lastoken.type != ""){
                    lastoken = Token(";","",line);
                    file.unget();
                }else {
                    std::string s(1,ch);
                    lastoken = Token(s, s, line);
                    maybe_semi = true;
                }
            }else {
                std::string s(1,ch);
                lastoken = Token(s, s, line);
                if(ch == ')' || ch == '}'){
                    maybe_semi = true;
                }
                if(ch == '{'){
                    open_bracket = true;
                }
            }

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
                logger->error("Bitwise AND not supported in GoLF",line);
            }
        
        //check for '||'
        } else if(ch == '|'){
            ch = file.get();
            if(ch == '|'){
                lastoken = Token("||","||",line);
            } else {
                logger->error("Bitwise OR not supported in GoLF",line);
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

        //check for integers
        } else if(isdigit(ch)){
            std::string number = "";
            while(isdigit(ch)){
                number.push_back(ch);
                ch = file.get();
            }
            file.unget();
            lastoken = Token("INTEGER",number,line);
            maybe_semi = true;
        
        //check for identifiers or keywords
        } else if(isalpha(ch) || ch == '_'){
            std::string word = "";
            while(isalpha(ch) || ch == '_' || isdigit(ch)){
                word.push_back(ch);
                ch = file.get();
            }
            //error checking for special case that this is an unescaped double-quote in a string
            if(ch == '"' && lastoken.type == "STRING"){
                logger->error("Unescaped double-quote in string",line);
            }
            file.unget();
            if(iskeyword(word)){
                lastoken = Token("KEYWORD",word,line);
                if(word == "break" || word == "return"){
                    maybe_semi = true;
                }
            } else {
                lastoken = Token("ID",word,line);
                maybe_semi = true;
            }
            
        //check for strings and string errors based on illegal escapes, illegal characters,
        //bad terminations, or unescaped double-quotes  
        } else if(ch == '"'){
            ch = file.get();
            std::string string = "";
            while(ch != '"'){
                if(ch == EOF){
                    logger->error("String terminated by end-of-file",line);
                }
                if(ch == '\n'){
                    logger->error("String contains newline character",line);
                }
                if(ch == '\\'){
                    char secondchar = file.get();
                    if(!islegalescape(secondchar)){
                        if(secondchar == '\n'){
                            logger->error("String terminated by newline",line);
                        }
                        if(secondchar == EOF){
                            logger->error("String terminated by EOF",line);
                        }
                        std::string badchar(1,secondchar);
                        std::string escaperr = "Illegal escape character \\" + badchar;
                        logger->error(escaperr,line);
                    }
                    string.push_back(ch);
                    string.push_back(secondchar);
                    ch = file.get();
                } else {
                    string.push_back(ch);
                    ch = file.get();
                }
            }
            lastoken = Token("STRING",string,line);
            maybe_semi = true;

        //all other unrecognized characters
        } else {
            logger->warning("Skipping unrecognized character",line);
            continue;
        }
        return lastoken;

    }
    //SHOULD NEVER BE REACHED
    return Token("","",0);
}

//'Undoes' the previous lex (for use by the parser)
void Scanner::unlex(){
    reread = true;
}