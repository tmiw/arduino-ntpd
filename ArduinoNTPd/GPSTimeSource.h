/*
 * File: GPSTimeSource.h
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef GPS_TIMESOURCE_H
#define GPS_TIMESOURCE_H

// Uncomment the below to take advantage of the PPS output of the EM-406.
// This will allow more accurate results to be sent to clients.
// See http://arduino.cc/en/Reference/AttachInterrupt for the correct value here.
#define PPS_INTERRUPT_LINE 4
#define PPS_PIN 19

#if defined(ARDUINO)
#include "Arduino.h"
#endif // defined(ARDUINO)

#include <TinyGPS.h>
#include "ITimeSource.h"
#include "IDataSource.h"

/*
 * A time source based on GPS satellite time.
 * WARNING: only one instance of this class should be instantiated if PPS mode is enabled.
 */
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

    /*
     * Enables interrupts.
     */
    void enableInterrupts();    
private:
    static GPSTimeSource *Singleton_;
    
    TinyGPS gps_;
    IDataSource &dataSource_;
    uint32_t secondsSinceEpoch_;
    uint32_t fractionalSecondsSinceEpoch_;
    uint32_t millisecondsOfLastUpdate_;
    
#ifdef PPS_INTERRUPT_LINE
    static void PpsInterrupt_();
#endif // PPS_INTERRUPT_LINE
};

#endif // GPS_TIMESOURCE_H
