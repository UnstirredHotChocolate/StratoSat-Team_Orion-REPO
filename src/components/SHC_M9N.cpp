#include "Arduino.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "Utils.h"
#include "SHC_M9N.h"

int mostRecentYear = 0;
int mostRecentMonth = 0;
int mostRecentDay = 0;
int mostRecentHour = 0;
int mostRecentMinute = 0;
int mostRecentSecond = 0;
int mostRecentSIV = 0;
double mostRecentLongitude = 0.0;
double mostRecentLatitude = 0.0;
double mostRecentAltitude = 0.0;


// void pvtCallback(UBX_NAV_PVT_data_t *ubxDataStruct) {
//     mostRecentPos = {
//         ((double)ubxDataStruct->lat) * pow(10, -7),
//         ((double)ubxDataStruct->lon) * pow(10, -7),
//         ubxDataStruct->height / 1000.0
//     };   
//     mostRecentTime = {
//         (int)ubxDataStruct->year,
//         (int)ubxDataStruct->month,
//         (int)ubxDataStruct->day,
//         (int)ubxDataStruct->hour,
//         (int)ubxDataStruct->min,
//         (int)ubxDataStruct->sec,
//     };
//     mostRecentSIV = ubxDataStruct->numSV;
// }
void pvtCallback(UBX_NAV_PVT_data_t *ubxDataStruct) {
    mostRecentLatitude = ((double)ubxDataStruct->lat) * pow(10, -7);
    mostRecentLongitude = ((double)ubxDataStruct->lon) * pow(10, -7);
    mostRecentAltitude = ubxDataStruct->height / 1000.0;
      
    mostRecentYear = (int)ubxDataStruct->year;
    mostRecentMonth = (int)ubxDataStruct->month;
    mostRecentDay = (int)ubxDataStruct->day;
    mostRecentHour = (int)ubxDataStruct->hour;
    mostRecentMinute = (int)ubxDataStruct->min;
    mostRecentSecond = (int)ubxDataStruct->sec;
    mostRecentSIV = ubxDataStruct->numSV;
}

// Copied directly from the SparkFun library, except we use the data we've already got from the callback
// instead of polling for new data (which takes too long!)
//
// I would much rather calculate directly using the GPS week and time-of-week, but we do not currently
// have time for that. For future reference the formula then would be:
// (week * 604800) + (timeOfWeek / 1000) + 315964800 - leapCount
// Where:
// - 604800 is "seconds in a week"
// - timeOfWeek is milliseconds
// - 315964800 is "seconds between Unix epoch and GPS epoch" (10 years, 6 days)
// - leapCount is the current number of leap seconds in Unix time
//
// For now, this will do. Also, the above would probably slow down the GPS due to requiring
// RAWX data packets that have a LOT more data in them.
uint32_t calculateUnixTime()
{
  uint32_t t = SFE_UBLOX_DAYS_FROM_1970_TO_2020;                                                   // Jan 1st 2020 as days from Jan 1st 1970
  t += (uint32_t)SFE_UBLOX_DAYS_SINCE_2020[mostRecentYear - 2020];                                 // Add on the number of days since 2020
  t += (uint32_t)SFE_UBLOX_DAYS_SINCE_MONTH[mostRecentYear % 4 == 0 ? 0 : 1][mostRecentMonth - 1]; // Add on the number of days since Jan 1st
  t += (uint32_t)mostRecentDay - 1;                                                                // Add on the number of days since the 1st of the month
  t *= 24;                                                                                         // Convert to hours
  t += (uint32_t)mostRecentHour;                                                                   // Add on the hour
  t *= 60;                                                                                         // Convert to minutes
  t += (uint32_t)mostRecentMinute;                                                                 // Add on the minute
  t *= 60;                                                                                         // Convert to seconds
  t += (uint32_t)mostRecentSecond;                                                                 // Add on the second
  return t;
}

Error M9N::init() {
    if (!m9n.begin()) {
        return M9N_ERROR;
    }
    m9n.setI2COutput(COM_TYPE_UBX);
    m9n.setNavigationFrequency(5);
    if (!m9n.setDynamicModel(DYN_MODEL_AIRBORNE4g)) {
        return M9N_ERROR;
    }
    m9n.saveConfiguration();
    m9n.setAutoPVTcallbackPtr(&pvtCallback);

    // bool sivCheck = config.waitForGPSLock;

    // Make it not block
    // while (sivCheck) {
    //     m9n.checkUblox();
    //     m9n.checkCallbacks();
    //     switch (mostRecentSIV) {
    //         // TODO ERRORS
    //         default:
    //             sivCheck = false;
    //             break;
    //     }
    // }
    return NO_ERROR;
}

bool M9N::prefetchData() {
    m9n.checkUblox();
    m9n.checkCallbacks();
    longitude = mostRecentLongitude;
    latitude = mostRecentLatitude;
    altitude = mostRecentAltitude;
    year = mostRecentYear;
    month = mostRecentMonth;
    day = mostRecentDay;
    hour = mostRecentHour;
    minute = mostRecentMinute;
    second = mostRecentSecond;
    SIV = mostRecentSIV;
    return true;
}

double M9N::getLatitude() {
    return latitude;
}
double M9N::getLongitude() {
    return longitude;
}
double M9N::getAltitude(){
    return altitude;
}

int M9N::getYear() {
    return year;
}
int M9N::getMonth() {
    return month;
}
int M9N::getDay() {
    return day;
}
int M9N::getHour() {
    return hour;
}
int M9N::getMinute() {
    return minute;
}
int M9N::getSecond() {
    return second;
}

unsigned int M9N::getUnixTime() {
    return (unsigned int) calculateUnixTime();
}

int M9N::getSIV() {
    return SIV;
}