#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "ORION.h"
#include "ORION_BME280.h"

#include <SD.h>
#include <SPI.h>

// put function declarations here:

// extern data telemetry; // Call struct 'data' for telemetry from "Orion.h";

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
  telemetryFile.print("ORION, ");
  telemetryFile.print(telemetry.TEMPERATURE);
  telemetryFile.print(", ");
  telemetryFile.print(telemetry.PRESSURE);
  telemetryFile.print(", ");
  telemetryFile.print(telemetry.ALTITUDE);
  telemetryFile.print(", ");
  telemetryFile.print(telemetry.HUMIDITY);
  telemetryFile.println(); 
  // ADD MORE TELEMETRY DATA HERE LATER



}

