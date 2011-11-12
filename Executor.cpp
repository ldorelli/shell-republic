#include "Executor.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <termios.h>
#include "MyTypo.hpp"
#include "Handlers.hpp"

std::list<Executor::Job>& Executor::getJobs() {
    return jobs;
}

int Executor::run(Command* command, int & firstPipedPid, bool isBackground,
                  int fdIn, int fdOut, int fdErr){ 
    const char ** execvector = command->getExecv();
    Job job;
    if (!jobs.empty()) {
        job.jobid = jobs.rbegin()->jobid + 1;
    } else job.jobid = 1;
    
    job.name = execvector[0];
    for (int i = 1; execvector[i]; i++) {
        job.name += ' ';
        job.name += execvector[i];
    }
    
    std::string in, out, err;
    bool outap, errap;
    in = command->getIn();
    out = command->getOut();
    err = command->getErr();
    outap = command->getOutAppend();
    errap = command->getErrAppend();
    
    if (!in.empty()) {
        if (fdIn != 0)
            close(fdIn);
        fdIn = open(in.c_str(), O_RDONLY);
    }
    
    if (!out.empty()) {
        if (fdOut != 1)
            close(fdOut);
		if(outap)
	        fdOut = open(out.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
		else
	        fdOut = open(out.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    }
    
    if (!err.empty()) {
        if (fdErr != 2)
            close(fdErr);
        if(errap)
			fdErr = open(err.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
		else	
			fdErr = open(err.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
			
    }
    
    /* mutex aqui */
    int pid = fork();
    
    if (pid == 0) {
        
        if (!firstPipedPid) firstPipedPid = getpid();
        setpgid(getpid(), firstPipedPid);
        if (!isBackground)
            tcsetpgrp(0, getpid());
        
        signal (SIGINT, SIG_DFL);
        signal (SIGQUIT, SIG_DFL);
        signal (SIGTSTP, SIG_DFL);
        signal (SIGTTIN, SIG_DFL);
        signal (SIGTTOU, SIG_DFL);
        signal (SIGCHLD, SIG_DFL);
        
		if(fdIn != 0){
			dup2(fdIn, 0);
			close(fdIn);
		}
		
		if(fdOut != 1){
			dup2(fdOut,1);
			close(fdOut);
		}

        if(fdErr!=2){
			dup2(fdErr, 2);
			close(fdErr);
		}	
		
		if(execvp(execvector[0], (char*const*) execvector)==-1) exit(0);
    } else {
        if (!firstPipedPid) firstPipedPid = pid;
        setpgid(pid, firstPipedPid);
        job.groupid = firstPipedPid;
        job.pid = pid;
        if (!isBackground) {
            foreground = firstPipedPid;
            tcsetpgrp(0, pid);
            tcsetattr(0, TCSADRAIN, 0);
        }
        else {
            MyTypo myt(MyTypo::NORMAL, MyTypo::PURPLE);
            std::cout << myt << "[" << job.jobid << "] " << myt <<
            job.pid << std::endl;
        }
        jobs.push_back(job);
        /* up */
    }
    return pid;
}

void Executor::run(CommandLine* cmdLine) {
    int fdIn = 0;
    int fdOut = 1;
    int fdErr = 2;
    int pp[2];
    int last;
    int firstPipedPid = 0;
    Command * command;
	pp[0] = 0;
	pp[1] = 1;
    while (command = cmdLine->next()) {
		if (cmdLine->hasNext()){
			pipe(pp);
			fdOut = pp[1];
		}else fdOut = 1;
        last = run(command, firstPipedPid, cmdLine->isBackground(),
                   fdIn, fdOut);
    	if(fdIn!=0) close(fdIn);
		if(fdOut!=1) close(fdOut);
		fdIn = pp[0];
	}    
    if(fdIn!=0) close(fdIn);
    
//    TODO: tah errado 
	if (!cmdLine->isBackground()) {
    	while( wait(0)>=0 );
	}
}

void Executor::cleanUp () {
    while (handlers::getDeathStatus()) {
        handlers::setDeathStatusFalse();
        std::list<Job>::iterator itA, itB;
        itA = jobs.begin();
        itB = jobs.end();
        
//        while (int who = waitpid(0, &status, WNOHANG)) {
//            
//        }
        
        while (itA != itB) {
            int status;
            if (waitpid(itA->pid, &status, WNOHANG)) {
                std::cerr << "YUHHUUU\n";
                std::cerr << foreground << " - " << itA->groupid << '\n';
                if (itA->groupid == foreground) {
                    tcsetpgrp(0, getpid());
                    tcsetattr(0, TCSADRAIN, &myTermios);
                    foreground = 0;
                    std::cerr << "passei por aqui ! \n";
                    std::cerr << tcgetpgrp(0) << std::endl;
                }
                itA = jobs.erase(itA);
            } else itA++;
        }
    }
}

Executor::Executor() : foreground(0) { tcgetattr(0, &myTermios); }
Executor::Job::Job() : stopped(false), dead(false) {}

