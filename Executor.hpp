#ifndef __Executor_HPP__
#define __Executor_HPP__

#include <string>
#include <vector>
#include <list>
#include <sys/types.h>
#include "Command.hpp"
#include "CommandLine.hpp"


class Executor {
public:
    struct Job {
        std::string name;
        pid_t pid;
        unsigned jobid;
        bool    stopped;
        Job ();
    };

private:
    std::list<Job> jobs;
    int run(Command* command, int fdIn = 0, int fdOut = 1, int fdErr = 2);
public:
    void run(CommandLine* commandLine);
    std::list<Job> & getJobs();
};

#endif
