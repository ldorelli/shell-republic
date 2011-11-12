#ifndef Handlers_HPP
#define Handlers_HPP

namespace handlers {
    void sigChildHandler( int );
    void sigTStpHandler( int );
    bool getDeathStatus();
    void setDeathStatusFalse();
}

#endif
