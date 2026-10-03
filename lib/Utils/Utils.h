#ifndef Utils_h
#define Utils_h

#include "Arduino.h"


// An enum of the different errors you can get
enum Error {
    NO_ERROR = 0,
    BNO_ERROR = 1,
    M9N_ERROR = 2,
    BME_ERROR = 3,
};



// A couple of timer classes to allow for accurate timing in sensor readings
class Stopwatch {
protected:
    unsigned long startTime;
public: 
    Stopwatch();
    unsigned long getStartTime();
    unsigned long time();
    void reset();
};
class Timer: public Stopwatch {
protected:
    unsigned long duration; // ms
public:
    Timer(unsigned long duration);
    bool isComplete();
    unsigned long timeRemaining();
    float progress();
    void setDuration(unsigned long newDuration);
    void reset();
    void reset(unsigned long newDuration);
};


// Sensor superclass, used to make all of your sensors match up in the way you collect data from them
class Sensor {    
protected:
    Timer dataReady = Timer(0);
public:
    virtual Error init() = 0;
    virtual bool prefetchData() = 0;
};



#endif