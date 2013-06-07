/*
 * File: SimulatedNMEADataSource.h
 * Description:
 *   Simulates NMEA output based on current system time. Intended for testability outside of the
 *   Arduino environment.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#include <string>
#include "interfaces/IDataSource.h"

class SimulatedNMEADataSource : public IDataSource
{
public:
    SimulatedNMEADataSource()
        : currentlyReading_(false)
    {
        // empty
    }
    
    /*
     * Returns whether there is data available to be read.
     */
    virtual bool available();

    /*
     * Returns one character from the data source.
     */
    virtual int read();
private:
    bool currentlyReading_;
    std::string dataString_;
    std::string::iterator dataStringIterator_;
};