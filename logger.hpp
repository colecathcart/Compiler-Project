#ifndef LOGGER
#define LOGGER
#include <string>

//Header file for logger.cpp, please see that file for
//descriptions of methods and variables
class Logger
{
    public:

        void error(std::string msg);

        void error(std::string msg, int line);

        void warning(std::string msg);

        void warning(std::string msg, int line);

        static Logger* getLogger();
    
    private:
        Logger();
        static Logger* logger;
        int warnings;
        void warnchecker();

};

#endif