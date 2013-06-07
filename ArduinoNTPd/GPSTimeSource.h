/*
 * File: GPSTimeSource.h
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef GPS_TIMESOURCE_H
#define GPS_TIMESOURCE_H

#if defined(ARDUINO)
#include "Arduino.h"
#endif // defined(ARDUINO)

#include <TinyGPS.h>
#include "ITimeSource.h"
#include "IDataSource.h"

class GPSTimeSource : public ITimeSource
{
public:
    GPSTimeSource(IDataSource &dataSource)
        : dataSource_(dataSource),
          secondsSinceEpoch_(0),
          fractionalSecondsSinceEpoch_(0)
    {
        // empty
    }
    
    virtual ~GPSTimeSource()
    {
        // empty
    }
    
    /*
     * Returns the number of seconds since the epoch (defined currently as
     * 1 January 1900 00:00 UTC per the NTP specification.)
     */
    virtual uint32_t getSecondsSinceEpoch(void) const;
    
    /*
     * Returns the number of fractions of a second. 0xFFFFFFFF/1000
     * would correspond to one millisecond, for example.
     */
    virtual uint32_t getFractionalSecondsSinceEpoch(void) const;
    
    /* 
     * Grabs latest time from the time source.
     */
    virtual bool updateTime(void);
    
private:
    TinyGPS gps_;
    IDataSource &dataSource_;
    uint32_t secondsSinceEpoch_;
    uint32_t fractionalSecondsSinceEpoch_;
};

#endif // GPS_TIMESOURCE_H
