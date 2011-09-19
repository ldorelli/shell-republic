#ifndef __COMMANDLIST_HPP__
#define __COMMANDLIST_HPP__

#include "Command.hpp"
#include <list>
#include <string>

class CommandList {
    std::string out;
    std::string in;
    std::list<Command*> pipeline;
    bool background;
    
public:
    
    void addCommand(Command * command);
    
};

#endif
