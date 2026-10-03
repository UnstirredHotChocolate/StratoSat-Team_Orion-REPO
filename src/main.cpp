#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

#include <Adafruit_BNO055.h>
//Custom header files
#include "ORION.h"
#include "ORION_BME280.h"



// put function declarations here:



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
  // BNO055 outputs
  // ADD MORE TELEMETRY DATA HERE LATER 



}

