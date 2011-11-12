#ifndef Builtin_HPP
#define Builtin_HPP

#include "Executor.hpp"

class Executor;

class Builtin {
    virtual int _run(const char*[], Executor*) = 0;
public:
    void run(const char*[], Executor*);
    virtual bool forkable();
};

//*******************************************

class CdCommand : public Builtin {
    int _run(const char*[], Executor*);
};

class PwdCommand : public Builtin {
    int _run(const char*[], Executor*);
};

class BgCommand : public Builtin {
    int _run(const char*[], Executor*);
};

class FgCommand : public Builtin {
    int _run(const char*[], Executor*);
};

class JobsCommand : public Builtin {
    int _run(const char*[], Executor*);
    bool forkable();
};

class ExitCommand : public Builtin {
    int _run(const char*[], Executor*);
};

#endif
