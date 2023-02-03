/// CPSC 411 Term Project: Main Class
/// Cole Cathcart, 30043556
/// Feb 3, 2023
///
/// This class is meant to centralize all the error handling
/// of the program. Instead of printing errors directly, the other
/// classes call one of the Logger's functions, sending it the error or
/// warning message. This class is implemented as a singleton pattern so
/// that each class can access the same instance of Logger without passing
/// it around as a parameter. Singleton construction based loosely off this
/// reference from refactoring guru: 
/// https://refactoring.guru/design-patterns/singleton/cpp/example

#include "logger.hpp"
#include <iostream>

Logger* Logger::logger = nullptr;

//Default constructor (not used outside this class)
Logger::Logger(){
    warnings = 0;
}

//This getter function is used by other classes to
//recieve a pointer to the singleton Logger object.
//If no object exists, a new one is created.
Logger* Logger::getLogger(){

    if(logger == nullptr){
        logger = new Logger();
    }

    return logger;
}

//Prints an error message containing the 'msg' parameter
//and then exits with nonzero value to indicate an error
void Logger::error(std::string msg){

    fprintf(stderr, "Error: %s\n", msg.c_str());
    exit(1); 

}

//Override function: Prints an error message containing the 'msg' parameter
//and the line number containing the error (for use by the scanner)
//and then exits with nonzero value to indicate an error
void Logger::error(std::string msg, int line){

    fprintf(stderr, "Error: %s, at or near line %d\n", msg.c_str(), line);
    exit(1); 

}

//Increments the number of warnings and calls an error if this number
//reaches 10
void Logger::warnchecker(){

    warnings++;
    if(warnings == 10){
        error("Too many warnings!");
    }

}

//Prints a warning containing the 'msg' parameter
//and then calls the warn checker in case warnings = 10
void Logger::warning(std::string msg){

    fprintf(stderr, "Warning: %s\n", msg.c_str());
    warnchecker();

}

//Override function: Prints a warning containing the 'msg' parameter
//and the line number containing the warning (for use by the scanner)
//and then calls the warn checker in case warnings = 10
void Logger::warning(std::string msg, int line){

    fprintf(stderr, "Warning: %s, at or near line %d\n", msg.c_str(), line);
    warnchecker();

}
