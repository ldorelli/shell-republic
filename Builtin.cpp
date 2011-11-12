#include "Builtin.hpp"
#include <unistd.h>
#include <iostream>
#include <list>
#include "MyTypo.hpp"
#include <cstdio>
#include <signal.h>
#include <sys/types.h>
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
    int jobid;
    
    if (args[1] == 0) jobid = executor->getLastForeground();
    else sscanf(args[1], "%d", &jobid);
    
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        if (itA->jobid == jobid) {
            
            executor->setLastForeground(itA->jobid);
            executor->setForeground(itA->pid);
            kill(itA->groupid, SIGCONT);
            
            tcsetpgrp(0, itA->groupid);
            
            waitpid(itA->groupid, 0, 0);
            
            executor->cleanUp();
            break;
        }
    }
    
    return 0;
}

int BgCommand::_run(const char * args[], Executor * executor) {
    std::list<Executor::Job> *lj = executor->getJobs();
    std::list<Executor::Job>::iterator itA, itB;
    int jobid;
    
    if (args[1] == 0) jobid = executor->getLastForeground();
    else sscanf(args[1], "%d", &jobid);
    
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

//int PwdCommand

bool JobsCommand::forkable() {return true; }