#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_INA260.h>
#include <elapsedMillis.h>
#include <cmath>
// #include <Adafruit_BNO055.h>
//Custom header files
#include "ORION.h" 
#include "SHC_BME280.h"
#include "SHC_BNO055.h"
#include "SHC_M9N.h"
/*

-----------------------------------------------------
SOFTWARE STATE VARIABLES:
*/

#define LED_RED 15 //Add actual LED pin num here and below
#define LED_WHITE 18

#define CCW_SOLENOID 2
#define CW_SOLENOID 21

state CURRENT_STATE = STABILIZATION; //Start in the launch state// TESTING
void CheckState(state &CURRENT_STATE, data telemetry);


unsigned long previousMillis = 0; 
unsigned long currentTime;
bool ASCENT_condition;
bool STABILIZATION_condition; 
bool DESCENT_condition;
bool LANDED_condition;

elapsedMillis timeElapsed; 
elapsedMillis timeElapsedBlink; 

/*
-----------------------------------------------------
CLASS CALLS and VARIABLE DECLARATIONS 
-----------------------------------------------------
*/
Error Code1; 
Error Code2;
Error Code3;
SHC_BME280 BMEsensor;
BNO055 BNOsensor;
M9N M9Nsensor;
//Adafruit_INA260 ina260 = Adafruit_INA260();


// BANG BANG CONTROLLER

const double deadzone = 15 * (PI/180); 
const double desired_gyro_speed = 0;
int output; 
//----------------------------------
//Telemetry Prototypes
void grabTelemetry(data &telemetry);
void printTelemetry(state CURRENT_STATE, data &telemetry);
void printTelemetry1(state CURRENT_STATE, data &telemetry);
// Blink Variables
boolean ledState = LOW;
const unsigned int interval = 1000; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  Code1 = BNOsensor.init(); // Initialize BNO sensor
  Code2 = M9Nsensor.init(); // Initialize M9N sensor 
  Code3 = BMEsensor.init(); // Initialize BME280 sensor
  delay(1000);
  //ina260.begin();
  Wire.begin();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Determine program error 
  //Do prefetches to get measurement 
  //Timer Stuff Here
  BMEsensor.prefetchData();
  BNOsensor.prefetchData();
  M9Nsensor.prefetchData(); 
  grabTelemetry(telemetry);
  printTelemetry(CURRENT_STATE, telemetry);
  printTelemetry1(CURRENT_STATE, telemetry);

  // if ((Code1 == 1 || Code2 == 2 || Code3 == 3)){ 
  //   //If it fails at the start or if it fails during runtime. 
  //   //Blink red
  //   if (timeElapsedBlink > interval)
  //   {
  //     ledState = !ledState; // toggle the state from HIGH to LOW to HIGH to LOW ...
  //     digitalWrite(LED_BUILTIN, ledState); //REPLACE LED_BUILTIN
  //     timeElapsedBlink = 0; // reset the counter to 0 so the counting starts over...
  //   }
  // }
  // else if (Code1 == 0 && Code2 == 0 && Code3 == 0){
  //   //Blink white
  // if (timeElapsedBlink > interval)
  // {
  //   ledState = !ledState; // toggle the state from HIGH to LOW to HIGH to LOW ...
  //   digitalWrite(LED_BUILTIN, ledState);
  //   timeElapsedBlink = 0; // reset the counter to 0 so the counting starts over...
  // }
  // }
  //End of timer.

  // if (timeElapsedBlink > interval)
  // {
  //   ledState = !ledState; // toggle the state from HIGH to LOW to HIGH to LOW ...
  //   digitalWrite(LED_BUILTIN, ledState);
  //   timeElapsedBlink = 0; // reset the counter to 0 so the counting starts over...
  // }
  // CheckState(CURRENT_STATE, telemetry); UNCOMMENT FUNCTION AFTER STAB TEST
  //BangBang Backup

  if(CURRENT_STATE == STABILIZATION){
    if (abs(telemetry.GYRO_X - desired_gyro_speed) < deadzone){ 
      output = 0; 
    }
    else if (telemetry.GYRO_X < desired_gyro_speed){
      output = -1; 
    }
    else if(telemetry.GYRO_X > desired_gyro_speed){
      output = 1; 
    }
    if (output == 0){ //Don't control
      digitalWrite(CCW_SOLENOID, LOW);
      digitalWrite(CW_SOLENOID, LOW);
    }
    else if (output == 1){ //Clockwise error, counter clockwise response
      digitalWrite(CCW_SOLENOID, HIGH);
      digitalWrite(CW_SOLENOID, LOW);
    }
    else if (output == -1){ //Counterclockwise error, clockwise response
      digitalWrite(CCW_SOLENOID, LOW);
      digitalWrite(CW_SOLENOID, HIGH);
    }
  }
  //digitalWrite(SolenoidPIN, output);
  //delay(1000);
}

void CheckState(state &CURRENT_STATE, data telemetry){
  //------------------------------------------------
  /*
  VARIABLES 
  *///---------------------------------------
  static unsigned int SinceAscentTrue;
  static unsigned int SinceStabilizationTrue;
  static unsigned int SinceDescentTrue;
  static unsigned int SinceLandedTrue; 

//  //Time Starts
  //Durations
  const unsigned long duration = 15000; //15 seconds. 
  const int LandedDuration = 600000; // 600 seconds.

  static float deltaAlt = 0; // Determine state changes based on altitude change
  static float currentAlt = 0;
  static float previousAlt;
  const float threshold_alt = 18000.0;
  static float deltaPressure; // Determine state changes based on pressure change
  static float currentPressure = 0; 
  // static float currentPressure = 1000; //TESTING PURPOSES; 
  static float previousPressure;
  static double GPSAlt = 0;
  static double previousGPSAlt = 0;
  static double deltaGPSAlt;
    // Key values
  previousAlt = currentAlt;
  currentAlt = telemetry.ALTITUDE;
  previousPressure = currentPressure; 
  currentPressure = telemetry.PRESSURE;
  previousGPSAlt = GPSAlt;
  // GPSAlt = telemetry.GPS_ALTITUDE;
  deltaAlt = currentAlt - previousAlt; // As altitude increases, check the sign of deltaAlt. 
  deltaPressure = currentPressure - previousPressure; // As altitude increases, pressure decreases. Check the sign of deltaPressure.  
  deltaGPSAlt = GPSAlt - previousGPSAlt;
  //CONDITIONS--------------------------------------------------------------------------------------------------------------------------
  ASCENT_condition = (deltaGPSAlt > 0 || deltaAlt > 0) && deltaPressure < 0;
  STABILIZATION_condition = telemetry.GPS_ALTITUDE > threshold_alt || telemetry.ALTITUDE > threshold_alt; // Are you stabilizing?
  DESCENT_condition = (deltaGPSAlt < 0); // Are you falling?
  LANDED_condition = (telemetry.GPS_ALTITUDE < 1800 || telemetry.ALTITUDE < 1800) && (telemetry.PRESSURE > 980) && (telemetry.PRESSURE < 1013.25);
  // Annie are you okay? Are you okay Annie?
//---------------------------------------------------------------------------------------------------------------------------------------

// //TESTING PURPOSES
//   GPSAlt = millis(); //TESTING PURPOSES; Always increasing
//   currentAlt = millis(); //TESTING PURPOSES; Always increasing
//   currentPressure = millis() * -1; //TESTING PURPOSES; Always decreasing

  if (CURRENT_STATE == LAUNCH){
    if (ASCENT_condition){
      if(timeElapsed - SinceAscentTrue >= duration){
        CURRENT_STATE = ASCENT; 
      }
    }
    else{
      SinceAscentTrue = timeElapsed; 
    } // If Ascent Condition is not true, keep updating until it is true.  
  }
  else if (CURRENT_STATE == ASCENT){
    if(STABILIZATION_condition){
      if(timeElapsed - SinceStabilizationTrue >= duration){
        CURRENT_STATE = STABILIZATION;
      }
      else{
        SinceStabilizationTrue = timeElapsed; //If stabilization is not true, keep updating until it is true. 
      }
    }
  }
  else if (CURRENT_STATE == STABILIZATION){
    if (DESCENT_condition){
      if(timeElapsed - SinceDescentTrue >= duration){
        CURRENT_STATE = DESCENT;
      }
    }
    else{
      SinceDescentTrue = timeElapsed; //If descent is not true, keep updating until
    }
  }
  else if (CURRENT_STATE == DESCENT){
    if (LANDED_condition){
      if (timeElapsed - SinceLandedTrue >= LandedDuration){
        CURRENT_STATE = LANDED;
      }
      else {
        SinceLandedTrue = timeElapsed; // If landed not true, keep updating until true. 
      }
    }
  }
}


//17 reference vars // Directly reference the struct to save memory
void grabTelemetry (data &telemetry) {
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
  // // M9N outputs
  // telemetry.UTC_TIME = M9Nsensor.getUnixTime();
  // telemetry.GPS_ALTITUDE = M9Nsensor.getAltitude();
  // telemetry.GPS_LATITUDE = M9Nsensor.getLatitude();
  // telemetry.GPS_LONGITUDE = M9Nsensor.getLongitude();
  // telemetry.SIV = M9Nsensor.getSIV();
}

void printTelemetry(state CURRENT_STATE, data &telemetry){
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
  //M9N Unix output----------------------
  Serial.print("ms, UTC_TIME : ");
  Serial.print(telemetry.UTC_TIME);
  // //BME output------------------------
  Serial.print("s, ALTITUDE: ");
  Serial.print(telemetry.ALTITUDE);
  Serial.print("m, PRESSURE: ");
  Serial.print(telemetry.PRESSURE);
  Serial.print("mb, TEMPERATURE: ");
  Serial.print(telemetry.TEMPERATURE);
  Serial.print("°C, HUMIDITY: ");
  Serial.print(telemetry.HUMIDITY);
  //BNO output---------------------------
  Serial.print("%, ACCEL_X: ");
  Serial.println(telemetry.ACCEL_X);
  Serial.print("m/s^2, ACCEL_Y: ");
  Serial.print(telemetry.ACCEL_Y);
  Serial.print("m/s^2, ACCEL_Z: ");
  Serial.println(telemetry.ACCEL_Z);
  Serial.print("m/s^2, GYRO_X: ");
  Serial.print(telemetry.GYRO_X);
  Serial.print(" radians/s, GYRO_Y: ");
  Serial.print(telemetry.GYRO_Y);
  Serial.print(" radians/s, GYRO_Z: ");
  Serial.print(telemetry.GYRO_Z);
  Serial.print(" radians/s, ORIENT_X: ");
  Serial.print(telemetry.ORIENT_X);
  Serial.print("°, ORIENT_Y: ");
  Serial.print(telemetry.ORIENT_Y);
  Serial.print("°, ORIENT_Z: ");
  Serial.print(telemetry.ORIENT_Z);
  Serial.print("°");
  // // M9N outputs-------------------------
  Serial.print(", GPS_ALT: ");
  Serial.print(telemetry.GPS_ALTITUDE);
  Serial.print("m, GPS_LAT: ");
  Serial.print(telemetry.GPS_LATITUDE);
  Serial.print("°, GPS_LONG: ");
  Serial.print(telemetry.GPS_LONGITUDE);
  Serial.print("°, SIV: ");
  Serial.print(telemetry.SIV);
  //INA260 outputs-----------------------
  //Serial.print(", CURRENT: ");
  //Serial.print(ina260.readCurrent());
  //Serial.print(" mA");
  //Serial.print(", BUS VOLTAGE: ");
  //Serial.print(ina260.readBusVoltage());
  //Serial.print(" mV");
  //Serial.print(", POWER: ");
  //Serial.print(ina260.readPower());
  //Serial.print(" mW");
  Serial.read();
  Serial.println();

}

//Serial1
// if(Serial.available() > 0){
//   char receivedChar;
//   receivedChar = Serial.read();
//   if (receivedChar = '\n'){
void printTelemetry1(state CURRENT_STATE, data &telemetry){
    Serial1.print("CURRENT_STATE: ");
  if (CURRENT_STATE == LAUNCH){
    Serial1.print("LAUNCH, ");
  }
  else if (CURRENT_STATE == ASCENT){
    Serial1.print("ASCENT, ");
  }
  else if (CURRENT_STATE == STABILIZATION){
    Serial1.print("STABILIZATION, ");
  }
  else if (CURRENT_STATE == DESCENT){
    Serial1.print("DESCENT, ");
  }
  else if (CURRENT_STATE == LANDED){
    Serial1.print("LANDED, ");
  }
  Serial1.print("MISSION_TIME: ");
  Serial1.print(telemetry.MISSION_TIME);
  //M9N Unix output----------------------
  Serial1.print("ms, UTC_TIME : ");
  Serial1.print(telemetry.UTC_TIME);
  // //BME output------------------------
  Serial1.print("s, ALTITUDE: ");
  Serial1.print(telemetry.ALTITUDE);
  Serial1.print("m, PRESSURE: ");
  Serial1.print(telemetry.PRESSURE);
  Serial1.print("mb, TEMPERATURE: ");
  Serial1.print(telemetry.TEMPERATURE);
  Serial1.print("°C, HUMIDITY: ");
  Serial1.print(telemetry.HUMIDITY);
  //BNO output---------------------------
  Serial1.print("%, ACCEL_X: ");
  Serial1.println(telemetry.ACCEL_X);
  Serial1.print("m/s^2, ACCEL_Y: ");
  Serial1.print(telemetry.ACCEL_Y);
  Serial1.print("m/s^2, ACCEL_Z: ");
  Serial1.println(telemetry.ACCEL_Z);
  Serial1.print("m/s^2, GYRO_X: ");
  Serial1.print(telemetry.GYRO_X);
  Serial1.print(" °/s, GYRO_Y: ");
  Serial1.print(telemetry.GYRO_Y);
  Serial1.print(" °/s, GYRO_Z: ");
  Serial1.print(telemetry.GYRO_Z);
  Serial1.print(" °/s, ORIENT_X: ");
  Serial1.print(telemetry.ORIENT_X);
  Serial1.print("°, ORIENT_Y: ");
  Serial1.print(telemetry.ORIENT_Y);
  Serial1.print("°, ORIENT_Z: ");
  Serial1.print(telemetry.ORIENT_Z);
  Serial1.print("°");
  // // M9N outputs-------------------------
  Serial1.print(", GPS_ALT: ");
  Serial1.print(telemetry.GPS_ALTITUDE);
  Serial1.print("m, GPS_LAT: ");
  Serial1.print(telemetry.GPS_LATITUDE);
  Serial1.print("°, GPS_LONG: ");
  Serial1.print(telemetry.GPS_LONGITUDE);
  Serial1.print("°, SIV: ");
  Serial1.print(telemetry.SIV);
  //INA260 outputs-----------------------
  //Serial1.print(", CURRENT: ");
  //Serial1.print(ina260.readCurrent());
  //Serial1.print(" mA");
  //Serial1.print(", BUS VOLTAGE: ");
  //Serial1.print(ina260.readBusVoltage());
  //Serial1.print(" mV");
  //Serial1.print(", POWER: ");
  //Serial1.print(ina260.readPower());
  //Serial1.print(" mW");
  Serial1.println();
} 
  


// //This likely isn't going to work. PID control is going to require PWM to make solenoids worth it. 
// double PID_X(){
//   /*
// -----------------------------------------------------
// PID CONTROLLER STUFF
// -----------------------------------------------------
// */
// // Target coordinates for PID control
// double error_X; 
// const double setpoint_X = 0.0;
// // PID control variables
// double derivative;
// double integral;
// double proportional;
// double last_error_X;
// double dt;
// unsigned int last_time;
// double output_X; // 
// // PID constants
// const double Kp = 1.0; // Proportional gain
// const double Ki = 0; // Integral gain
// const double Kd = 0.1; // Derivative gain
// unsigned int now;
// // PID declarations;
// /*---------------------------------------------
//   PID controller calculations// Update per loop. 
//   */ 
//   dt = now - last_time;
//   last_time = now;
//   now = millis();
//   error_X = setpoint_X - telemetry.GYRO_X; // 
//   proportional = error_X;
//   integral += error_X * dt;
//   derivative = (error_X - last_error_X) / dt;
//   last_error_X = error_X;
//   output_X = (Kp * proportional) + (Ki * integral) + (Kd * derivative);
// //----------------------------------------------
// if(abs(error_X) > 15){
//   return output_X;
// }
  
// }



