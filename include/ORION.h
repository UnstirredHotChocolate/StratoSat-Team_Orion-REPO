#ifndef ORION_H
#define ORION_H

enum state {LAUNCH, ASCENT, STABILIZATION, DESCENT, LANDED}; //How we'll set the state




struct data {

unsigned int MISSION_TIME; // in milliseconds
unsigned int UTC_TIME; //in seconds
int SIV; //Satellites In View
state CURRENT_STATE;
float ALTITUDE;// in meters
float TEMPERATURE; // in degrees Celsius
float PRESSURE; // in hPa
float HUMIDITY; // in Percentage

double ACCEL_X; // in m/s^2
double ACCEL_Y; // in m/s^2
double ACCEL_Z; // in m/s^2
double GYRO_X; // in radians per second
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