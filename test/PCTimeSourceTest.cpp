/*
 * File: PCTimeSourceTestApp.cpp
 * Description:
 *   Test application for PC time source. Mainly intended to test time utility code.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#include <iostream>
#include "common/TimeUtilities.h"
#include "pc/PCTimeSource.h"
#include "pc/SimulatedNMEADataSource.h"
#include "common/GPSTimeSource.h"

int main(void)
{
/*    PCTimeSource timeSource;
    timeSource.updateTime();
    
    std::cout << "Current time (seconds since 1900): " << timeSource.getSecondsSinceEpoch() << std::endl;
    
    int totalLeapSeconds = 0;
    for (int year = TimeUtilities::LEAP_SECOND_YEAR; year < 2013; year++)
    {
        int ls = TimeUtilities::numberOfLeapSecondsInYear(year, false);
        totalLeapSeconds += ls;
        
        std::cout << "leap seconds in " << year << ": " 
                  << ls 
                  << std::endl;
    }
    std::cout << "total leap seconds: " << totalLeapSeconds << std::endl;*/
    
    SimulatedNMEADataSource source;
    GPSTimeSource gps(source);
    
    while(true)
    {
        gps.updateTime();
        std::cout << "secs since 1900: " << gps.getSecondsSinceEpoch() << std::endl;
        sleep(1);
    }
    
    return 0;
}