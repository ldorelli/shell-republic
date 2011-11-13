#include <iostream>
#include "CommandLine.hpp"
#include "Parser.hpp"
#include "MyTypo.hpp"
#include "Executor.hpp"
#include <signal.h>
#include "Handlers.hpp"
#include <map>
#include "Builtin.hpp"
#include "Program.hpp"

int Program::run () {
    
    setpgid(getpid(), getpid());
    /* Ignora os sinais inuteis */
    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    signal (SIGTTIN, SIG_IGN);
    signal (SIGTTOU, SIG_IGN);
    
    struct sigaction newAction;

    /* Registra o handler para SIGCHLD */
    
	newAction.sa_handler = handlers::sigChildHandler;
    sigemptyset(&newAction.sa_mask);
    newAction.sa_flags = 0;
    sigaction (SIGCHLD, &newAction, 0);
    
    tcsetpgrp(0, getpid());
    
    std::map<std::string, Builtin*> bCommands;
 	
	/* Registra os comandos built-in */

    bCommands[std::string("jobs")] = new JobsCommand();
    bCommands[std::string("pwd")] = new PwdCommand();
    bCommands[std::string("exit")] = new ExitCommand();
    bCommands[std::string("quit")] = 
    bCommands[std::string("exit")];
    bCommands[std::string("cd")] = new CdCommand();
    bCommands[std::string("bg")] = new BgCommand();
    bCommands[std::string("fg")] = new FgCommand();
    bCommands[std::string("kill")] = new KillCommand();
 	bCommands[std::string("echo")] =  new EchoCommand();

    Parser parser;
    Executor executor;
    
    MyTypo myt1(MyTypo::NORMAL, MyTypo::CYAN);
    MyTypo myt2(MyTypo::NORMAL, MyTypo::BROWN);
    MyTypo myt3(MyTypo::BLINK, MyTypo::RED);
    
    while (true) {
    	/* Imprime o prompt */
		if (parser.newLine()) std::cout <<
            myt1 << "Shell" << myt1 << myt3 << "-" << myt3 << myt1 << "Republic" << myt1 << myt2 << "$ " << myt2;
        CommandLine *cl = parser.readCommandLine();
        executor.cleanUp();
        if (cl)
            executor.run(cl, bCommands);
        delete cl;
    }
    return 0;
}
