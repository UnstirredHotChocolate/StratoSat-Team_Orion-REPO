#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

#include <Adafruit_BNO055.h>
//Custom header files
#include "ORION.h"
#include "ORION_BME280.h"
#include "SHC_BNO055.h"
#include "SHC_M9N.h"

// put function declarations here:

BNO055 BNOsensor;
M9N M9Nsensor;
File telemetryFile;
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  SD.begin();
  telemetryFile = SD.open("telemetry.csv");


}

void loop() {
  // put your main code here, to run repeatedly:
  readEnviroValues(telemetry.TEMPERATURE, telemetry.PRESSURE, telemetry.ALTITUDE, telemetry.HUMIDITY);
  BNOsensor.prefetchData(); // Prefetch data from the BNO055 sensor
  M9Nsensor.prefetchData(); // Prefetch data from the M9N sensor
  // BNO055 outputs
  telemetry.ACCEL_X = BNOsensor.getAccelerationX();
  telemetry.ACCEL_Y = BNOsensor.getAccelerationY();
  telemetry.ACCEL_Z = BNOsensor.getAccelerationZ();
  telemetry.GYRO_X = BNOsensor.getGyroX();
  telemetry.GYRO_Y = BNOsensor.getGyroY();
  telemetry.GYRO_Z = BNOsensor.getGyroZ();
  telemetry.ORIENT_X = BNOsensor.getOrientationX();
  telemetry.ORIENT_Y = BNOsensor.getOrientationY();
  telemetry.ORIENT_Z = BNOsensor.getOrientationZ();
  // ADD MORE TELEMETRY DATA HERE LATER 
  // M9N outputs
  


}

