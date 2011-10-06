#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <string>
#include <vector>

class Command {
    const char ** execvector;
    std::vector<std::string> parameters;
    bool errAppend;
    bool outAppend;
    std::string err;
    std::string out;
    std::string in;
    
    Command& operator= (const Command&);
    
public:
    Command(std::vector<std::string> & parameters,
            std::string in = std::string(),
            std::string out = std::string(),
            std::string err = std::string(),
            bool outAppend = false,
            bool errAppend = false
            );
    ~Command ();
    std::string getIn();
    std::string getOut();
    std::string getErr();
    bool getOutAppend();
    bool getErrAppend();
    const char ** getExecv();
};

#endif
