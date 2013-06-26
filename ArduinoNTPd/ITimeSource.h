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
     * Grabs latest time from the time source.
     */
    virtual void now(uint32_t *secs, uint32_t *fract) = 0;
    
    /*
     * Ethernet receive time.
     */
    virtual uint32_t timeRecv(uint32_t *secs, uint32_t *fract) const = 0;
};

#endif // I_TIMESOURCE_H
