#include "Executor.hpp"
#include <unistd.h>

std::list<Job> Executor::getJobs() {
    return jobs;
}

void Executor::run(Command* command, int& fdIn, int& fdOut, int& fdErr) {
    const char ** execvector = command.getExecv();
    Job job;
    if (!jobs.empty()) {
        job.jobid = jobs.rbegin().jobid + 1;
    } else job.jobid = 1;
    job.name = execvector[0];
    
    std::string in, out, err;
    bool outap, errap;
    in = command->getIn();
    out = command->getOut();
    err = command->getErr();
    outap = command->getOutAppend();
    errap = command->getErrAppend();
    
    if (!in.empty()) {
        //pipezar
    }
    
    if (!out.empty()) {
        //pipezar
    }
    
    if (!err.empty()) {
        //pipezar
    }
    
    
    
}

void Executor::run(CommandLine cmdLine) {
    
}
