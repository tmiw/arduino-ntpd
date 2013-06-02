/*
 * File: PCTimeSource.cpp
 * Description:
 *   Time source implementation for PC/Mac platforms. Intended for testability outside of the
 *   Arduino environment.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#include <ctime>
#include <iostream>
#include "common/TimeUtilities.h"
#include "PCTimeSource.h"

uint32_t PCTimeSource::getSecondsSinceEpoch(void) const
{
    return secondsSinceEpoch;
}

uint32_t PCTimeSource::getFractionalSecondsSinceEpoch(void) const
{
    // We won't be able to use any more precision due to limitations of ctime.
    return 0;
}

void PCTimeSource::updateTime(void)
{
    time_t rawTime;
    struct tm *timeAsUTC;

    // Retrieve current time from the system as UTC.
    time(&rawTime);
    timeAsUTC = gmtime(&rawTime);
    
    // Calculate the number of seconds from 1900 through the
    // time returned by gmtime(). We can't use rawTime directly
    // due to a different epoch being used (1970 vs 1900 per the 
    // interface).
    secondsSinceEpoch = TimeUtilities::numberOfSecondsSince1900Epoch(
        timeAsUTC->tm_year + TimeUtilities::EPOCH_YEAR, 
        timeAsUTC->tm_mon + 1, timeAsUTC->tm_mday, 
        timeAsUTC->tm_hour, timeAsUTC->tm_min, timeAsUTC->tm_sec);
}