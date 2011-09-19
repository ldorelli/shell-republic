#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <string>
#include <vector>

class Command {
    char ** execvector;
    std::vector<std::string> parameters;
    std::string err;
    
    Command& operator= (const Command&);
    
public:
    Command(std::vector<std::string> & parameters,
             std::string & err = std::string());
    ~Command ();
    char ** getExecv();
};

#endif
