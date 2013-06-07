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

// TODO: move to a config.h or something.
#define RX_PIN 11
#define TX_PIN 12

class SerialDataSource : public IDataSource
{
public:
    SerialDataSource()
        : serialPort_(RX_PIN, TX_PIN)
    {
        // GPS module needs 4800 baud.
        serialPort_.begin(4800);
        
        // Disable everything but $GPRMC
        serialPort_.write("$PSRF103,00,00,01*08\r\n");
        serialPort_.write("$PSRF103,01,00,01*09\r\n");
        serialPort_.write("$PSRF103,02,00,01*0A\r\n");
        serialPort_.write("$PSRF103,03,00,01*0B\r\n");
        serialPort_.write("$PSRF103,04,01,01*0D\r\n");
        serialPort_.write("$PSRF103,05,00,01*0D\r\n");
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
    SoftwareSerial serialPort_;
};

#endif // defined(ARDUINO)

#endif // SERIAL_DATASOURCE_H
