/*
 * File: GPSTimeSource.cpp
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
 
#include "GPSTimeSource.h"

uint32_t GPSTimeSource::getSecondsSinceEpoch(void) const
{
    return 0; // TODO
}

uint32_t GPSTimeSource::getFractionalSecondsSinceEpoch(void) const
{
    return 0; // TODO
}

void GPSTimeSource::updateTime(void)
{
    while (dataSource_.available())
    {
        int c = dataSource_.read();
        if (gps_.encode(c))
        {
            // TODO
        }
    }
}