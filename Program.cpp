#include <iostream>
#include "CommandLine.hpp"
#include "Parser.hpp"

int main (int argc, char *argv[]) {
    Parser parser;
    while (true) {
        CommandLine cl = parser.readCommandLine();
        Command* cmd;
        while (cmd = cl.next()) {
            const char ** execvector;
            execvector = cmd->getExecv();
            for (int i = 0; execvector[i] != 0; i++) {
                std::cout << execvector[i] << std::endl;
            }
        }
    }
    return 0;
}
