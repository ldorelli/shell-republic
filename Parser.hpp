#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
#include "CommandList.hpp"


class Parser {
    std::string line;
    unsigned index;
    
public:
    CommandList getCommandList();
};

#endif
