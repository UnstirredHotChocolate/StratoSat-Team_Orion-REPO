#ifndef ORION_H

#define ORION_H

enum state {LAUNCH, ASCENT, STABILIZATION, DESCENT, LANDED} CURRENT_STATE; //How we'll set the state




struct data {
state CURRENT_STATE;
int ALTITUDE;// 
int FIVESEC_ALT; //
float TEMPERATURE;
float PRESSURE;
float HUMIDITY;

double GYRO_X; // Use in Proportional Intergral Derivative (PID) Controller
double GYRO_Y; // Use in PID Controller
double GYRO_Z; // Use in PID Controller

};






#endif //ORION_H