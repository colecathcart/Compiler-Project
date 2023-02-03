/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///
/// This is the scanner class which contains all the functionality
/// of the compiler scanner. Its methods are called by main.cpp.
/// The implementation is based off  Dr. Aycock's example code 
/// in python, shown in the video series "Calculator demo video 
/// series" under "Other resources" on the CPSC 411 d2l page.
///
/// This implementation is written purely in C++, without the use
/// of tools (lex, flex, etc)

#include <stdio.h>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){

    if(argc < 2){
        fprintf(stderr, "Error: Need to specify 1 command-line arg for input file\n");
        return 1;
    }

    if(argc > 2){
        fprintf(stderr, "Error: Too many command-line args (need 1 for input file)\n");
        return 1;
    }

    std::ifstream file(argv[1]);

    if(!file){
        fprintf(stderr, "Error: Could not find or open specified file\n");
        return 1;
    }

    file.close();

}