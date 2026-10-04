#ifndef BNO055_h
#define BNO055_h

#include "Arduino.h"
#include <Adafruit_BNO055.h>
#include "Utils.h"

class BNO055 : public Sensor {
private:
    double accelX = 0.0;
    double accelY = 0.0;
    double accelZ = 0.0;
    double gyroX = 0.0;
    double gyroY = 0.0;
    double gyroZ = 0.0;
    double orientX = 0.0;
    double orientY = 0.0;
    double orientZ = 0.0;
public:
    Error init();
    bool prefetchData();
    double getAccelerationX();
    double getAccelerationY();
    double getAccelerationZ();
    double getGyroX();
    double getGyroY();
    double getGyroZ();
    double getOrientationX();
    double getOrientationY();
    double getOrientationZ();
};

#endif