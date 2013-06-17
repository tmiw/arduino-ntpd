/*
 * File: SerialDataSource.h
 * Description:
 *   Data source that retrieves bytes from a serial pin using NewSoftSerial.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef SERIAL_DATASOURCE_H
#define SERIAL_DATASOURCE_H

#if defined(ARDUINO)

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "IDataSource.h"
#include "config.h"

class SerialDataSource : public IDataSource
{
public:
    SerialDataSource()
#ifndef HARDWARE_SERIAL_CLASS
        : serialPort_(GPS_RX_PIN, GPS_TX_PIN)
#endif
    {
        // empty
    }
    
    /*
     * Starts serial comms.
     */
    void begin()
    {
        #ifdef HARDWARE_SERIAL_CLASS
#undef serialPort_
#define serialPort_ HARDWARE_SERIAL_CLASS
#endif

        // GPS module needs 4800 baud.
        serialPort_.begin(4800);
        
        // Disable everything but $GPRMC
        serialPort_.write("$PSRF103,00,00,01*08\r\n");
        serialPort_.write("$PSRF103,01,00,01*09\r\n");
        serialPort_.write("$PSRF103,02,00,01*0A\r\n");
        serialPort_.write("$PSRF103,03,00,01*0B\r\n");
        serialPort_.write("$PSRF103,04,00,01*0C\r\n");
        serialPort_.write("$PSRF103,05,00,01*0D\r\n");
        serialPort_.write("$PSRF103,08,00,01,00*2C\r\n");
        
#ifdef HARDWARE_SERIAL_CLASS
#undef serialPort_
#endif
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
#ifndef HARDWARE_SERIAL_CLASS
    SoftwareSerial serialPort_;
#endif
};

#endif // defined(ARDUINO)

#endif // SERIAL_DATASOURCE_H
