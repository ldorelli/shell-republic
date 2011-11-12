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

/*! \brief Responsavel pela execucao.
 *         Executa uma linha de comando.
 *	
 * Uma instancia de Executor guarda a lista de processos que sao extraidos das linhas de comando recebidas.
 * Controla a atualizacao dos estados dos processos.
 * 
 * \sa CommandLine, Command
 */
class Executor {
public:
	/** \brief Estrutura que representa um job.
	 *			Guarda os dados necessarios para controlar processos.
	 *
	 *	\sa Executor
	 */
    struct Job {
        std::string name;
        pid_t       pid;
        unsigned	jobid;
        unsigned    groupid;
        bool        stopped;
        bool        dead;
        Job ();
    };

private:
    std::list<Job> jobs;
    int run(Command* command, int & firstPipedPid, bool isBackground, std::map<std::string, Builtin*>&,
            int fdIn = 0, int fdOut = 1, int fdErr = 2);
    unsigned foreground;
    unsigned lastForeground;
public:
    Executor();
    /**
	 * \brief Executa uma linha de comando.
	 *		Executa os comandos de uma CommandLine.
	 *		
	 *	\sa Parser, CommandLine, Builtin
	 * 	\param commandLine Linha de comando a ser executada.
	 *  \param bCommands Mapa que associa nomes de comandos internos a comandos internos.
	 *
	 */
	void run(CommandLine* commandLine, std::map<std::string, Builtin*>& bCommands);

	/**
	 * \brief Realiza ajustes na lista de jobs, se necessario.
	 * 		Percorre a lista de jobs, removendo, atualizando status, quando necessario.
	 *
	 *	\sa Job, handlers
	 *	
	 */
    void cleanUp ();

	/**
	 *
	 * \brief Controle interno.
	 *			Atualiza o job que esta em foreground.
	 *
	 */
    void setForeground( int pid );

	/**
	 * \return JobID do processo que deve ir para foreground.
	 */
    unsigned getLastForeground();
    
	/**
	 *	\brief Altera qual processo deve ir para foreground.
	 *			Quando ocorrer uma chamada a fg, o processo configurado sera utilizado.
	 *	
	 *	\sa FgCommand, BgCommand
	 *	\param jobid JobID do processo a ser configurado.
	 */
	void setLastForeground(unsigned jobid);

    /**
	 * \return Ponteiro para a lista de jobs.
	 * 
	 */
	std::list<Job> * getJobs();
};

#endif
