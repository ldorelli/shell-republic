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
        myt << itA->pid << " (" << getpgid(itA->pid) << ")\t" <<
        ((itA->stopped)?"Stopped\t\t":"Running\t\t") <<
        itA->name << '\n';
    }
    return 0;
}

int FgCommand::_run(const char * args[], Executor * executor) {
    std::list<Executor::Job> *lj = executor->getJobs();
    std::list<Executor::Job>::iterator itA, itB;
    unsigned jobid;
 	int ret = 1;

    if (args[1] == 0) jobid = executor->getLastForeground();
    else ret = sscanf(args[1], "%%%d", &jobid);

	if(!ret){
		printf("Argumentos invalidos. Esperado: fg <ID> | fg\n");
		return 0;
	}
    
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    std::list<int> wail;
    int pgid = 0;
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        if (itA->jobid == jobid) {
            wail.push_back(itA->pid);
            if (!pgid) { 
                pgid = getpgid(itA->pid);
                executor->setLastForeground(itA->jobid);
            }
        }
    }
    if (!pgid) return 0;
    kill(-pgid, SIGCONT);
    tcsetpgrp(0, pgid);
    std::list<int>::iterator it;
#ifdef __linux__
    pause();
#endif
    for (it = wail.begin(); it != wail.end(); it++) {
        waitpid(*it, 0, 0);
    }    
    tcsetpgrp(0, getpid());
    executor->cleanUp();
    return 0;
}

int BgCommand::_run(const char * args[], Executor * executor) {
    std::list<Executor::Job> *lj = executor->getJobs();
    std::list<Executor::Job>::iterator itA, itB;
    unsigned jobid;
    int ret = 1;
    if (args[1] == 0) jobid = executor->getLastForeground();
    else ret = sscanf(args[1], "%%%d", &jobid);
	if(!ret){
		printf("Argumentos invalidos. Esperado: bg | bg <ID>\n");
		return 0;
	}
    
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        if (itA->jobid == jobid) {
            executor->setLastForeground(itA->jobid);
            kill(-getpgid(itA->pid), SIGCONT);
            handlers::setDeathStatusTrue();
            std::cout << myt << '[' << itA->jobid << "] " <<
            myt << itA->pid << " (" << getpgid(itA->pid) << ")\t\t" <<
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
    int ret = 1;
    if (args[1] == 0) return 0;
    else ret = sscanf(args[1], "%%%d", &jobid);
    if(!ret){
		printf("Argumentos invalidos. Esperado: kill <ID>\n");
		return 0;
	}
    MyTypo myt (MyTypo::NORMAL, MyTypo::PURPLE);
    
    for (itA = lj->begin(), itB = lj->end(); itA!=itB; itA++) {
        if (itA->jobid == jobid) {
            kill(-getpgid(itA->pid), SIGTERM);
            handlers::setDeathStatusTrue();
            std::cout << myt << '[' << itA->jobid << "] " <<
            myt << itA->pid << " (" << getpgid(itA->pid) << ")\t\t" <<
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

bool EchoCommand::forkable(){ return true; }
bool PwdCommand::forkable(){ return true; }
bool JobsCommand::forkable() {return true; }
