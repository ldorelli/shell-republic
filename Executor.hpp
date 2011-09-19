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
    };

private:
    std::list<Job> jobs;
    void run(Command* command, int& fdIn = 0, int& fdOut = 1, int& fdErr = 2);
public:
    void run(CommandLine commandLine);
    std::vector<Job> & getJobs();
};

#endif
