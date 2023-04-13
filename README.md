# CPSC411 Term Project
Cole Cathcart, 30043556
April 12, 2023

A C++ program which is a compiler for the 'GoLF' language. This program was written in C++ without the use of external compiler-building tools. It compiles to **C code**. A short description of the program structure:
- main.cpp: The driver of the program
- scanner.cpp/scanner.hpp: The scanner. Contains functions to facilitate the scanning of an input file, the main of which is called 'lex()' and contains the majority of the scanning logic
- logger.cpp/logger.hpp: Handles error and warning messages in order to have a central location for error handling. Implemented as a singleton pattern for ease of use
- token.cpp/token.hpp: A class representing a token. An instance of Token contains the type, value and line number of a token
- ast.cpp/ast.hpp: A class representing a custom datastructure for holding an abstract-syntax tree
- parser.cpp/parser.hpp: The parser. Contains functions to facilitate the parsing of tokens received from the scaner. The main function is 'parse()' and is called once to parse an entire input file.
- symbol.cpp/symbol.hpp: A class representing symbols to be used in the symbol table class. A symbol contains the name, signature, 
scope level, and optional list of formals
- stable.cpp/stable.hpp: A class representing a symbol table. Has functions for adding and looking up symbols, and handles errors
related to redeclarations or missing declarations
- checker.cpp/checker.hpp: A class that performs all semantic checking/editing on an AST. Called once from main
- generator.cpp/generator.hpp: A class that outputs the annotated AST as C code to standart output
All references for these files are cited in the code where applicable

## Run Instructions
A makefile has been included in the repo. To run the compiler, simply use the 'make' command in the root directory. An executable file 'golf' will then be created. Then use the command './golf {filename}'. Failing to specify a file, specifying a nonexistent file, or specifying more than 1 argument will result in an error message. Otherwise, the C code/errors will be printed to standard output/error. For ease of testing, run the command 'make clean' to remove the executable and all .o files

## run.output
I have included a run.output file showing the output of the 'run' script on the latest build of this project. This file was generated from a copy of this repo on the cpsc linux servers
