/*
 * File: TimeUtilities.cpp
 * Description:
 *   Common utility code for time manipulation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#include "TimeUtilities.h"

unsigned long TimeUtilities::numberOfSecondsSince1900Epoch(
    int year, int month, int day, int hour, int minute, int second)
{
    unsigned long returnValue = 0;
    
    // February is 28 here, but we will account for leap years further down.
    int numDaysInMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Hours, minutes and seconds are trivial to add. 
    // TODO: Leap seconds would seriously complicate things, but are probably 
    //       necessary at some point.
    returnValue = 
        second + 
        (minute * SECONDS_IN_MINUTE) + 
        (hour * SECONDS_IN_MINUTE * MINUTES_IN_HOUR);
    
    // Days, months and years are as well, with several caveats: 
    //   a) We need to account for leap years.
    //   b) We need to account for different sized months.
    int numDays = 0;
    for (int currentYear = EPOCH_YEAR; currentYear <= year; currentYear++)
    {
        int multipleOfFour = (currentYear % 4) == 0;
        int multipleOfOneHundred = (currentYear % 100) == 0;
        int multipleOfFourHundred = (currentYear % 400) == 0;
        
        // Formula: years divisble by 4 are leap years, EXCEPT if it's
        // divisible by 100 and not by 400.
        if (multipleOfFour && !(multipleOfOneHundred && !multipleOfFourHundred))
        {
            numDays++;
        }
    }
    numDays += DAYS_IN_YEAR * (year - EPOCH_YEAR);
    for (int currentMonth = 1; currentMonth < month; currentMonth++)
    {
        numDays += numDaysInMonths[currentMonth];
    }
    numDays += day - 1;
    returnValue += numDays * SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY;
    
    // Return final result.
    return returnValue;
}