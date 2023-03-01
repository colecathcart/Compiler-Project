# CPSC411 Term Project
Cole Cathcart, 30043556
Feb 3, 2023

A C++ program which is a compiler for the 'GoLF' language. Currently, only the scanner and parser portions of the compiler are implemented. This program was written in C++ without the use of scanner generating tools. A short description of the program structure:
- main.cpp: The driver of the program. Currently its only function is to open a file, call the main parser method and print its ast tree
- scanner.cpp/scanner.hpp: The scanner. Contains functions to facilitate the scanning of an input file, the main of which is called 'lex()' and contains the majority of the scanning logic
- logger.cpp/logger.hpp: Handles error and warning messages in order to have a central location for error handling. Implemented as a singleton pattern for ease of use
- token.cpp/token.hpp: A class representing a token. An instance of Token contains the type, value and line number of a token
- ast.cpp/ast.hpp: A class representing a custom datastructure for holding an abstract-syntax tree
- parser.cpp/parser.hpp: The parser. Contains functions to facilitate the parsing of tokens received from the scaner. The main function is 'parse()' and is called once to parse an entire input file. The parser is **not fully implemented** (see milestone 2 self-eval) and will not yet print a correct ast 

All references for these files are cited in the code where applicable

## Run Instructions
A makefile has been included in the repo. To run the compiler, simply use the 'make' command in the root directory. An executable file 'golf' will then be created. Then use the command './golf {filename}'. Failing to specify a file, specifying a nonexistent file, or specifying more than 1 argument will result in an error message. Otherwise, the ast/errors/warnings will be printed to standard output. For ease of testing, run the command 'make clean' to remove the executable and all .o files

## run.output
I have included a run.output file showing the output of the 'run' script on the latest build of this project. This file was generated from a copy of this repo on the cpsc linux servers

## milestone 1 pdf
For convenience I have included a copy of my milestone 1 submission here (also submitted on d2l)

## milestone 2 pdf
For convenience I have included a copy of my milestone 2 submission here (also submitted on d2l)


