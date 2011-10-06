#include "Command.hpp"

Command::Command(std::vector<std::string> & parameters,
                 std::string in,
                 std::string out,
                 std::string err,
                 bool outAppend,
                 bool errAppend
                 )
: parameters(parameters), in(in), out(out), err(err), outAppend(outAppend), errAppend(errAppend)
{
    execvector = 0;
}

Command::~Command() {
    if (execvector) {
        delete execvector;
    }
}

const char** Command::getExecv() {
    if (!execvector) {
        int size = parameters.size();
        execvector = new const char*[size+1];
        execvector[size] = 0;
        for (int i = 0; i < size; i++) {
            execvector[i] = parameters[i].c_str();
        }
    }
    return execvector;
}

std::string Command::getIn() {
    return in;
}

std::string Command::getOut() {
    return out;
}

std::string Command::getErr() {
    return err;
}

bool Command::getOutAppend() {
    return outAppend;
}

bool Command::getErrAppend() {
    return errAppend;
}
