#include "Parser.hpp"
#include <iostream>
#include <string>
#include "Command.hpp"
#include <signal.h>

Parser::Parser() : index(0) {}

std::string Parser::nextWord() {
    std::string word;
    bool specialChar = false; //It read '\\'
    bool insideComma = false;
    
    int lsize = line.size();
    for (; index < lsize; index++) {
        char ch = line[index];
        if (specialChar) specialChar = false, word += ch;
        else if (insideComma && ch == '"') insideComma = false;
        else if (ch == '\\') specialChar = true;
        else if (insideComma) word += ch;
        else if (ch == '"') insideComma = true;
        else if (ch == '&' && word.length() > 0) {; break; }
        else if (ch != ' ') word += ch;
        else if (!word.empty()) break;
    }
    return word;
}

CommandLine* Parser::readCommandLine () {
    tcsetpgrp(0, getpid());
    while (line.empty()) {
        index = 0;

        
//        Solucao parcial :
        
//        sigset_t mask;
//        sigset_t orig_mask;
//        
//        sigemptyset(&mask);
//        sigaddset(&mask, SIGCHLD);        
//        sigprocmask(SIG_BLOCK, &mask, &orig_mask);
        
        if (std::cin.eof()) {
            std::cin.clear();
        }
        std::getline(std::cin, line);
        
//        Solucao parcial: 
//        sigprocmask(SIG_SETMASK, &orig_mask, 0);
    }
    if (line.empty()) return 0;

    std::string in;
    std::string out;
    std::string err;
    bool errAppend = false;
    bool outAppend = false;
    std::vector<std::string> parameters;
    std::list<Command*> *commands = new std::list<Command*>;
    
    while (true) {
        std::string word = nextWord();
        if (word == "|") {
            
            commands->push_back(new Command(parameters, in, out, err, outAppend, errAppend));
            
            parameters.clear();
            in.clear();
            out.clear();
            err.clear();
            errAppend = false;
            outAppend = false;
        } else if (word == "&") {
            commands->push_back(new Command(parameters, in, out, err, outAppend, errAppend));
            if (index == line.size()) line.clear();
            return new CommandLine(commands, true);
        } else if (word == "&>") {
            out = err = nextWord();
        } else if (word == "1>" || word == ">") {
            out = nextWord();
        } else if (word == "1>>" || word == ">>") {
            out = nextWord();
            outAppend = true;
        } else if (word == "2>") {
            err = nextWord();
        } else if (word == "2>>") {
            err = nextWord();
            errAppend = true;
        } else if (word == "<") {
            in = nextWord();
        } else if (word == "") {
            commands->push_back(new Command(parameters, in, out, err, outAppend, errAppend));
            if (index == line.size()) line.clear();
            return new CommandLine(commands, false);
        } else {
            parameters.push_back(word);
        }
    }
}

bool Parser::newLine() {
    return line.empty();
}
