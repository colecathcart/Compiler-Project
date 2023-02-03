# CPSC411 Term Project
Cole Cathcart, 30043556
Feb 3, 2023

A C++ program which is a compiler for the 'GoLF' language. Currently, only the scanner portion of the compiler is complete.

## Run Instructions
A makefile has been included in the repo. To run the compiler, simply use the 'make' command in the root directory. An executable file 'golf' will then be created. Then use the command './golf {filename}'. Failing to specify a file, specifying a nonexistent file, or specifying more than 1 argument will result in an error message. Otherwise, all tokens/errors/warnings will be printed to command line. For ease of testing, run the command 'make clean' to remove the executable and all .o files


