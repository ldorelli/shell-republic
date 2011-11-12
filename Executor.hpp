#ifndef Executor_hpp
#define Executor_hpp

#include <string>
#include <vector>
#include <list>
#include <sys/types.h>
#include "Command.hpp"
#include "CommandLine.hpp"
#include <termios.h>
#include "Builtin.hpp"
#include <map>

class Builtin;

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
    int run(Command* command, int & firstPipedPid, bool isBackground, std::map<std::string, Builtin*>&,
            int fdIn = 0, int fdOut = 1, int fdErr = 2);
    int foreground;
    int lastForeground;
public:
    Executor();
    void run(CommandLine* commandLine, std::map<std::string, Builtin*>&);
    void cleanUp ();
    void setForeground( int pid );
    int getLastForeground();
    void setLastForeground(int pid);
    std::list<Job> * getJobs();
};

#endif
