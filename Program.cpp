#include <iostream>
#include "CommandLine.hpp"
#include "Parser.hpp"
#include "MyTypo.hpp"

int main (int argc, char *argv[]) {
    Parser parser;
    std::string presentation("shell$ ");
    while (true) {
        if (parser.newLine()) std::cout << presentation;
        CommandLine *cl = parser.readCommandLine();
        Command* cmd;
        while (cmd = cl->next()) {
            const char ** execvector;
            execvector = cmd->getExecv();
            for (int i = 0; execvector[i] != 0; i++) {
                
            }
        }
        delete cl;
    }
    return 0;
}
