#ifndef Parser_hpp
#define Parser_hpp

#include <string>
#include "CommandLine.hpp"


class Parser {
    std::string line;
    unsigned index;
    
    std::string nextWord();
    
public:
    Parser();
    CommandLine* readCommandLine();
    bool newLine();
};

#endif
