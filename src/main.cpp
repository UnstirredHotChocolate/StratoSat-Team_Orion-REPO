#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

#include <Adafruit_BNO055.h>
//Custom header files
#include "ORION.h"
#include "SHC_BME280.h"
#include "SHC_BNO055.h"
#include "SHC_M9N.h"
// put function declarations here:
Error ErrorCode;
SHC_BME280 BMEsensor;
BNO055 BNOsensor;
M9N M9Nsensor;
// File telemetryFile;
void setup() {
  // put your setup code here, to run once:
  BMEsensor.init();
  ErrorCode = BNOsensor.init();
  Serial.begin(9600);
  // SD.begin();
  // telemetryFile = SD.open("telemetry.csv")
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  // BNOsensor.prefetchData(); // Prefetch data from the BNO055 sensor
  // M9Nsensor.prefetchData(); // Prefetch data from the M9N sensor
  BMEsensor.prefetchData();
  BNOsensor.prefetchData();
  M9Nsensor.prefetchData();
      if (ErrorCode == 1){
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Error Code: 1");
  }
  else {
    Serial.print("Error Code: 0");
  }
  // digitalWrite(LED_BUILTIN, HIGH);
  //BME outputs
  telemetry.ALTITUDE = BMEsensor.getAltitude();
  telemetry.PRESSURE = BMEsensor.getPressure();
  telemetry.TEMPERATURE = BMEsensor.getTemperature();
  telemetry.HUMIDITY = BMEsensor.getHumidity();
  // digitalWrite(LED_BUILTIN, HIGH);
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
  Serial.print("Altitude: ");
  Serial.println(telemetry.ALTITUDE);
  Serial.print("Pressure: ");
  Serial.println(telemetry.PRESSURE);
  Serial.print("Temperature: ");
  Serial.println(telemetry.TEMPERATURE);
  Serial.print("Humidity: ");
  Serial.println(telemetry.HUMIDITY);
  Serial.print("ACCEL_X: ");
  Serial.println(telemetry.ACCEL_X);
  Serial.print("ACCEL_Y: ");
  Serial.println(telemetry.ACCEL_Y);
  Serial.print("ACCEL_Z: ");
  Serial.println(telemetry.ACCEL_Z);
  Serial.print("GYRO_X: ");
  Serial.println(telemetry.GYRO_X);
  Serial.print("GYRO_Y: ");
  Serial.println(telemetry.GYRO_Y);
  Serial.print("GYRO_Z: ");
  Serial.println(telemetry.GYRO_Z);
  Serial.print("ORIENT_X: ");
  Serial.println(telemetry.ORIENT_X);
  Serial.print("ORIENT_Y: ");
  Serial.println(telemetry.ORIENT_Y);
  Serial.print("ORIENT_Z: ");
  Serial.println(telemetry.ORIENT_Z);
  // digitalWrite(LED_BUILTIN, HIGH);
  // // M9N outputs
  // telemetry.MISSION_TIME = M9Nsensor.getUnixTime();
  // telemetry.GPS_ALTITUDE = M9Nsensor.getAltitude();
  // telemetry.GPS_LONGITUDE = M9Nsensor.getLongitude();
  // telemetry.GPS_LATITUDE = M9Nsensor.getLatitude();
  // // ADD MORE TELEMETRY DATA HERE LATER 
  // //


}

