
#include "Parser.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>

CommandLine Parser::readCommandLine () {
    std::string line;
    int chRead = 0, totRead = 0;
    char *buffer;
    const char *c_str;
    std::getline(std::cin, line);
    int length = line.length();
    std::vector<std::string>* parameters = new std::vector<std::string>;
    
    buffer = new char[length];
    c_str = line.c_str();

    while (totRead != length) {
        sscanf(c_str + totRead, "%s%n", buffer, &chRead);
        parameters->push_back(std::string(buffer));
        totRead += chRead;
    }
    
    return CommandLine(parameters);
}

CommandLine::CommandLine (std::vector<std::string>* parameters) {
    this->parameters = parameters;
    execv = NULL;
}


CommandLine::~CommandLine() {
    delete parameters;
    delete execv;
}

const std::vector<std::string>* CommandLine::getParameters() {
    return (const std::vector<std::string>*) parameters;
}

const char ** CommandLine::execvFormat() {
    if (execv == NULL) {
        int size = parameters->size();
        execv = new const char*[size+1];
        execv[size] = NULL;
        for (int i = 0; i < size; i++) {
            execv[i] = (*parameters)[i].c_str();
        }
    }
    return execv;
}
