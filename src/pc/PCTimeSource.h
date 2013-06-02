/*
 * File: PCTimeSource.h
 * Description:
 *   Time source implementation for PC/Mac platforms. Intended for testability outside of the
 *   Arduino environment.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef PC_TIMESOURCE_H
#define PC_TIMESOURCE_H

#include "interfaces/ITimeSource.h"

class PCTimeSource : ITimeSource
{
public:
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
    virtual void updateTime(void);
    
private:
    uint32_t secondsSinceEpoch;
};

#endif // PC_TIMESOURCE_H