/*
 * File: TimeUtilities.cpp
 * Description:
 *   Common utility code for time manipulation.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#include "TimeUtilities.h"

uint32_t TimeUtilities::numberOfSecondsSince1900Epoch(
    uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{
    uint32_t returnValue = 0;
    
    // February is 28 here, but we will account for leap years further down.
    static int numDaysInMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Hours, minutes and regular seconds are trivial to add. 
    returnValue = 
        second + 
        (minute * SECONDS_IN_MINUTE) + 
        (hour * SECONDS_IN_MINUTE * MINUTES_IN_HOUR);
    
    // Leap second handling. For each year between 1972 and the provided year,
    // add 1 second for each 1 bit in the leapSeconds array.
    if (year >= LEAP_SECOND_YEAR)
    {
        returnValue += LEAP_SECOND_CATCHUP_VALUE;
        
        for (uint32_t currentYear = LEAP_SECOND_YEAR; currentYear < year; currentYear++)
        {
            returnValue += numberOfLeapSecondsInYear(currentYear, false);
        }
    
        // For the current year, only add the June leap second if the current month is 
        // >= July.
        if (month >= 7)
        {
            returnValue += numberOfLeapSecondsInYear(year, true);
        }
    }
    
    // Days, months and years are as well, with several caveats: 
    //   a) We need to account for leap years.
    //   b) We need to account for different sized months.
    uint32_t numDays = 0;
    for (uint32_t currentYear = EPOCH_YEAR; currentYear <= year; currentYear++)
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
    for (uint32_t currentMonth = 1; currentMonth < month; currentMonth++)
    {
        numDays += numDaysInMonths[currentMonth];
    }
    numDays += day - 1;
    returnValue += numDays * SECONDS_IN_MINUTE * MINUTES_IN_HOUR * HOURS_IN_DAY;
    
    // Return final result.
    return returnValue;
}

uint32_t TimeUtilities::numberOfLeapSecondsInYear(uint32_t year, bool skipDecember)
{
    // Leap second bit vector. Every group of two bits is the 6/30 leap second
    // and 12/31 leap second, respectively, beginning from 1972.
    // NOTE: update this whenever IERS announces a new leap second. No, it's 
    //       not optimal.
    static uint32_t leapSecondAdds[] = {
        0xD5552A21, // 1972-1988
        0x14A92400, // 1989-2005
        0x10408000, // 2006-2022
        0x00000000  // 2023-2039
    };
    static uint32_t leapSecondDeletes[] = {
        0x00000000, // 1972-1988
        0x00000000, // 1989-2005
        0x00000000, // 2006-2022
        0x00000000  // 2023-2039
    };
    
    const int numBitsPerEntry = (sizeof(uint32_t) * 8);
    uint32_t yearDiff = year - LEAP_SECOND_YEAR;
    uint32_t leapSecondAddEntry = leapSecondAdds[(2*yearDiff) / numBitsPerEntry];
    uint32_t leapSecondDeleteEntry = leapSecondDeletes[(2*yearDiff) / numBitsPerEntry];
    uint32_t leapSecondMaskJune = 1 << (numBitsPerEntry - (2*yearDiff) - 1);
    uint32_t leapSecondMaskDecember = skipDecember ? 0 : (1 << (numBitsPerEntry - (2*yearDiff) - 2));
    
    return
        ((leapSecondAddEntry & leapSecondMaskJune) ? 1 : 0) +
        ((leapSecondAddEntry & leapSecondMaskDecember) ? 1 : 0) -
        ((leapSecondDeleteEntry & leapSecondMaskJune) ? 1 : 0) -
        ((leapSecondDeleteEntry & leapSecondMaskDecember) ? 1 : 0);
}
