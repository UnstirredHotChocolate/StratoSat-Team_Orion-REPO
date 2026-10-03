#include "Arduino.h"
#include "Utils.h"
#include <EEPROM.h>
#include <Adafruit_BNO055.h>
#include "SHC_BNO055.h"


Adafruit_BNO055 bno = Adafruit_BNO055(55);
sensors_event_t event;

Error BNO055::init() {
    if(!bno.begin()) {
        return BNO_ERROR;  
    }
    return NO_ERROR;
}

bool BNO055::prefetchData() {
    imu::Vector<3> fetchedAcceleration = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    accelX = fetchedAcceleration.x();
    accelY = fetchedAcceleration.y();
    accelZ = fetchedAcceleration.z();

    imu::Vector<3> fetchedGyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    gyroX = fetchedGyro.x();
    gyroY = fetchedGyro.y(); 
    gyroZ = fetchedGyro.z();

    imu::Vector<3> fetchedOrientation = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    orientX = fetchedOrientation.x(); 
    orientY = fetchedOrientation.y();
    orientZ = fetchedOrientation.z();

    return true;
}

double BNO055::getAccelerationX() {
    return accelX;
}
double BNO055::getAccelerationY() {
    return accelY;
}
double BNO055::getAccelerationZ() {
    return accelZ;
}

double BNO055::getGyroX() {
    return gyroX;
}
double BNO055::getGyroY() {
    return gyroY;
}
double BNO055::getGyroZ() {
    return gyroZ;
}

double BNO055::getOrientationX() {
    return orientX;
}
double BNO055::getOrientationY() {
    return orientY;
}
double BNO055::getOrientationZ() {
    return orientZ;
}