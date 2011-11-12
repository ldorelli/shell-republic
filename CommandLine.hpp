#ifndef CommandLine_hpp
#define CommandLine_hpp

#include "Command.hpp"
#include <list>
#include <string>


/**
 * \brief Represeta uma linha de comando.
 *
 *        Uma linha de comando pode conter uma serie de comandos em pipeline e
 *        termina quando ha um & ou um final de linha.
 *        A linha de comando pode ser gerada por uma instancia de Parser
 *
 * \sa Command, Parser
 */
class CommandLine {
    std::list<Command*>* pipeline;
    bool background;
    std::list<Command*>::iterator iterator;
    
public:
    CommandLine& operator= (CommandLine& commandLine);
    /**
     * \brief Construtor
     *
     * \param pipeLine lista de comandos representando a pipeline
     * \param background verdadeira, caso a pipeline deva ser executada em segundo plano 
     */
    CommandLine(std::list<Command*> * pipeline, bool background);
    ~CommandLine();
    /**
     * \brief Navega pela pipeline existente
     *        A cada utilizacao, o comando extraido e retirado completamente da pipeline existente
     *
     * \return Um ponteiro para o proximo comando da pipeline existente ou NULL
     *         o ultimo comando retirado tenha sido o ultimo
     */
    Command* next();
    /**
     * \return Verdadeiro se a pipeline nao esta vazia
     */
    bool hasNext();
    /**
     * \return Verdadeiro se a pipeline deve ser executada em segundo plano
     */
    bool isBackground();
};

#endif
