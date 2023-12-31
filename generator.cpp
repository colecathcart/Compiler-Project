/// CPSC 411 Term Project: Scanner Class
/// Cole Cathcart, 30043556
/// April 12, 2023
///
/// This is the generator class which contains all the functionality
/// of the compiler's C code generator. Its methods are called by main.cpp.
/// The implementation is based off  Dr. Aycock's example code 
/// in python, as shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page:
/// https://youtu.be/NcIhZ_6l3T8 , as well as the outputs of the reference
/// compiler found on the school linux servers.
///

#include "ast.hpp"
#include "token.hpp"
#include "logger.hpp"
#include "symbol.hpp"
#include "stable.hpp"
#include "generator.hpp"
#include <string>

//Constructor for generator class
//
//Ast ast: the tree to generate C code from
//
Generator::Generator(Ast &ast) : ast_{ast}{
    logger = Logger::getLogger();
}

//Top-level function called by main. Calls the other generator functions
void Generator::generate(){
    //zerodivcheck(ast_);
    realreturncheck();
    before();
    toplevel();
    outputter(ast_, 0, ";");
    after();
}

//Function to handle all pre-declared variables and functions
void Generator::before(){
    printf("#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <sys/types.h>\n");
    printf("typedef int32_t golf_t;\ngolf_t Gtrue = 1;\ngolf_t Gfalse = 0;\n");
    printf("golf_t Ggetchar();\n_Noreturn void Ghalt();\ngolf_t Glen(const char *);\nvoid Gprintb(golf_t);\nvoid Gprintc(golf_t);\nvoid Gprinti(golf_t);\nvoid Gprints(const char *);\n_Noreturn void Gnoreturn(const char *fn);\ngolf_t Gzerodiv(golf_t n);\n");

    printf("_Noreturn\nvoid Ghalt(){\n\texit(0);\n}\n");
    printf("golf_t Ggetchar(){\n\treturn getchar();\n}\n");
    printf("golf_t Glen(const char *s){\n\treturn strlen(s);\n}\n");
    printf("void Gprintb(golf_t b){\n\tprintf(b ? \"true\" : \"false\");\n}\n");
    printf("void Gprintc(golf_t c){\n\tputchar(c);\n}\n");
    printf("void Gprinti(golf_t i){\n\tprintf(\"%%d\", i);\n}\n");
    printf("void Gprints(const char *s){\n\tprintf(\"%%s\", s);\n}\n");
	printf("golf_t Gzerodiv(golf_t n){\n\tif (n == 0) {\n\tfprintf(stderr, \"error: division by zero\\n\");\n\texit(!0);}\n\treturn n;}\n");
	printf("_Noreturn\nvoid Gnoreturn(const char *fn){\n\tfprintf(stderr, \"error: function \\\"%%s\\\" must return a value\\n\", fn);\n\texit(!0);}\n");
}

//Function for removing leading zeroes on an integer type
std::string Generator::lzremover(std::string num){
    if(num[0] == '0' && num.size() > 1){
        return lzremover(num.substr(1));
    }else{
        return num;
    }
}

//Function to handle dividing by zero (NOT USED)
void Generator::zerodivcheck(Ast &ast){
    for(auto i : ast.children){
        if(ast.type == "/" && lzremover(ast.children[1].attr) == "0"){
            logger->error("Division by zero",ast.where);
        }
        zerodivcheck(i);
    }
}

//Helper function for realreturnchek
int Generator::realrethelper(Ast &ast){
    int goodreturn = 0;
    for(auto i: ast.children){
        if(i.type == "return" || i.attr == "halt"){
            goodreturn++;
        }else if(i.type == "if"){
            for(auto k : i.children){
                if(k.type == "else"){
                    goodreturn += realrethelper(k);
                }
            }
        }else{
            goodreturn += realrethelper(i);
        }
    }
    return goodreturn;
}

//Function to handle returns that only exist within conditionals
void Generator::realreturncheck(){
    for(auto &i : ast_.children){
        if(i.type == "func" && i.children[1].children[1].attr != "void"){
            int goodreturn = 0;
            for(auto j : i.children[2].children){
                if(j.type == "return" || j.attr == "halt"){
                    goodreturn++;
                }else if(j.type == "if"){
                    for(auto k : j.children){
                        if(k.type == "else"){
                            goodreturn += realrethelper(k);
                        }
                    }
                }else{
                    goodreturn += realrethelper(i);
                }
            }
            if(goodreturn < 1){
                Ast noret = Ast{"ID","noreturn",i.where};
				Ast args = Ast{"arguments"};
				Ast arg = Ast{"STRING",i.children[0].attr,i.where};
				args.children.push_back(arg);
				noret.children.push_back(args);
				i.children[2].children.push_back(noret);
            }
        }
    }
}

//Helper function to generate all top-level variable and function declarations
void Generator::toplevel(){
    for(auto i : ast_.children){
        if(i.type == "func"){
            std::string fname = i.children[0].attr;
            std::string type = i.children[1].children[1].attr;
            if(type == "int" || type == "bool"){
                type = "golf_t";
            }else if(type == "string"){
                type = "const char *";
            }else{
				type = "void";
			}
            printf("%s G%s(",type.c_str(),fname.c_str());
            unsigned int length = 0;
            for(auto j : i.children[1].children[0].children){
                length++;
                std::string pname = "G" + j.children[0].attr;
                std::string ptype = j.children[1].attr;
                if(ptype == "int" || ptype == "bool"){
                    ptype = "golf_t";
                }else{
                    ptype = "const char *";
                }
                if(length == i.children[1].children[0].children.size()){
                    printf("%s %s", ptype.c_str(), pname.c_str());
                }else{ 
                    printf("%s %s, ", ptype.c_str(), pname.c_str());
                }
            }
            printf(");\n");
        }else if(i.type == "var"){
            std::string vtype = i.sig;
            if(vtype == "int"){
                vtype = "golf_t";
                std::string vname = i.children[0].attr;
                printf("%s G%s = 0;\n",vtype.c_str(),vname.c_str());
            }else if(vtype == "bool"){
                vtype = "golf_t";
                std::string vname = i.children[0].attr;
                printf("%s G%s = 0;\n",vtype.c_str(),vname.c_str());
            }else{
                vtype = "const char *";
                std::string vname = i.children[0].attr;
                printf("%s G%s = \"\";\n",vtype.c_str(),vname.c_str());
            }
        }
    }
}

//Main function that parses the AST and outputs the corresponsing C-code
void Generator::outputter(Ast &tree, int level, std::string setsemi){
    for(auto &i : tree.children){
        if(i.type == "func"){
            std::string fname = i.children[0].attr;
            std::string type = i.children[1].children[1].attr;
            if(type == "int" || type == "bool"){
                type = "golf_t";
            }else if(type == "string"){
                type = "const char *";
            }else{
				type = "void";
			}
            printf("%s G%s(",type.c_str(),fname.c_str());
            unsigned int length = 0;
            for(auto j : i.children[1].children[0].children){
                length++;
                std::string pname = "G" + j.children[0].attr;
                std::string ptype = j.children[1].attr;
                if(ptype == "int" || ptype == "bool"){
                    ptype = "golf_t";
                }else{
                    ptype = "const char *";
                }
                if(length == i.children[1].children[0].children.size()){
                    printf("%s %s", ptype.c_str(), pname.c_str());
                }else{ 
                    printf("%s %s, ", ptype.c_str(), pname.c_str());
                }
            }
            printf(")");
            outputter(i, level + 1, setsemi);
        }else if(i.type == "var" && level != 0){
            std::string vtype = i.sig;
            if(vtype == "int"){
                vtype = "golf_t";
                std::string vname = i.children[0].attr;
                printf("%s G%s = 0;\n",vtype.c_str(),vname.c_str());
            }else if(vtype == "bool"){
                vtype = "golf_t";
                std::string vname = i.children[0].attr;
                printf("%s G%s = 0;\n",vtype.c_str(),vname.c_str());
            }else{
                vtype = "const char *";
                std::string vname = i.children[0].attr;
                printf("%s G%s = \"\";\n",vtype.c_str(),vname.c_str());
            }
        }else if(i.type == "block"){
            printf("{\n");
            outputter(i, level + 1, ";");
            printf("}\n");
        }else if(i.type == "ID"){
            std::string iname = "G" + i.attr;
            printf("%s",iname.c_str());
            if(i.children.size() > 0 && i.children[0].type == "arguments"){
                printf("(");
                unsigned int length = 0;
                for(auto j : i.children[0].children){
                    length++;
                    argshandler(j);
                    if(length < i.children[0].children.size()){
                        printf(", ");
                    }
                }
                printf(")%s",setsemi.c_str());
            }
        }else if(i.type == "="){
            printf("G%s = ",i.children[0].attr.c_str());
            if(i.children[1].type == "INTEGER"){
                printf("%s;\n",lzremover(i.children[1].attr).c_str());
            }else if(i.children[1].type == "STRING"){
                printf("\"%s\";\n",i.children[1].attr.c_str());
            }else if(i.children[1].type == "ID"){
                argshandler(i.children[1]);
                printf(";\n");
            }else{
                eqhandler(i.children[1]);
                printf(";\n");
            }
        }else if(i.type == "for" || i.type == "if"){
            if(i.type == "for"){
                printf("while(");
            } else {
                printf("%s(",i.type.c_str());
            }
            if(i.children[0].attr == "true"){
                printf("1){\n");
            }else if(i.children[0].type == "ID"){
                argshandler(i.children[0]);
                printf("){\n");
            }else{
                eqhandler(i.children[0]);
                printf("){\n");
            }
            outputter(i.children[1], level + 1, ";");
            printf("}\n");
            if(i.children.size() > 2 && i.children[2].type == "else"){
                printf("else ");
                outputter(i.children[2], level + 1, ";");
            }
        }else if(i.type == "else"){
            printf("else ");
            outputter(i, level + 1, ";");
        }else if(i.type == "return"){
            printf("return ");
            outputter(i,level,"");
            printf(";\n");
        }else if(is_eq(i.type)){
            eqhandler(i);
            printf("%s",setsemi.c_str());
        }else if(i.type == "INTEGER"){
            printf("%s%s",i.attr.c_str(),setsemi.c_str());
        }else if(i.type == "STRING"){
            printf("\"%s\"%s",i.attr.c_str(),setsemi.c_str());
        }else if(i.type == "break"){
			printf("break;");
		}
    }
}

void Generator::argshandler(Ast &ast){
    if(ast.type == "ID"){
            printf("G%s",ast.attr.c_str());
            if(ast.children.size() > 0 && ast.children[0].type == "arguments"){
                printf("(");
                unsigned int length = 0;
                for(auto j : ast.children[0].children){
                    length++;
                    argshandler(j);
                    if(length < ast.children[0].children.size()){
                        printf(", ");
                    }
                }
                printf(")");
            }
    }else if(ast.type == "INTEGER"){
        printf("%s",lzremover(ast.attr).c_str());
    }else if(ast.type == "STRING"){
        printf("\"%s\"",ast.attr.c_str());
    }else{
        eqhandler(ast);
    }
}

//Function to handle boolean and arithmetic expressions
void Generator::eqhandler(Ast &ast){
    if(ast.type == "u!" || ast.type == "u-" || ast.type == "expr_operand"){
        if(ast.type == "u!"){
            printf("!(");
        }else if(ast.type == "u-"){
            printf("-(");
        }else{
            printf("(");
        }
        eqhandler(ast.children[0]);
        printf(")");
    } else {
        if(is_eq(ast.type)){
            if(ast.children.size() > 1 && ast.children[0].type == "STRING" && ast.children[1].type == "STRING"){
                printf("strcmp(\"%s\", \"%s\")", ast.children[0].attr.c_str(), ast.children[1].attr.c_str());
                if(ast.type == "<" || ast.type == ">" || ast.type == "<=" || ast.type == ">="){
                    printf(" %s 0", ast.type.c_str());
                }else if(ast.type == "=="){
                    printf(" == 0");
                }
            }else if(ast.children.size() > 1 && ast.children[0].type == "STRING" && ast.children[1].type == "ID"){
                printf("strcmp(\"%s\", ", ast.children[0].attr.c_str());
                argshandler(ast.children[1]);
                printf(") ");
                if(ast.type == "<" || ast.type == ">" || ast.type == "<=" || ast.type == ">="){
                    printf(" %s 0", ast.type.c_str());
                }else if(ast.type == "=="){
                    printf(" == 0");
                }
            }else if(ast.children.size() > 1 && ast.children[0].type == "ID" && ast.children[1].type == "STRING"){
                printf("strcmp(");
                argshandler(ast.children[0]);
                printf(", \"%s\")", ast.children[1].attr.c_str());
                if(ast.type == "<" || ast.type == ">" || ast.type == "<=" || ast.type == ">="){
                    printf(" %s 0", ast.type.c_str());
                }else if(ast.type == "=="){
                    printf(" == 0");
                }
            }else{
                eqhandler(ast.children[0]);
                printf("%s ",ast.type.c_str());
                if(ast.children.size() > 1 && ast.type == "/"){
					printf("Gzerodiv(");
                    eqhandler(ast.children[1]);
					printf(")");
                }else if(ast.children.size() > 1){
					eqhandler(ast.children[1]);
				}
            }
        }else if(ast.type == "ID"){
            printf("G%s",ast.attr.c_str());
            if(ast.children.size() > 0 && ast.children[0].type == "arguments"){
                printf("(");
                unsigned int length = 0;
                for(auto j : ast.children[0].children){
                    length++;
                    argshandler(j);
                    if(length < ast.children[0].children.size()){
                        printf(", ");
                    }
                }
                printf(")");
            }
        }else if(ast.type == "STRING"){
            printf("\"%s\" ",ast.attr.c_str());
        }else if(ast.type == "INTEGER"){
            printf("%s ",lzremover(ast.attr).c_str());
        }else{
            printf("%s ",ast.type.c_str());
        }
    }
}

bool Generator::is_eq(std::string t){
    return (t == "+" || t == "-" ||
        t == "*" || t == "/" ||
        t == "%" || t == "u-" ||
        t == "u!" || t == ">" ||
        t == "<" || t == ">=" ||
        t == "<=" || t == "==" ||
        t == "!=" || t == "&&" ||
        t == "||" || t == "expr_operand");
}

//Function to handle ending the generated C code
void Generator::after(){
    printf("int main(){\n\tGmain();\n\treturn 0;\n}\n");
}