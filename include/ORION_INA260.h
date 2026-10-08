#ifndef ORION_INA260_H
#define ORION_INA260_H

#include <Arduino.h>
#include <Adafruit_INA260.h>

Adafruit_INA260 ina260 = Adafruit_INA260();

void printPowerReport();

#endif // ORION_INA260_H