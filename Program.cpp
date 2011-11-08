#include <iostream>
#include "CommandLine.hpp"
#include "Parser.hpp"
#include "MyTypo.hpp"
#include "Executor.hpp"
#include <signal.h>

int main (int argc, char *argv[]) {
    
    setpgid(getpid(), getpid());
  
    
//    signal (SIGINT, SIG_IGN);
//    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    
    tcsetpgrp(0, getpid());
    
    Parser parser;
    Executor executor;
    std::string presentation("shell$ ");
    while (true) {
        if (parser.newLine()) std::cout << presentation;
        CommandLine *cl = parser.readCommandLine();
        executor.run(cl);
        delete cl;
    }
    return 0;
}
