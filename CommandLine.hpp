#ifndef __COMMANDLIST_HPP__
#define __COMMANDLIST_HPP__

#include "Command.hpp"
#include <list>
#include <string>

class CommandLine {
    std::list<Command*>* pipeline;
    bool background;
    std::list<Command*>::iterator iterator;
    
public:
    CommandLine& operator= (CommandLine&);
    CommandLine(std::list<Command*> * pipeline, bool background);
    ~CommandLine();
    Command* next();    
    bool hasNext();
    bool isBackground();
};

#endif
