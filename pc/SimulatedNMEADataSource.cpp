/*
 * File: SimulatedNMEADataSource.cpp
 * Description:
 *   Simulates NMEA output based on current system time. Intended for testability outside of the
 *   Arduino environment.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#include "SimulatedNMEADataSource.h"
#include <ctime>
#include <sstream>
#include <iomanip>

bool SimulatedNMEADataSource::available()
{
    if (currentlyReading_ == false)
    {
        // Generate new data string.
        time_t currentTime;
        struct tm *timeStruct;
        currentTime = time(NULL);
        timeStruct = gmtime(&currentTime);
        
        unsigned char checksum = 0;
        std::stringstream ss;
        ss << "$GPRMC,"
           << std::setfill('0') << std::setw(2) << timeStruct->tm_hour 
           << std::setfill('0') << std::setw(2) << timeStruct->tm_min 
           << std::setfill('0') << std::setw(2) << timeStruct->tm_sec
           << ".000,A,3723.2475,N,12158.3416,W,0.13,309.62," 
           << std::setfill('0') << std::setw(2) << timeStruct->tm_mday
           << std::setfill('0') << std::setw(2) << timeStruct->tm_mon 
           << std::setfill('0') << std::setw(2) << (timeStruct->tm_year - 100)
           << ", ,";
        
        dataString_ = ss.str();

        // Generate checksum from the string.
        for (int count = 0; count < dataString_.size(); count++)
        {
            if (dataString_[count] != '$')
            {
                checksum ^= dataString_[count];
            }
        }
        ss << "*" << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)checksum;
        dataString_ = ss.str() + "\r\n";
        dataStringIterator_ = dataString_.begin();
        currentlyReading_ = true;
    }
    else if (currentlyReading_ == true && dataStringIterator_ == dataString_.end())
    {
        currentlyReading_ = false;
    }
    return currentlyReading_;
}

int SimulatedNMEADataSource::read()
{
    return *dataStringIterator_++;
}