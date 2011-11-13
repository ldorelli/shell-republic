#include "Builtin.hpp"
#include <unistd.h>
#include <iostream>
#include <list>
#include "MyTypo.hpp"
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Handlers.hpp"

void Builtin::run(const char* args[], Executor *executor) {
    int ret = _run(args, executor);
    if (forkable())
        exit(ret);
}

bool Builtin::forkable() { return false; }

int CdCommand::_run(const char*args[], Executor *executor) {
    if (args[1] != 0) {
        chdir(args[1]);
    }
    return 0;
}

int ExitCommand::_run(const char * args[], Executor *executor) {
    exit(0);
}

int PwdCommand::_run(const char * args[], Executor * executor) {
    char buffer[1024];
    std::cout << getcwd(buffer, 1023) << std::endl;
    return 0;
}

int JobsCommand::_run(const char * args[], Executor * executor) {
    std::list<Executor::Job> *lj = executor->getJobs();
    std::list<Executor::Job>::iterator itA, itB;
    
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        std::cout << myt << '[' << itA->jobid << "] " <<
        myt << itA->pid << " (" << itA->groupid << ")\t" <<
        ((itA->stopped)?"Stopped\t\t":"Running\t\t") <<
        itA->name << '\n';
    }
    return 0;
}

int FgCommand::_run(const char * args[], Executor * executor) {
    std::list<Executor::Job> *lj = executor->getJobs();
    std::list<Executor::Job>::iterator itA, itB;
    unsigned jobid;
    
    if (args[1] == 0) jobid = executor->getLastForeground();
    else sscanf(args[1], "%%%d", &jobid);
    
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        if (itA->jobid == jobid) {
            
            executor->setLastForeground(itA->jobid);
            executor->setForeground(itA->groupid);
            tcsetpgrp(0, itA->groupid);
            kill(itA->groupid, SIGCONT);
			executor->cleanUp();
			int status;
            do{
				waitpid(-itA->groupid, &status, WUNTRACED | WCONTINUED);
			}while(!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));
            executor->setForeground(getpid());
			tcsetpgrp(0, getpid());
			executor->cleanUp();
            break;
        }
    }
    
    return 0;
}

int BgCommand::_run(const char * args[], Executor * executor) {
    std::list<Executor::Job> *lj = executor->getJobs();
    std::list<Executor::Job>::iterator itA, itB;
    unsigned jobid;
    
    if (args[1] == 0) jobid = executor->getLastForeground();
    else sscanf(args[1], "%%%d", &jobid);
    
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        if (itA->jobid == jobid) {
            executor->setLastForeground(itA->jobid);
            kill(itA->pid, SIGCONT);
            handlers::setDeathStatusTrue();
            std::cout << myt << '[' << itA->jobid << "] " <<
            myt << itA->pid << " (" << itA->groupid << ")\t\t" <<
            itA->name << '\n';
            break;
        }
    }
    
    return 0;
}

int KillCommand::_run(const char * args[], Executor * executor) {
    std::list<Executor::Job> *lj = executor->getJobs();
    std::list<Executor::Job>::iterator itA, itB;
    unsigned jobid;
    
    if (args[1] == 0) return 0;
    else sscanf(args[1], "%%%d", &jobid);
    
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        if (itA->jobid == jobid) {
            kill(itA->pid, SIGTERM);
            handlers::setDeathStatusTrue();
            std::cout << myt << '[' << itA->jobid << "] " <<
            myt << itA->pid << " (" << itA->groupid << ")\t\t" <<
            itA->name << '\n';
            break;
        }
    }
    
    return 0;
}

int EchoCommand::_run(const char *args[], Executor * executor) {
	int i = 1;
	if(args[i]) printf("%s",args[i++]);
	while(args[i]) printf(" %s",args[i++]);
	printf("\n");
}

bool EchoCommand::forkable(){ return false; }

//int PwdCommand

bool JobsCommand::forkable() {return true; }
