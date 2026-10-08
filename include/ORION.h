#ifndef ORION_H
#define ORION_H

enum state {LAUNCH, ASCENT, STABILIZATION, DESCENT, LANDED} CURRENT_STATE; //How we'll set the state




struct data {

unsigned int MISSION_TIME;
unsigned int UTC_TIME;
state CURRENT_STATE;
float ALTITUDE;// 
float TEMPERATURE;
float PRESSURE;
float HUMIDITY;

double ACCEL_X; 
double ACCEL_Y;
double ACCEL_Z;
double GYRO_X; // Use in Proportional Intergral Derivative (PID) Controller
double GYRO_Y; // Use in PID Controller
double GYRO_Z; // Use in PID Controller
double ORIENT_X;
double ORIENT_Y;
double ORIENT_Z;

double GPS_LATITUDE;
double GPS_LONGITUDE;
double GPS_ALTITUDE;
};

data telemetry;



#endif //ORION_H