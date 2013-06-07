/*
 * File: ITimeSource.h
 * Description:
 *   Interface abstraction for time sources. Intended for testability outside of the
 *   Arduino environment.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef I_TIMESOURCE_H
#define I_TIMESOURCE_H

#include <stdint.h>

class ITimeSource
{
public:         
    /*
     * Returns the number of seconds since the epoch (defined currently as
     * 1 January 1900 00:00 UTC per the NTP specification.)
     */
    virtual uint32_t getSecondsSinceEpoch(void) const = 0;
    
    /*
     * Returns the number of fractions of a second. 0xFFFFFFFF/1000
     * would correspond to one millisecond, for example.
     */
    virtual uint32_t getFractionalSecondsSinceEpoch(void) const = 0;
    
    /* 
     * Grabs latest time from the time source.
     */
    virtual bool updateTime(void) = 0;
};

#endif // I_TIMESOURCE_H
