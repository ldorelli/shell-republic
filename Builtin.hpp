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

	/**
	 *	\brief Se o comando pode ser executado como um proceso independente.
	 */
    virtual bool forkable();
};

//*******************************************

/**
 * \brief Classe que implementa o comando cd.
 * 		
 * Sintaxe: cd <diretorio>
 */
class CdCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando pwd.
 *
 * Sintaxe: pwd			
 */
class PwdCommand : public Builtin {
    int _run(const char*[], Executor*);
    bool forkable();
};


/**
 *	\brief Classe que implementa o comando bg.
 *
 *  Sintaxe: bg %<JOBID> | 	bg\n
 *	Se o comando for chamado sem o JOBID, sera utilizado o job mais recentemente aberto,
 *		colocado em foreground ou background ou, caso ja tenha sido fechado, o mais antigo aberto.
 *		
 */
class BgCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando fg.
 *
 *  Sintaxe: fg %<JOBID> | 	fg \n
 *		Se o comando for chamado sem o JOBID, sera utilizado o job mais recentemente aberto,
 *		colocado em foreground ou background ou, caso ja tenha sido fechado, o mais antigo aberto.
 *		
 */
class FgCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando jobs.
 *	
 * Sintaxe: jobs
 */
class JobsCommand : public Builtin {
    int _run(const char*[], Executor*);
    bool forkable();
};

/**
 *	\brief Classe que implementa o comando exit.
 *
 * Sintaxe: exit, quit
 */
class ExitCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 *	\brief Classe que implementa o comando kill.
 *
 *  Sintaxe: kill %<JOBID>
 *
 */
class KillCommand : public Builtin {
    int _run(const char*[], Executor*);
};

/**
 * \brief Class que implementa o comando echo.
 *	
 * 
 * Sintaxe: echo <arg1> <arg2> ... <arg_n> 
 */ 
 class EchoCommand : public Builtin{
	int _run(const char*[],Executor*);
	bool forkable();
 };

#endif
