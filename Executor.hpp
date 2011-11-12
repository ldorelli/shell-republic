#ifndef __Executor_HPP__
#define __Executor_HPP__

#include <string>
#include <vector>
#include <list>
#include <sys/types.h>
#include "Command.hpp"
#include "CommandLine.hpp"
#include <termios.h>


class Executor {
public:
    struct Job {
        std::string name;
        pid_t       pid;
        unsigned    jobid;
        unsigned    groupid;
        bool        stopped;
        bool        dead;
        Job ();
    };

private:
    std::list<Job> jobs;
    int run(Command* command, int & firstPipedPid, bool isBackground,
            int fdIn = 0, int fdOut = 1, int fdErr = 2);
    int foreground;
public:
    Executor();
    void run(CommandLine* commandLine);
    void cleanUp ();
    std::list<Job> & getJobs();
};

#endif
