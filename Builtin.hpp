#ifndef Builtin_HPP
#define Builtin_HPP

#include "Executor.hpp"

class Executor;

/**
 *	\brief Classe abstrata para comandos Built-In.
 *	\sa Executor
 */
class Builtin {
    virtual int _run(const char*[], Executor*) = 0;
public:
	/**
	 *	\brief Executa um comando built in.
	 *	\param args Argumentos do comando, incluindo seu nome.
	 *	\param executor Pointeiro para uma instancia de Executor.
	 */
    void run(const char*args[], Executor *executor);
    virtual bool forkable();
};

//*******************************************

/**
 * \brief Classe que implementa o comando cd.
 */
class CdCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando pwd.
 */
class PwdCommand : public Builtin {
    int _run(const char*[], Executor*);
};


/**
 *	\brief Classe que implementa o comando bg.
 */
class BgCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando fg.
 */
class FgCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando jobs.
 */
class JobsCommand : public Builtin {
    int _run(const char*[], Executor*);
    bool forkable();
};

/**
 *	\brief Classe que implementa o comando exit.
 */
class ExitCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando kill.
 */
class KillCommand : public Builtin {
    int _run(const char*[], Executor*);
};

#endif
