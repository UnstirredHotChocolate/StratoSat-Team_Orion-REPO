 
#include "Utils.h"
#include "SHC_BME280.h"
#include "Arduino.h"
#include <BME280.h>  

Error SHC_BME280::init() {
    // BME280::Settings settings;
    // settings.i2cAddress = 0x77;
    if (!bme.begin()) {
        return BME_ERROR;
    }
    bme.mode(ForcedMode); // Force one measurement to begin
    bme.setOversampling(TemperatureSensor,Oversample16);
    bme.setOversampling(HumiditySensor,   Oversample16);
    bme.setOversampling(PressureSensor,   Oversample16);
    bme.iirFilter(IIR16);
    return NO_ERROR;
}

bool SHC_BME280::prefetchData() {
    // If the BME is in sleep mode it means a measurement has been completed
    if (bme.mode() == SleepMode) {
        bme.mode(SleepMode); // Must set it to sleep mode again to avoid library trying to take another reading
        // Therefore we collect the data here...
        long fetchedTemperature, fetchedHumidity, fetchedPressure;
        bme.getSensorData(fetchedTemperature, fetchedHumidity, fetchedPressure);
        pressure = (float)fetchedPressure / 100.0;
        temperature = (float)fetchedTemperature / 100.0;
        humidity = (float) fetchedHumidity / 100.0;
        altitude = 44330.0*(1.0 - pow(((float) fetchedPressure / 100.0) / 1013.25, 0.1903));
        // ...then the BME to forced mode to take another measurement
        bme.mode(ForcedMode);
        return true;
    }
    return false;
}

float SHC_BME280::getPressure() {
    return pressure;
}

float SHC_BME280::getTemperature() {
    return temperature;
}

float SHC_BME280::getAltitude() { 
    return altitude;
}

float SHC_BME280::getHumidity(){
    return humidity;
}