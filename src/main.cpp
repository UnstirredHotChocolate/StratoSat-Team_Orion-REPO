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
#include "ORION_INA260.h"
/*
-----------------------------------------------------
SOFTWARE STATE VARIABLES:
*/
void stateBoolCheck();
state CURRENT_STATE = LAUNCH; //Start in the launch state
float deltaAlt; // Determine state changes based on altitude change
float currentAlt;
float previousAlt;
float threshold_alt;
float deltaPressure; // Determine state changes based on pressure change
float currentPressure;
float previousPressure;
unsigned long STATE_timerStart;
unsigned long current_time;

bool ASCENT_condition;
bool STABILIZATION_condition; 
bool DESCENT_condition;
bool LANDED_condition;
bool STATE_condition;
bool STATE_timerStarted;


/*
-----------------------------------------------------
CLASS CALLS and VARIABLE DECLARATIONS 
-----------------------------------------------------
*/
Error ErrorCode;
SHC_BME280 BMEsensor;
BNO055 BNOsensor;
M9N M9Nsensor;
// Adafruit_INA260 ina260 = Adafruit_INA260();
/*
-----------------------------------------------------
PID CONTROLLER STUFF
-----------------------------------------------------
*/
// Target coordinates for PID control
double target_X = 0.0;
double target_Y = 0.0;
double target_Z = 0.0;
// PID control variables
double derivative;
double integral;
double proportional;
double error_X;
double last_error_X;
double error_Y;
double last_error_Y;
double error_Z;
double last_error_Z;
double dt;
unsigned int last_time = millis();
double output_X; // 
double output_Y; // 
double output_Z; // 
// PID constants
double Kp = 1.0; // Proportional gain
double Ki = 1.0; // Integral gain
double Kd = 1.0; // Derivative gain
// PID declarations;
double PID_X(double error_X);
double PID_Y(double error_Y);
double PID_Z(double error_Z);
//--------------------------------------------------- 

void grabTelemetry(data &telemetry);
void printTelemetry(data telemetry);


// File telemetryFile; //Create a .csv file
void setup() {
  // put your setup code here, to run once:
  // ErrorCode = BNOsensor.init();
  ina260.begin();
  ErrorCode = BMEsensor.init(); // Initialize BME280 sensor
  // M9Nsensor.init(); // Initialize M9N sensor // TESTING: M9N
  Serial.begin(9600);
  // SD.begin();
  // telemetryFile = SD.open("telemetry.csv")
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  // Determine deltaTime
  //Do prefetches to get measurement 
  current_time = millis();
  M9Nsensor.prefetchData();
  // BMEsensor.prefetchData();
  // BNOsensor.prefetchData();
  digitalWrite(LED_BUILTIN, HIGH);
  if (ErrorCode == 1){
    Serial.println("Error Code: 1");
  }
  else {
    Serial.println("Error Code: 0");
  }
  grabTelemetry(telemetry);
  printTelemetry(telemetry);
  // stateBoolCheck();



  // //If CURRENT_STATE is STABILIZATION do PID control

  // error_X = target_X - telemetry.GYRO_X;
  // error_Y = target_Y - telemetry.GYRO_Y;
  // error_Z = target_Z - telemetry.GYRO_Z;
  // output_X = PID_X(error_X);
  // output_Y = PID_Y(error_Y);
  // output_Z = PID_Z(error_Z);
  // // analogWrite(PWM_X, output_X); //Actuate control for X
  // // analogWrite(PWM_Y, output_Y); //Actuate control for Y
  // // analogWrite(PWM_Z, output_Z); //Actuate control for Z 
  

  // // ADD MORE TELEMETRY DATA HERE LATER 
  //


}

void stateBoolCheck(){
    // Boolean state checks
  previousAlt = currentAlt;
  currentAlt = telemetry.ALTITUDE;
  previousPressure = currentPressure; 
  currentPressure = telemetry.PRESSURE;
  deltaAlt = currentAlt - previousAlt; // As altitude increases, altitude increases. Check the sign of deltaAlt. 
  deltaPressure = currentPressure - previousPressure; // As altitude increases, pressure decreases. Check the sign of deltaPressure.  
  ASCENT_condition = (deltaAlt > 0 && deltaPressure < 0) && (telemetry.ALTITUDE < threshold_alt); // Are you flying? 
  STABILIZATION_condition = telemetry.ALTITUDE > threshold_alt; // Are you chilling?
  DESCENT_condition = deltaAlt < 0 && deltaPressure > 0; // Are you falling?
  // Annie are you okay? Are you okay Annie?
//---------------------------------------------
// Set up timing flag
// //FINITE STATE MACHINE -- WIP
// if ((ASCENT_condition || STABILIZATION_condition || DESCENT_condition) && !STATE_timerStarted){
//   STATE_timerStarted = true; 
//   STATE_timerStart = current_time;
// }
// else if (!ASCENT_condition && !STABILIZATION_condition && !DESCENT_condition && !LANDED_condition){
//   STATE_timerStarted = false;
// }

// // Gauge CURRENT_STATE
// if (ASCENT_condition && current_time - STATE_timerStart >= 10000){
//   CURRENT_STATE = ASCENT;
// }
// else if (STABILIZATION_condition && current_time - STATE_timerStart >= 10000){
//   CURRENT_STATE = STABILIZATION;
// }
// else if (DESCENT_condition && current_time - STATE_timerStart >= 10000){
//   CURRENT_STATE = DESCENT;
// }
}
//17 reference vars // Directly reference the struct to save memory
void grabTelemetry
(data &telemetry)
{
    // Teensy outputs
  telemetry.MISSION_TIME = millis();
    // //BME outputs
  telemetry.ALTITUDE = BMEsensor.getAltitude();
  telemetry.PRESSURE = BMEsensor.getPressure();
  telemetry.HUMIDITY = BMEsensor.getHumidity();
  telemetry.TEMPERATURE = BMEsensor.getTemperature();
  // BNO outputs
  telemetry.ACCEL_X = BNOsensor.getAccelerationX();
  telemetry.ACCEL_Y = BNOsensor.getAccelerationY();
  telemetry.ACCEL_Z = BNOsensor.getAccelerationZ();
  telemetry.GYRO_X = BNOsensor.getGyroX();
  telemetry.GYRO_Y = BNOsensor.getGyroY();
  telemetry.GYRO_Z = BNOsensor.getGyroZ();
  telemetry.ORIENT_X = BNOsensor.getOrientationX();
  telemetry.ORIENT_Y = BNOsensor.getOrientationY();
  telemetry.ORIENT_Z = BNOsensor.getOrientationZ();
  // M9N outputs
  telemetry.UTC_TIME = M9Nsensor.getUnixTime();
  telemetry.GPS_ALTITUDE = M9Nsensor.getAltitude();
  telemetry.GPS_LATITUDE = M9Nsensor.getLatitude();
  telemetry.GPS_LONGITUDE = M9Nsensor.getLongitude();

}

void printTelemetry(data telemetry){
  //Teensy output
  Serial.print("CURRENT_STATE: ");
  if (CURRENT_STATE == LAUNCH){
    Serial.print("LAUNCH, ");
  }
  else if (CURRENT_STATE == ASCENT){
    Serial.print("ASCENT, ");
  }
  else if (CURRENT_STATE == STABILIZATION){
    Serial.print("STABILIZATION, ");
  }
  else if (CURRENT_STATE == DESCENT){
    Serial.print("DESCENT, ");
  }
  else if (CURRENT_STATE == LANDED){
    Serial.print("LANDED, ");
  }
  Serial.print("MISSION_TIME: ");
  Serial.print(telemetry.MISSION_TIME);
  //M9N Unix output
  Serial.print("ms, UTC_TIME : ");
  Serial.print(telemetry.UTC_TIME);
  //BME output
  Serial.print("s, ALTITUDE: ");
  Serial.print(telemetry.ALTITUDE);
  Serial.print("m, PRESSURE: ");
  Serial.print(telemetry.PRESSURE);
  Serial.print("hPa, TEMPERATURE: ");
  Serial.print(telemetry.TEMPERATURE);
  Serial.print("°C, HUMIDITY: ");
  Serial.print(telemetry.HUMIDITY);
  //BNO output
  Serial.print("%, ACCEL_X: ");
  Serial.println(telemetry.ACCEL_X);
  Serial.print("m/s^2, ACCEL_Y: ");
  Serial.print(telemetry.ACCEL_Y);
  Serial.print("m/s^2, ACCEL_Z: ");
  Serial.println(telemetry.ACCEL_Z);
  Serial.print("m/s^2, GYRO_X: ");
  Serial.print(telemetry.GYRO_X);
  Serial.print("rads, GYRO_Y: ");
  Serial.print(telemetry.GYRO_Y);
  Serial.print("rads, GYRO_Z: ");
  Serial.print(telemetry.GYRO_Z);
  Serial.print(", ORIENT_X: ");
  Serial.print(telemetry.ORIENT_X);
  Serial.print(", ORIENT_Y: ");
  Serial.print(telemetry.ORIENT_Y);
  Serial.print(", ORIENT_Z: ");
  Serial.print(telemetry.ORIENT_Z);
  // M9N outputs
  Serial.print(", GPS_ALT: ");
  Serial.print(telemetry.GPS_ALTITUDE);
  Serial.print(", GPS_LAT: ");
  Serial.print(telemetry.GPS_LATITUDE);
  Serial.print(", GPS_LONG: ");
  Serial.print(telemetry.GPS_LONGITUDE);
  printPowerReport(); // INA260 measurements
  Serial.println();



}

double PID_X(double error_X){
/*---------------------------------------------
  PID controller calculations*/ 
  unsigned int now = millis();
  dt = now - last_time;
  last_time = now;
  error_X = target_X - telemetry.GYRO_X; // 
  proportional = error_X;
  integral += error_X * dt;
  derivative = (error_X - last_error_X) / dt;
  last_error_X = error_X;
  output_X = (Kp * proportional) + (Ki * integral) + (Kd * derivative);
//----------------------------------------------
  return output_X;
  
}

double PID_Y(double error_Y){
  /*-------------------------------------------
    PID controller calculations*/ 
  unsigned int now = millis();
  dt = now - last_time; 
  last_time = now;
  error_X = target_X - telemetry.GYRO_Y;
  proportional = error_Y;
  integral += error_Y * dt;
  derivative = (error_Y - last_error_Y)/dt;
  last_error_Y = error_Y;
  output_Y = (Kp * proportional) + (Ki * integral) + (Kd * derivative);
//----------------------------------------------
  return output_Y;
}

double PID_Z(double error_Z){
  /*-------------------------------------------
  PID controller calculations*/ 
  unsigned int now = millis();
  dt = now - last_time; 
  last_time = now;
  error_X = target_X - telemetry.GYRO_Y;
  proportional = error_Y;
  integral += error_Z * dt;
  derivative = (error_Z - last_error_Z)/dt;
  last_error_Z = error_Y;
  output_Z = (Kp * proportional) + (Ki * integral) + (Kd * derivative);
  // Do something with the output
  return output_Z;
}


void printPowerReport() {
  Serial.print("CURRENT: ");
  Serial.print(ina260.readCurrent());
  Serial.print(" mA");

  Serial.print(", BUS VOLTAGE: ");
  Serial.print(ina260.readBusVoltage());
  Serial.print(" mV");

  Serial.print(", POWER: ");
  Serial.print(ina260.readPower());
  Serial.print(" mW");
}

