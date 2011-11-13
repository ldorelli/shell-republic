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
#include "MyTypo.hpp"
#include "Handlers.hpp"

std::list<Executor::Job>* Executor::getJobs() {
    return &jobs;
}


int Executor::run(Command* command, int & firstPipedPid, bool isBackground, std::map<std::string, Builtin*>& bCommands,
                  int fdIn, int fdOut, int fdErr){ 
    const char ** execvector = command->getExecv();
    Job job;
    if (!jobs.empty()) {
            job.jobid = jobs.rbegin()->jobid + ((firstPipedPid)?0:1);
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
    
    std::map<std::string, Builtin*>::iterator it = 
    bCommands.find(std::string(execvector[0]));
    
    if (it == bCommands.end() || it->second->forkable()) {
        int pid = fork();
        
        if (pid == 0) {
            
            if (!firstPipedPid) firstPipedPid = getpid();
            setpgid(getpid(), firstPipedPid);
            if (!isBackground)
                tcsetpgrp(0, firstPipedPid);
            
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
            
            if (it == bCommands.end()) {
                if(execvp(execvector[0], (char*const*) execvector)==-1){
					exit(0);
				}
            }
            else
                it->second->run(execvector, this);
        } else {
            if (!firstPipedPid) firstPipedPid = pid;
            setpgid(pid, firstPipedPid);
            job.pid = pid;
            setLastForeground(job.jobid);
            if (!isBackground)
                tcsetpgrp(0, firstPipedPid);
            else {
                MyTypo myt(MyTypo::NORMAL, MyTypo::PURPLE);
                std::cout << myt << "[" << job.jobid << "] " << myt <<
                job.pid << std::endl;
            }
            jobs.push_back(job);
        }
        return pid;
    } else {
        it->second->run(execvector, this);
    }
	return 0;
}

void Executor::run(CommandLine* cmdLine, std::map<std::string, Builtin*>& bCommands) {
    int fdIn = 0;
    int fdOut = 1;
    int pp[2];
    int last;
    int firstPipedPid = 0;
    std::list<int> wail;
    Command * command;
	pp[0] = 0;
	pp[1] = 1;
    while ((command = cmdLine->next())) {
		if (cmdLine->hasNext()){
			pipe(pp);
			fdOut = pp[1];
		}else fdOut = 1;
        last = run(command, firstPipedPid, cmdLine->isBackground(), bCommands, fdIn, fdOut);
        wail.push_back(last);
    	if(fdIn!=0) close(fdIn);
		if(fdOut!=1) close(fdOut);
		fdIn = pp[0];
	}    
    if(fdIn!=0) close(fdIn);
    
    if (firstPipedPid) {
        if (!cmdLine->isBackground()) {
            std::list<int>::iterator it;
            for (it = wail.begin(); it != wail.end(); it = wail.erase(it)) {
                waitpid(*it, 0, 0);
            }
            cleanUp();
        }
    }
    
}

void Executor::cleanUp () {
    while (handlers::getDeathStatus()) {
        handlers::setDeathStatusFalse();
        std::list<Job>::iterator itA, itB;
        itA = jobs.begin();
        itB = jobs.end();
        
//        Outra abordagem
//        while (int who = waitpid(0, &status, WNOHANG)) {
//            
//        }
        
        while (itA != itB) {
            int status;
            if (waitpid(itA->pid, &status, WNOHANG | WUNTRACED | WCONTINUED)) {
                if (WIFSTOPPED(status)) {
                    itA->stopped = true;
                    std::cout << '\n' << itA->pid << " stopped\n";
                    if (getpgid(itA->pid) == tcgetpgrp(0)) {
                        tcsetpgrp(0, getpid());
                    }
                } else if (WIFEXITED(status) || WIFSIGNALED(status)){
                    if (lastForeground == itA->jobid) {
                        lastForeground = 0;
                    }
                    if (getpgid(itA->pid) == tcgetpgrp(0)) {
                        tcsetpgrp(0, getpid());
                    }
                    itA = jobs.erase(itA);
                } else if (WIFCONTINUED(status)) {
                    itA->stopped = false;
                }
            } else itA++;
        }
        if (!lastForeground && !jobs.empty())
            lastForeground = jobs.begin()->jobid;
    }
}

//void Executor::setForeground (int pid) {foreground = pid; }

unsigned Executor::getLastForeground () { return lastForeground; }
void Executor::setLastForeground(unsigned jid) { lastForeground = jid;}

Executor::Executor() : lastForeground(0) {  }
Executor::Job::Job() : stopped(false), dead(false) {}

