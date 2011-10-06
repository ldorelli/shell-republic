#include <iostream>
#include "CommandLine.hpp"
#include "Parser.hpp"
#include "MyTypo.hpp"
#include "Executor.hpp"

int main (int argc, char *argv[]) {
    Parser parser;
    Executor executor;
    std::string presentation("shell$ ");
    while (true) {
        if (parser.newLine()) std::cout << presentation;
        CommandLine *cl = parser.readCommandLine();
        executor.run(cl);
        delete cl;
    }
    return 0;
}
