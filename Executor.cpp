#include "Executor.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>

std::list<Executor::Job>& Executor::getJobs() {
    return jobs;
}

int Executor::run(Command* command, int fdIn, int fdOut, int fdErr) {
    const char ** execvector = command->getExecv();
    Job job;
    if (!jobs.empty()) {
        job.jobid = jobs.rbegin()->jobid + 1;
    } else job.jobid = 1;
    job.name = execvector[0];
    jobs.push_back(job);
    
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
        fdOut = open(out.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    }
    
    if (!err.empty()) {
        if (fdErr != 2)
            close(fdErr);
        fdErr = open(err.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    }
    
    int pid = fork();
    
    if (pid == 0) {
        dup2(fdIn, 0);
        dup2(fdOut, 1);
        dup2(fdErr, 2);
    	if (fdIn != 0)
            close(fdIn); 
	    if (fdOut != 1)
            close(fdOut); 
        if (fdErr != 2)
            close(fdErr);
		if(execvp(execvector[0], (char*const*) execvector)==-1) exit(0);
    }

    return pid;
    
}

void Executor::run(CommandLine* cmdLine) {
    int fdIn = 0;
    int fdOut = 1;
    int fdErr = 2;
    int pp[2];
    int last;
    
    Command * command;
    while (command = cmdLine->next()) {
        if (cmdLine->hasNext())
		 	pipe(pp);
        fdOut = pp[1];
        last = run(command, fdIn, fdOut, fdErr);
        fdIn = pp[0];
    }
    
    if (!cmdLine->isBackground()) {
        waitpid(last, 0, 0);
    }
}
