#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>

// #include <Adafruit_BNO055.h>
//Custom header files
#include "ORION.h"
#include "SHC_BME280.h"
#include "SHC_BNO055.h"
#include "SHC_M9N.h"
// #include "ORION_INA260.h"
// put function declarations here:
Error ErrorCode;
SHC_BME280 BMEsensor;
BNO055 BNOsensor;
M9N M9Nsensor;

// File telemetryFile;
void setup() {
  // put your setup code here, to run once:
  // ErrorCode = BNOsensor.init();
  BMEsensor.init();
  ErrorCode = M9Nsensor.init();
  Serial.begin(9600);
  // SD.begin();
  // telemetryFile = SD.open("telemetry.csv")
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Do prefetches to get measurement
  M9Nsensor.prefetchData();
  BMEsensor.prefetchData();
  BNOsensor.prefetchData();
  if (ErrorCode == 2){
    Serial.println("Error Code: 2");
  }
  else {
    Serial.println("Error Code: 0");
  }
  // printPowerReport();
  grabTelemetry(telemetry.ALTITUDE, telemetry.PRESSURE, telemetry.TEMPERATURE, telemetry.HUMIDITY, 
    telemetry.ACCEL_X, telemetry.ACCEL_Y, telemetry.ACCEL_Z, telemetry.GYRO_X, telemetry.GYRO_Y, telemetry.GYRO_Z,
    telemetry.ORIENT_X, telemetry.ORIENT_Y, telemetry.ORIENT_Z, 
    telemetry.MISSION_TIME, telemetry.GPS_ALTITUDE, telemetry.GPS_LATITUDE, telemetry.GPS_LONGITUDE);
  printTelemetry  
  ( telemetry.ALTITUDE, telemetry.PRESSURE, telemetry.TEMPERATURE, telemetry.HUMIDITY, 
    telemetry.ACCEL_X, telemetry.ACCEL_Y, telemetry.ACCEL_Z, telemetry.GYRO_X, telemetry.GYRO_Y, telemetry.GYRO_Z,
    telemetry.ORIENT_X, telemetry.ORIENT_Y, telemetry.ORIENT_Z, 
    telemetry.MISSION_TIME, telemetry.GPS_ALTITUDE, telemetry.GPS_LATITUDE, telemetry.GPS_LONGITUDE);
  // // ADD MORE TELEMETRY DATA HERE LATER 
  // //


}

//17 reference vars // Directly reference the variable to save memory
void grabTelemetry
(float &altitude, float &pressure, float &temperature, float &humidity, 
  double &accel_X, double &accel_Y, double &accel_Z, double &gyro_X, double &gyro_Y, double &gyro_Z, double &orient_X, double orient_Y, double &orient_Z,
  unsigned int &mission_time, double &gps_alt, double &gps_lat, double &gps_long)
{
    // //BME outputs
  altitude = BMEsensor.getAltitude();
  pressure = BMEsensor.getPressure();
  humidity = BMEsensor.getHumidity();
  temperature = BMEsensor.getTemperature();
  // BNO outputs
  accel_X = BNOsensor.getAccelerationX();
  accel_Y = BNOsensor.getAccelerationY();
  accel_Z = BNOsensor.getAccelerationZ();
  gyro_X = BNOsensor.getGyroX();
  gyro_Y = BNOsensor.getGyroY();
  gyro_Z = BNOsensor.getGyroZ();
  orient_X = BNOsensor.getOrientationX();
  orient_Y = BNOsensor.getOrientationY();
  orient_Z = BNOsensor.getOrientationZ();
  mission_time = M9Nsensor.getUnixTime();
  gps_alt = M9Nsensor.getAltitude();
  gps_lat = M9Nsensor.getLatitude();
  gps_long = M9Nsensor.getLongitude();

}

void printTelemetry(float altitude, float pressure, float temperature, float humidity, 
  double accel_X, double accel_Y, double accel_Z, double gyro_X, double gyro_Y, double gyro_Z, double orient_X, double orient_Y, double orient_Z,
unsigned int mission_time, double gps_alt, double gps_lat, double gps_long){
  //BME output
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.print(", Pressure: ");
  Serial.print(pressure);
  Serial.print(", Temperature: ");
  Serial.print(temperature);
  Serial.print(", Humidity: ");
  Serial.print(humidity);
  //BNO output
  Serial.print(", ACCEL_X: ");
  Serial.println(accel_X);
  Serial.print(", ACCEL_Y: ");
  Serial.print(accel_Y);
  Serial.print(", ACCEL_Z: ");
  Serial.println(accel_Z);
  Serial.print(", GYRO_X: ");
  Serial.print(gyro_X);
  Serial.print(", GYRO_Y: ");
  Serial.print(gyro_Y);
  Serial.print(", GYRO_Z: ");
  Serial.print(gyro_Z);
  Serial.print(", ORIENT_X: ");
  Serial.print(orient_X);
  Serial.print(", ORIENT_Y: ");
  Serial.print(orient_Y);
  Serial.print(", ORIENT_Z: ");
  Serial.print(orient_Z);
  // M9N outputs
  Serial.print("MISSION_TIME: ");
  Serial.print(mission_time);
  Serial.print(", GPS_ALT: ");
  Serial.print(gps_alt);
  Serial.print(", GPS_LAT: ");
  Serial.print(gps_lat);
  Serial.print(", GPS_LONG: ");
  Serial.print(gps_long);



}