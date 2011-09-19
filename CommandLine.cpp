#include "CommandLine.hpp"

CommandLine::CommandLine(std::list<Command*> * pipeline, bool background) :
pipeline(pipeline), background(background) {
    iterator = this->pipeline->begin();
}


Command* CommandLine::next() {
    if (iterator == pipeline->end()) {
        return 0;
    } return *(iterator++);
}

CommandLine& CommandLine::operator= (CommandLine& commandLine) {
    pipeline = commandLine.pipeline;
    background = commandLine.background;
    iterator = this->pipeline->begin();
    return *this;
}

CommandLine::~CommandLine() {
    delete pipeline;
}
