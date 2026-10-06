#ifndef SHC_BME280_h
#define SHC_BME280_h

#include "Arduino.h"
#include "Utils.h"
#include <BME280.h>



class SHC_BME280: Sensor {
private:
    BME280_Class bme;
    float pressure = 0.0;
    float temperature = 0.0;
    float humidity = 0.0;
    float altitude = 0.0;
public:
    Error init();
    bool prefetchData();
    float getPressure();
    float getTemperature();
    float getAltitude();
    float getHumidity();
};

#endif