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

bool CommandLine::hasNext() {
    return iterator != pipeline->end();
}

CommandLine::~CommandLine() {
    delete pipeline;
}

bool CommandLine::isBackground() {
    return background;
}
