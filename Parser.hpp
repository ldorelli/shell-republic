#ifndef Parser_hpp
#define Parser_hpp

#include <string>
#include "CommandLine.hpp"

/**
 * \brief Utilizado para converter a entrada do usuario em uma CommandLine
 *
 *        A linha de usuario deve ter a seguinte forma\n
 *        <comando><paramentro>...<parametro>|...|<comando>...[< <entrada>][[1>|>|>>][2>]<saida>|&><saida>][&]
 */
class Parser {
    std::string line;
    unsigned index;
    
    std::string nextWord();
    
public:
    Parser();
    /**
     * \return Le e interpreta uma linha de comando dada pelo usuario no stdin
     */
    CommandLine* readCommandLine();
    /**
     * \return Verdadeiro caso esteja em uma nova linha de comando (nao necessariamente endl,
     *         mas fim da pipeline por &
     */
    bool newLine();
};

#endif
