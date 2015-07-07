/*
 * File: TimeUtilities.h
 * Description:
 *   Common utility code for time manipulation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef TIME_UTILITIES_H
#define TIME_UTILITIES_H

#include <stdint.h>

class TimeUtilities
{
public:
    /*
     * The epoch year. Currently 1900.
     */
    static const int EPOCH_YEAR = 1900;
    
    /*
     * The year leap seconds began.
     */
    static const int LEAP_SECOND_YEAR = 1972;
    
    /*
     * The amount of seconds we were already behind by by 1972.
     * This is added to all times in year >= 1972 before the addition
     * of leap seconds.
     */
    static const int LEAP_SECOND_CATCHUP_VALUE = 10;
          
    /*
     * Time/date constants that will probably never change.
     */
    static const int DAYS_IN_YEAR = 365;
    static const int HOURS_IN_DAY = 24;
    static const int SECONDS_IN_MINUTE = 60;
    static const int MINUTES_IN_HOUR = 60;
 
    /*
     * Calculates the number of seconds between 1 January 1900 and 
     * the date/time given.
     * WARNING: this will fail beyond 2036 due to data type overflow.
     * NOTE: this does not account for leap seconds, so there may be 
     *       some variation.
     */
    static uint32_t numberOfSecondsSince1900Epoch(
        uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);

    /*
     * Returns whether a year is a leap year.
     */
    static bool isLeapYear(uint32_t year);

    /*
     * Returns the number of leap seconds that occurred in the given year.
     */
    static uint32_t numberOfLeapSecondsInYear(uint32_t year, bool skipDecember);
    
    /*
     * Returns the date components from the number of seconds.
     */
    static void dateFromNumberOfSeconds(
        uint32_t secs, uint32_t *year, uint32_t *month, uint32_t *day,
        uint32_t *hour, uint32_t *minute, uint32_t *second);
};

#endif // TIME_UTILITIES_H
