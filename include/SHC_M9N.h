#ifndef M9N_h
#define M9N_h

#include "Arduino.h"
#include "Utils.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>


class M9N : public Sensor {
private:    
    // // Cached data 
    // int mostRecentYear = 0;
    // int mostRecentMonth = 0;
    // int mostRecentDay = 0;
    // int mostRecentHour = 0;
    // int mostRecentMinute = 0;
    // int mostRecentSecond = 0;
    // int mostRecentSIV = 0;
    // double mostRecentLongitude = 0.0;
    // double mostRecentLatitude = 0.0;
    // double mostRecentAltitude = 0.0;

    SFE_UBLOX_GNSS m9n;
    double longitude = 0.0;
    double latitude = 0.0;
    double altitude = 0.0;
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int SIV = 0;
public:
    Error init();
    bool prefetchData();
    double getLatitude();
    double getLongitude();
    double getAltitude();
    int getYear();
    int getMonth();
    int getDay();
    int getHour();
    int getMinute();
    int getSecond();
    unsigned int getUnixTime();
    int getSIV();
};


#endif