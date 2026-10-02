#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <SD.h>
#include <Wire.h>
#include "ORION.h"


// put function declarations here:



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SD.open("telemetry.txt");

}

void loop() {
  // Serial.write(TELEMETRY DATA)
  // put your main code here, to run repeatedly:



}

