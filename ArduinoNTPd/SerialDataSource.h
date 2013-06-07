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
#define TX_PIN 10

class SerialDataSource : public IDataSource
{
public:
    SerialDataSource()
        : serialPort_(RX_PIN, TX_PIN)
    {
        // GPS module needs 4800 baud.
        serialPort_.begin(4800);
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
