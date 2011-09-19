#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
#include "CommandLine.hpp"


class Parser {
    std::string line;
    unsigned index;
    
    std::string nextWord();
    
public:
    Parser();
    CommandLine readCommandLine();
};

#endif
