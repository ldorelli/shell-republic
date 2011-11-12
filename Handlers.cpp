#include "Handlers.hpp"
#include <iostream>

bool deathStatus = false;

void handlers::sigChildHandler( int signum ) {
    deathStatus = true;
}

bool handlers::getDeathStatus() {
    return deathStatus;
}

void handlers::setDeathStatusFalse() {
    deathStatus = false;
}

void handlers::sigTStpHandler( int signum ) {
    std::cerr << "I'm here oh yeah\nYeah baby yeah!\n";
}
