#ifndef Handlers_HPP
#define Handlers_HPP

namespace handlers {
    void sigChildHandler( int );
    bool getDeathStatus();
    void setDeathStatusFalse();
    void setDeathStatusTrue();
}

#endif
