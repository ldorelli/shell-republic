#include <iostream>
#include "CommandLine.hpp"
#include "Parser.hpp"
#include "MyTypo.hpp"
#include "Executor.hpp"
#include <signal.h>
#include "Handlers.hpp"


Executor * executorPointer;

int main (int argc, char *argv[]) {
    
    setpgid(getpid(), getpid());
    
    std::cout << getpid() << std::endl;
    
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    signal (SIGCHLD, SIG_IGN);
    
    struct sigaction newAction;
    
    newAction.sa_handler = handlers::sigChildHandler;
    sigemptyset(&newAction.sa_mask);
    newAction.sa_flags = 0;
    sigaction (SIGCHLD, &newAction, 0);
    
    newAction.sa_handler = handlers::sigTStpHandler;
    sigemptyset(&newAction.sa_mask);
    newAction.sa_flags = 0;
    sigaction (SIGTSTP, &newAction, 0);
    
    tcsetpgrp(0, getpid());
    
    Parser parser;
    Executor executor;
	executorPointer = &executor;
    std::string presentation("Shell-Republic$ ");
    while (true) {
        if (parser.newLine()) std::cout << presentation;
        CommandLine *cl = parser.readCommandLine();
        executor.cleanUp();
        if (cl)
            executor.run(cl);
        delete cl;
    } else tcsetpgrp(STDIN_FILENO, getpid());
    return 0;
}
